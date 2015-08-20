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
#include "defines.h"

class MocapSegmentList;
class MocapSubjectList;

// A Segment has name, translation and rotation
class MocapSegment
{
public:
    MocapSegment(const QString name, const double tr[3], const double rot[4]);
    QString name;
	double translation[3];
    double localRotation[4];
    friend QTextStream& operator << (  QTextStream&, MocapSegment& );
};

inline bool operator == (const MocapSegment &a, const MocapSegment &b)
{
    return a.name == b.name;
}


class MocapMarker
{
public:
    MocapMarker(const QString name, const double tr[3]);
    QString name;
    double translation[3];
    friend QTextStream& operator << ( QTextStream&, MocapMarker& );
};

inline bool operator == (const MocapMarker &a, const MocapMarker &b)
{
    return a.name == b.name;
}


class SubjectData
{
public:
    SubjectData(const SubjectData &);
    SubjectData(QString name, ClientId id);
    QList<MocapSegment> segments;
    QList<MocapMarker>  markers;
    QString             name;
    ClientId            client;

    void update(SubjectData &other);
    void setSegment(QString name, double trans[3], double rot[4]);
    void setMarker(QString name, double trans[3]);
};


// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	Q_OBJECT
public:
    MocapSubject(const SubjectData&, QObject *parent = NULL, QMutex *mutex = NULL, bool local = true);

    friend QTextStream& operator << ( QTextStream&, MocapSubject& );

    bool operator==(const SubjectData &);

    SubjectData         data;
    QMutex*             mutex;
    bool                isLocal;
};

// A list of mocap subjects
class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);

    //! Finds the subject, or returns null;
    MocapSubject* find(QString name, ClientId id);

    //! Serializes the data as a text stream
    void read(QTextStream &stream, bool localOnly);

	QMutex subjectMutex;

	QList<MocapSubject*> items;

 public slots:
    //! Updates an existing subject
    void update(SubjectData *);
};


#endif
