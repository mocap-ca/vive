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
