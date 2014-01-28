#include "mocapSubject.h"

#include <sstream>
#include <string>
#include <QMutexLocker>

/* 
	Mocap Segment 
*/
// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapSegment &segment)
{
    stream << segment.name.c_str() << "\t";
    stream << segment.translation[0] << "\t";
    stream << segment.translation[1] << "\t";
    stream << segment.translation[2] << "\t";
    stream << segment.rotation[0] << "\t";
    stream << segment.rotation[1] << "\t";
    stream << segment.rotation[2] << "\n";
    return stream;
};

/*
    Mocap Segment List
*/

MocapSegmentList::MocapSegmentList(QMutex &m, QObject *parent)
: QObject(parent)
, mutex(m)
{}

// Set the translation and rotation for an item
void MocapSegmentList::set(std::string name, double trans[3], double rot[3])
{
	QMutexLocker lock(&mutex);

	//for( QList<MocapSegment>::iterator i = items.begin(); i != items.end(); i++)
    for(int i=0; i < items.length(); i++)
	{
		MocapSegment &seg = items[i];
		if(seg.name == name)
		{
			seg.translation[0] = trans[0];
			seg.translation[1] = trans[1];
			seg.translation[2] = trans[2];
			seg.rotation[0]    = rot[0];
			seg.rotation[1]    = rot[1];
			seg.rotation[2]    = rot[2];
			return;
		}
	}

	MocapSegment seg;
	seg.name = name;
	seg.translation[0] = trans[0];
	seg.translation[1] = trans[1];
	seg.translation[2] = trans[2];
	seg.rotation[0]    = rot[0];
	seg.rotation[1]    = rot[1];
	seg.rotation[2]    = rot[2];
	items.append(seg);
}

QTextStream& operator << ( QTextStream& stream, MocapSegmentList &segments)
{
    stream << segments.items.length() << "\n";

    for( QList<MocapSegment>::iterator i = segments.items.begin(); i != segments.items.end(); i++)
        stream << (*i);

    return stream;

}


/*
	Mocap Subject
*/

MocapSubject::MocapSubject(QString n, QMutex &m, QObject *parent)
: QObject(parent)
, segments(m, this)
, name(n)
, mutex(m)
{};

QTextStream& operator << ( QTextStream& stream, MocapSubject &subject)
{
    stream << subject.name.toUtf8().data() << "\t";
    stream << subject.segments;
    return stream;
}

/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent) {}


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

	MocapSubject *n = new MocapSubject(inName, subjectMutex, this);
	items.append(n);

	return n;
}

QTextStream& operator << ( QTextStream& stream, MocapSubjectList &subjects)
{
	subjects.subjectMutex.lock();
	stream << subjects.items.length() << "\n";

	for(QList<MocapSubject*>::iterator i = subjects.items.begin(); i != subjects.items.end(); i++)
        stream << *(*i);

	subjects.subjectMutex.unlock();

	return stream;
}

		
		
