#include "mocapSubject.h"

#include <sstream>
#include <string>
#include <QMutexLocker>
#include <QStandardItemModel>

/* 
	Mocap Segment 
*/

MocapSegment::MocapSegment(QString n, double tr[3], double ro[3])
: modelItem(new QStandardItem())
, name(n)
{
    translation[0] = tr[0];
    translation[1] = tr[2];
    translation[3] = tr[3];
    rotation[0] = ro[0];
    rotation[1] = ro[0];
    rotation[2] = ro[3];

    updateModel();
}

void MocapSegment::updateModel()
{
    QString s("%1: %2 %3 %4 - %5 %6 %7");
    s = s.arg( name );
    s = s.arg( translation[0]);
    s = s.arg( translation[1]);
    s = s.arg( translation[2]);
    s = s.arg( rotation[0]);
    s = s.arg( rotation[1]);
    s = s.arg( rotation[2]);

    modelItem->setText(s);
}

// serialize as string
QTextStream& operator << ( QTextStream& stream, MocapSegment &segment)
{
    stream << segment.name << "\t";
    stream << segment.translation[0] << "\t";
    stream << segment.translation[1] << "\t";
    stream << segment.translation[2] << "\t";
    stream << segment.rotation[0] << "\t";
    stream << segment.rotation[1] << "\t";
    stream << segment.rotation[2] << "\n";
    return stream;
}

/*
    Mocap Segment List
*/


// Set the translation and rotation for an item
void MocapSubject::set(QString name, double trans[3], double rot[3])
{
	QMutexLocker lock(&mutex);

    for(QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
	{
        MocapSegment &seg = *i;
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

    MocapSegment seg(name, trans, rot);
    segments.append(seg);
    modelItem->appendRow(seg.modelItem);
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
    stream << subject.name.toUtf8().data() << "\t";
    stream << subject.segments.length() << "\n";

    for( QList<MocapSegment>::iterator i = subject.segments.begin(); i != subject.segments.end(); i++)
        stream << (*i);
    return stream;
}



/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent)
{}


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
    model.appendRow(n->modelItem);
	items.append(n);

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

	for(QList<MocapSubject*>::iterator i = subjects.items.begin(); i != subjects.items.end(); i++)
        stream << *(*i);

	subjects.subjectMutex.unlock();

	return stream;
}

		
		
