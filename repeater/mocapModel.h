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
#ifndef MOCAPMODEL_H
#define MOCAPMODEL_H

#include <QStandardItem>
#include <QAbstractItemModel>
#include <QObject>
#include <QModelIndex>
#include <QVariant>
#include "MocapSubject.h"


class MocapModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MocapModel(QObject *parent, MocapSubjectList *);

    void update();

    MocapSubjectList *subjects;

    //QMap<MocapSubject*, QStandardItem*> subjectItems;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    data(const QModelIndex &index, int role) const;

};

#endif // MOCAPMODEL_H
