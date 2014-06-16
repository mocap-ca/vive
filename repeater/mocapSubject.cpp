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

MocapSegment::MocapSegment(const QString n, const double tr[3], const double localRot[4] )
: name(n)
{
    for(size_t i=0; i < 3; i++)  translation[i] = tr[i];
    for(size_t i=0; i < 4; i++)  localRotation[i]  = localRot[i];
}


// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapSegment &segment )
{
    stream << segment.name << "\t";
    for(size_t i=0; i < 3; i++) stream << segment.translation[i]    << "\t";
    for(size_t i=0; i < 4; i++)
    {
        stream << segment.localRotation[i];
        if(i < 3) stream << "\t";
    }
    stream << "\n";
    return stream;
}


MocapMarker::MocapMarker(const QString n, const double tr[3])
: name(n)
{
    for(size_t i=0; i < 3; i++) translation[i] = tr[i];
}


// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapMarker &marker )
{
    stream << marker.name << "\t";
    stream << marker.translation[0] << "\t";
    stream << marker.translation[1] << "\t";
    stream << marker.translation[2] << "\n";
    return stream;
}




/************************
 *    Subject Data      *
 ************************/

SubjectData::SubjectData(QString n, ClientId id)
    : name(n)
    , client(id)
{}

SubjectData::SubjectData(const SubjectData& other)
    : name(other.name)
    , client(other.client)
{
    for(QList<MocapSegment>::const_iterator si = other.segments.begin(); si != other.segments.end(); si++)
    {
        segments.append(MocapSegment( (*si).name, (*si).translation, (*si).localRotation));
    }
    for(QList<MocapMarker>::const_iterator mi = other.markers.begin(); mi != other.markers.end(); mi++)
    {
        markers.append(MocapMarker( (*mi).name, (*mi).translation ));
    }
}

void SubjectData::update(SubjectData &other)
{
    // TODO : remove markers as needed
    for(QList<MocapSegment>::iterator si = other.segments.begin(); si != other.segments.end(); si++)
    {
        setSegment( (*si).name, (*si).translation, (*si).localRotation);
    }
    for(QList<MocapMarker>::iterator mi = other.markers.begin(); mi != other.markers.end(); mi++)
    {
        setMarker( (*mi).name, (*mi).translation );
    }
}

// Set the translation and rotation for an item
void SubjectData::setSegment(QString name, double trans[3], double rot[4])
{
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
}

// Set the translation and rotation for an item
void SubjectData::setMarker(QString name, double trans[3])
{
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
}



/********************
    Mocap Subject
********************/

MocapSubject::MocapSubject(const SubjectData &data, QObject *parent, QMutex *m, bool local)
: QObject(parent)
, data(data)
, mutex(m)
, isLocal(local)
{
}


bool MocapSubject::operator ==(const SubjectData &other)
{
    if(this->data.client != other.client) return false;
    if(this->data.name != other.name) return false;
    return true;
}

QTextStream& operator << ( QTextStream& stream, MocapSubject &subject )
{
    stream << subject.data.name.toUtf8().data() << "\t";
    stream << subject.data.segments.length() << "\t";
    stream << subject.data.markers.length() << "\n";

    for( QList<MocapSegment>::iterator i = subject.data.segments.begin(); i != subject.data.segments.end(); i++)
        stream << *i;

    for( QList<MocapMarker>::iterator i = subject.data.markers.begin(); i != subject.data.markers.end(); i++)
        stream << *i;

    return stream;
}


/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent)
{}


// Find the named subjectm or optionally add it.
MocapSubject* MocapSubjectList::find(QString inName, ClientId id)
{
	QMutexLocker lock(&subjectMutex);

	for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
	{
        if( (*i)->data.name == inName && (*i)->data.client == id )
			return *i;
	}
    return NULL;

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

void MocapSubjectList::update(SubjectData *inSubject)
{
    QMutexLocker lock(&subjectMutex);
    bool found = false;
    for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
    {
        MocapSubject *s = *i;

        if( (*s) == (*inSubject)  )
        {
            s->data.update(*inSubject);
            found = true;
            delete inSubject;
            return;
        }
    }
    if(!found)
    {
        items.append( new MocapSubject(*inSubject, this, &subjectMutex, true ) );
    }

}
