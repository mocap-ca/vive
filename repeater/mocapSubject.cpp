#include "mocapSubject.h"

#include <sstream>
#include <string>

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

MocapSegmentList::MocapSegmentList(QObject *parent)
: QObject(parent) {}

void MocapSegmentList::set(std::string name, double trans[3], double rot[3])
{
	for( QList<MocapSegment>::iterator i = items.begin(); i != items.end(); i++)
	{
		if((*i).name == name)
		{
			(*i).translation[0] = trans[0];
			(*i).translation[1] = trans[1];
			(*i).translation[2] = trans[2];
			(*i).rotation[0]    = rot[0];
			(*i).rotation[1]    = rot[1];
			(*i).rotation[2]    = rot[2];
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

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent) {}

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


template<typename T>
T& MocapSubjectList::operator << ( T& oss)
{
	if(items.length() == 0) return oss;
	oss << items.length() << "\n";

	for(QList<MocapSubject*>::iterator i = items.begin(); i != items.end(); i++)
		oss << *i;
}

		
		