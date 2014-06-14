/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr University

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "mocapSubject.h"

#include <sstream>
#include <string>
#include <QMutexLocker>
#include <QStandardItemModel>

#include "profile.h"

/* 
	Mocap Segment 
*/


#define PREC 8

MocapSegment::MocapSegment(QString n, double tr[3], double localRot[4] )
: name(n)
{
    for(size_t i=0; i < 3; i++)  translation[i] = tr[i];
    for(size_t i=0; i < 4; i++)  localRotation[i]  = localRot[i];
}

void MocapSegment::updateModel()
{
    if(modelItems.length() == 0)
    {
        modelItems << new QStandardItem(name);
        for(size_t i=0; i < 3; i++) modelItems << new QStandardItem(QString::number(translation[i], 'f', PREC));
        for(size_t i=0; i < 4; i++) modelItems << new QStandardItem(QString::number(localRotation[i], 'f', PREC));
    }
    modelItems[0]->setText(name);
    for(int i=0; i < 3; i++)  modelItems[i+1]->setText(QString::number(translation[i],   'f', PREC));
    for(int i=0; i < 4; i++)  modelItems[i+4]->setText(QString::number(localRotation[i], 'f', PREC));
}

// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapSegment &segment )
{
    startProfile("MocapSegment");
    stream << segment.name << "\t";
    for(size_t i=0; i < 3; i++) stream << segment.translation[i]    << "\t";
    for(size_t i=0; i < 4; i++)
    {
        stream << segment.localRotation[i];
        if(i < 3) stream << "\t";
    }
    stream << "\n";
    stopProfile("MocapSegment");
    return stream;
}


MocapMarker::MocapMarker(QString n, double tr[3])
: name(n)
{
    for(size_t i=0; i < 3; i++) translation[i] = tr[i];
}

void MocapMarker::updateModel()
{
    if(modelItems.length() == 0)
    {
        modelItems << new QStandardItem(name);
        for(size_t i=0; i < 3; i++) modelItems << new QStandardItem(QString::number(translation[i], 'f', PREC));
    }
    modelItems[0]->setText(name);
    int i=1;
    for(; i < 4; i++)  modelItems[i]->setText(QString::number(translation[i-1], 'f', PREC));
}

// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapMarker &marker )
{
    startProfile("MocapMarker");
    stream << marker.name << "\t";
    stream << marker.translation[0] << "\t";
    stream << marker.translation[1] << "\t";
    stream << marker.translation[2] << "\n";
    stopProfile("MocapMarker");
    return stream;
}




/*
    Mocap Subject
*/


// Set the translation and rotation for an item
void MocapSubject::setSegment(QString name, double trans[3], double rot[4])
{
	QMutexLocker lock(&mutex);

    for(QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
	{
        // Update existing segment
        MocapSegment &seg = *i;
        if(seg.name == name)
		{
            for(size_t i=0; i < 3; i++) seg.translation[i]    = trans[i];
            for(size_t i=0; i < 4; i++) seg.localRotation[i]  = rot[i];
            return;
		}
	}

    // Create new segment
    MocapSegment seg(name, trans, rot);
    segments.append(seg);
    modelItem->appendRow(seg.modelItems);
}

// Set the translation and rotation for an item
void MocapSubject::setMarker(QString name, double trans[3])
{
    QMutexLocker lock(&mutex);

    for(QList<MocapMarker>::iterator i = markers.begin(); i != markers.end(); i++)
    {
        // Update existing marker
        MocapMarker &marker = *i;
        if(marker.name == name)
        {
            for(size_t i=0; i < 3; i++) marker.translation[i]    = trans[i];
            return;
        }
    }

    // Create new marker
    MocapMarker marker(name, trans);
    markers.append(marker);
    modelItem->appendRow(marker.modelItems);
}


MocapSubject::MocapSubject(QString n, QMutex &m, QObject *parent, bool local)
: QObject(parent)
, name(n)
, mutex(m)
, modelItem(new QStandardItem(n))
, isLocal(local)
{}

void MocapSubject::updateModel()
{
    for( QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
        (*i).updateModel();
}

QTextStream& operator << ( QTextStream& stream, MocapSubject &subject )
{
    startProfile("MocapSubject");

    stream << subject.name.toUtf8().data() << "\t";
    stream << subject.segments.length() << "\t";
    stream << subject.markers.length() << "\n";

    for( QList<MocapSegment>::iterator i = subject.segments.begin(); i != subject.segments.end(); i++)
        stream << *i;

    for( QList<MocapMarker>::iterator i = subject.markers.begin(); i != subject.markers.end(); i++)
        stream << *i;

    stopProfile("MocapSubject");
    return stream;
}



/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent)
{
    QStringList headers;
    headers << "Name";
    headers << "tx";
    headers << "ty";
    headers << "tz";
    headers << "rx";
    headers << "ry";
    headers << "rz";
    headers << "rw";
    model.setHorizontalHeaderLabels(headers);
}

MocapSubjectList::~MocapSubjectList()
{
    finishProfile("c://out2.txt");
}


// Find the named subjectm or optionally add it.
MocapSubject* MocapSubjectList::find(QString inName, bool add)
{
	QMutexLocker lock(&subjectMutex);

	for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
	{
		if( (*i)->name == inName )
			return *i;
	}

	if(add == false) return NULL;

    // Not found - create a new subject
    MocapSubject *n = new MocapSubject(inName, subjectMutex, this, true);
    items.append(n);

    // Add it to the model
    QList<QStandardItem*> x;
    x << n->modelItem;
    for(size_t i=0; i < 8; i++) x << new QStandardItem();
    model.appendRow(x);

	return n;
}

void MocapSubjectList::updateModel()
{
    QMutexLocker lock(&subjectMutex);

    for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
    {
        (*i)->updateModel();
    }
}


void MocapSubjectList::read(QTextStream &stream, bool localOnly)
{
    subjectMutex.lock();
    stream << items.length() << "\n";

    startProfile("total");
    for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
    {
        if( localOnly && !(*i)->isLocal ) continue;
        stream << *(*i);
    }
    stopProfile("total");

    subjectMutex.unlock();
}

		
		
