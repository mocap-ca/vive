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

MocapSegment::MocapSegment(QString n, double tr[3], double staticRot[4], double localRot[4], double globalRot[4] )
: name(n)
{
    for(size_t i=0; i < 3; i++)  translation[i] = tr[i];
    for(size_t i=0; i < 4; i++)  staticRotation[i] = staticRot[i];
    for(size_t i=0; i < 4; i++)  localRotation[i]  = localRot[i];
    for(size_t i=0; i < 4; i++)  globalRotation[i] = globalRot[i];

    modelItems << new QStandardItem(name);
    for(size_t i=0; i < 3; i++) modelItems << new QStandardItem(QString::number(translation[i], 'f', PREC));
    for(size_t i=0; i < 4; i++) modelItems << new QStandardItem(QString::number(staticRotation[i], 'f', PREC));
    for(size_t i=0; i < 4; i++) modelItems << new QStandardItem(QString::number(localRotation[i], 'f', PREC));
    for(size_t i=0; i < 4; i++) modelItems << new QStandardItem(QString::number(globalRotation[i], 'f', PREC));

    updateModel();
}

void MocapSegment::updateModel()
{
    modelItems[0]->setText(name);
    size_t i=1;
    for(; i < 4; i++)  modelItems[i]->setText(QString::number(translation[i-1],     'f', PREC));
    for(; i < 8; i++)  modelItems[i]->setText(QString::number(staticRotation[i-4],  'f', PREC));
    for(; i < 12; i++) modelItems[i]->setText(QString::number(localRotation[i-8],  'f', PREC));
    for(; i < 16; i++) modelItems[i]->setText(QString::number(globalRotation[i-12], 'f', PREC));
}

// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapSegment &segment)
{
    startProfile("MocapSegment");
    stream << segment.name << "\t";
    for(size_t i=0; i < 3; i++) stream << segment.translation[i]    << "\t";
    for(size_t i=0; i < 4; i++) stream << segment.staticRotation[i] << "\t";
    for(size_t i=0; i < 4; i++) stream << segment.localRotation[i]  << "\t";
    for(size_t i=0; i < 4; i++)
    {
        stream << segment.globalRotation[i];
        if(i < 3) stream << "\t";
    }
    stream << "\n";
    stopProfile("MocapSegment");
    return stream;
}

/*
    Mocap Segment List
*/


// Set the translation and rotation for an item
void MocapSubject::set(QString name, double trans[3], double staticRot[4], double localRot[4], double globalRot[4])
{
	QMutexLocker lock(&mutex);

    for(QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
	{
        MocapSegment &seg = *i;
        if(seg.name == name)
		{
            for(size_t i=0; i < 3; i++) seg.translation[i]    = trans[i];
            for(size_t i=0; i < 4; i++) seg.staticRotation[i] = staticRot[i];
            for(size_t i=0; i < 4; i++) seg.localRotation[i]  = localRot[i];
            for(size_t i=0; i < 4; i++) seg.globalRotation[i] = globalRot[i];
            return;
		}
	}

    MocapSegment seg(name, trans, staticRot, localRot, globalRot);
    segments.append(seg);
    modelItem->appendRow(seg.modelItems);
}


/*
	Mocap Subject
*/

MocapSubject::MocapSubject(QString n, QMutex &m, QObject *parent)
: QObject(parent)
, name(n)
, mutex(m)
, modelItem(new QStandardItem(n))
{}

void MocapSubject::updateModel()
{
    for( QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
        (*i).updateModel();
}

QTextStream& operator << ( QTextStream& stream, MocapSubject &subject)
{
    startProfile("MocapSubject");

    stream << subject.name.toUtf8().data() << "\t";
    stream << subject.segments.length() << "\n";

    for( QList<MocapSegment>::iterator i = subject.segments.begin(); i != subject.segments.end(); i++)
        stream << (*i);

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
    headers << "s-rx";
    headers << "s-ry";
    headers << "s-rz";
    headers << "s-rw";
    headers << "l-rx";
    headers << "l-ry";
    headers << "l-rz";
    headers << "l-rw";
    headers << "g-rx";
    headers << "g-ry";
    headers << "g-rz";
    headers << "g-rw";
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
	MocapSubject *n = new MocapSubject(inName, subjectMutex, this);
    items.append(n);

    // Add it to the model
    QList<QStandardItem*> x;
    x << n->modelItem;
    for(size_t i=0; i < 15; i++) x << new QStandardItem();
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


QTextStream& operator << ( QTextStream& stream, MocapSubjectList &subjects)
{
	subjects.subjectMutex.lock();
	stream << subjects.items.length() << "\n";

    startProfile("total");
	for(QList<MocapSubject*>::iterator i = subjects.items.begin(); i != subjects.items.end(); i++)
        stream << *(*i);
    stopProfile("total");

	subjects.subjectMutex.unlock();

	return stream;
}

		
		
