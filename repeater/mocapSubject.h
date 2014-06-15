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

#ifndef MOCAP_SUBJECT
#define MOCAP_SUBJECT

#include <QTextStream>

#include <QObject>
#include <QList>
#include <QMutex>
#include <QStandardItemModel>
#include <QStandardItem>

class MocapSegmentList;
class MocapSubjectList;

// A Segment has name, translation and rotation
class MocapSegment
{
    friend class MocapSubject;
private:
    MocapSegment(QString name, double tr[3], double rot[4]);
    QString name;
	double translation[3];
    double localRotation[4];
    friend QTextStream& operator << (  QTextStream&, MocapSegment& );
};

class MocapMarker
{
    friend class MocapSubject;
private:
    MocapMarker(QString name, double tr[3]);
    QString name;
    double translation[3];
    friend QTextStream& operator << ( QTextStream&, MocapMarker& );
};


// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	friend class MocapSubjectList;

	Q_OBJECT
public:
    MocapSubject(QString name, QMutex &mutex, QObject *parent, bool local);

    friend QTextStream& operator << ( QTextStream&, MocapSubject& );

    void setSegment(QString name, double trans[3], double rot[4]);

    void setMarker(QString name, double trans[3]);


private :
    QList<MocapSegment> segments;
    QList<MocapMarker> markers;
	QString name;
    QMutex &mutex;

public :
    bool isLocal;
};

// A list of mocap subjects
class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);

	MocapSubject* find(QString name, bool add=true);

    void read(QTextStream &stream, bool localOnly);

	QMutex subjectMutex;

	QList<MocapSubject*> items;
};


#endif
