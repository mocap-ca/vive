#include "mocapSubject.h"

#include <sstream>
#include <string>
#include <QMutexLocker>
#include <QStandardItemModel>

/* 
	Mocap Segment 
*/


#define PREC 2

MocapSegment::MocapSegment(QString n, double tr[3], double ro[3])
: name(n)
{
    translation[0] = tr[0];
    translation[1] = tr[1];
    translation[2] = tr[2];
    rotation[0] = ro[0];
    rotation[1] = ro[1];
    rotation[2] = ro[2];
    rotation[3] = ro[3];

    modelItems << new QStandardItem(name);
    modelItems << new QStandardItem(QString::number(translation[0], 'f', PREC));
    modelItems << new QStandardItem(QString::number(translation[1], 'f', PREC));
    modelItems << new QStandardItem(QString::number(translation[2], 'f', PREC));
    modelItems << new QStandardItem(QString::number(rotation[0], 'f', PREC));
    modelItems << new QStandardItem(QString::number(rotation[1], 'f', PREC));
    modelItems << new QStandardItem(QString::number(rotation[2], 'f', PREC));
    modelItems << new QStandardItem(QString::number(rotation[3], 'f', PREC));

    updateModel();
}

void MocapSegment::updateModel()
{
    modelItems[0]->setText(name);
    modelItems[1]->setText(QString::number(translation[0], 'f', PREC));
    modelItems[2]->setText(QString::number(translation[1], 'f', PREC));
    modelItems[3]->setText(QString::number(translation[2], 'f', PREC));
    modelItems[4]->setText(QString::number(rotation[0], 'f', PREC));
    modelItems[5]->setText(QString::number(rotation[1], 'f', PREC));
    modelItems[6]->setText(QString::number(rotation[2], 'f', PREC));
    modelItems[6]->setText(QString::number(rotation[3], 'f', PREC));

    //QString s("%1: %2 %3 %4 - %5 %6 %7");
    /*s = s.arg( name );
    s = s.arg( translation[0]);
    s = s.arg( translation[1]);
    s = s.arg( translation[2]);
    s = s.arg( rotation[0]);
    s = s.arg( rotation[1]);
    s = s.arg( rotation[2]);*/
    //modelItem->setText(s);

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
    stream << segment.rotation[2] << "\t";
    stream << segment.rotation[3] << "\n";
    return stream;
}

/*
    Mocap Segment List
*/


// Set the translation and rotation for an item
void MocapSubject::set(QString name, double trans[3], double rot[4])
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
            seg.rotation[3]    = rot[3];
            return;
		}
	}

    MocapSegment seg(name, trans, rot);
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
    x << new QStandardItem();
    x << new QStandardItem();
    x << new QStandardItem();
    x << new QStandardItem();
    x << new QStandardItem();
    x << new QStandardItem();
    x << new QStandardItem();
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

	for(QList<MocapSubject*>::iterator i = subjects.items.begin(); i != subjects.items.end(); i++)
        stream << *(*i);

	subjects.subjectMutex.unlock();

	return stream;
}

		
		
