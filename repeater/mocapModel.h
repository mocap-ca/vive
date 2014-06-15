#ifndef MOCAPMODEL_H
#define MOCAPMODEL_H

#include <QStandardItem>
#include <QObject>
#include "MocapSubject.h"


class MocapModel : public QObject
{
    Q_OBJECT

public:
    MocapModel(QObject *parent, MocapSubjectList *);

    void updateModel();

    MocapSubjectList *subjects;

    QStandardItemModel *model;

};

#endif // MOCAPMODEL_H
