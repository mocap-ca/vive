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
#include <QDataStream>

#include <QObject>
#include <QList>
#include <QMutex>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextStream>
#include <QString>
#include "defines.h"

#define ISTREAM QTextStream
#define OSTREAM QTextStream
#define STRING  QString
#define LIST    QList
#define FLOAT   float

#include "mocapData.h"

class MocapSegmentList;
class MocapSubjectList;

/*

// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	Q_OBJECT
public:
    MocapSubject(const SubjectData &, QObject *parent = NULL, QMutex *mutex = NULL);
    MocapSubject(QString name, int clientId, bool isLocal, QObject *parent=NULL, QMutex *mutex=NULL);

    bool operator==(const MocapSubject &);
    bool operator==(const SubjectData &);

    SubjectData         data;
    QMutex*             mutex;
};

*/

// A list of mocap subjects
class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);

	QMutex subjectMutex;

//    MocapCollection data;

//    MocapSubject* find(QString name, int id);


 //public slots:
    //! Updates an existing subject
  //  void update(const MocapSubject *);
};


#endif
