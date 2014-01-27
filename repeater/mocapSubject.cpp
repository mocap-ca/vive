#include "mocapSubject.h"

#include <sstream>
#include <string>


/* 
	Mocap Segment 
*/
// serialize as string
template<typename T>
T& MocapSegment::operator << ( T& oss )
{
	oss << name << "\t";
	oss << translation[0] << "\t";
	oss << translation[1] << "\t";
	oss << translation[2] << "\t";
	oss << rotation[0] << "\t";
	oss << rotation[1] << "\t";
	oss << rotation[2] << "\n";
	return oss;
};

/*
    Mocap Segment List
*/

MocapSegmentList::MocapSegmentList(QObject *parent)
: QObject(parent) {}

// Set the translation and rotation for an item
void MocapSegmentList::set(std::string name, double trans[3], double rot[3])
{
	//for( QList<MocapSegment>::iterator i = items.begin(); i != items.end(); i++)
	for(size_t i=0; i < items.length(); i++)
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

template<typename T>
T& MocapSegmentList::operator << ( T& oss)
{
	oss << items.length() << "\n";

	for( QList<MocapSegment>::iterator i = items.begin(); i != items.end(); i++)
		oss << (*i);

	return pos;

}


/*
	Mocap Subject
*/

MocapSubject::MocapSubject(QString n, QObject *parent)
: QObject(parent)
, name(n) {}

template<typename T>
T& MocapSubject::operator << ( T& oss)
{
	oss << name.toUtf8().data() << "\t";
	oss << segments;
	return oss;
}

/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent) {}


// Find the named subjectm or optionally add it.
MocapSubject* MocapSubjectList::find(QString inName, bool add)
{
	for(QList<MocapSubject*>::iterator i = items.begin();
		i != items.end(); i++)
	{
		if( (*i)->name == inName ) return *i;
	}

	if(add == false) return NULL;

	MocapSubject *n = new MocapSubject(inName, this);
	items.append(n);

	return n;
}

QTextStream& operator << ( QTextStream& stream, MocapSubjectList &subjects)
{
	//if(subjects.items.length() == 0) return stream;
	stream << subjects.items.length() << "\n";

	for(QList<MocapSubject*>::iterator i = subjects.items.begin(); i != subjects.items.end(); i++)
		stream << *i;

	return stream;
}

		
		