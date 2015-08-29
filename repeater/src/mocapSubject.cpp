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



/*
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
}*/





/********************
    Mocap Subject
********************/

MocapSubject::MocapSubject( const SubjectData& data, QObject *parent, QMutex *m)
: QObject(parent)
, data(data)
, mutex(m) {}

MocapSubject::MocapSubject(QString name, int clientId, bool isLocal, QObject *parent, QMutex *m)
    : QObject(parent)
    , mutex(m)
    , data(name, clientId, isLocal)
{}


bool MocapSubject::operator ==(const MocapSubject &data)
{    return this->data == data.data; }

bool MocapSubject::operator ==(const SubjectData &data)
{    return this->data == data;  }



/*
	Mocap Subject List
*/

MocapSubjectList::MocapSubjectList(QObject *parent)
: QObject(parent) {}

MocapSubject* MocapSubjectList::find(QString name, int id)
{
    QMutexLocker lock(&subjectMutex);
    SubjectData *sd = data.find(name, id);
    if(sd == NULL) return NULL;
    return new MocapSubject( *sd, this, &subjectMutex );
}

void MocapSubjectList::update(const MocapSubject *inSubject)
{
    QMutexLocker lock(&subjectMutex);
    bool found = false;
    for(QList<SubjectData*>::iterator i = data.items.begin(); i != data.items.end(); i++)
    {
        SubjectData *s = *i;

        if( (*s) == (*inSubject).data  )
        {
            s->update(&(*inSubject).data);
            found = true;
            delete inSubject;
            return;
        }
    }
    if(!found)
    {
        data.items.append( new SubjectData((*inSubject).data) );
    }

}
