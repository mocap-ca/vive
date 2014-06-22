#include "mocapModel.h"
#include <QMutex>
#include <QList>

MocapModel::MocapModel(QObject *parent, MocapSubjectList *s)
    : QAbstractItemModel(parent)
    , subjects(s)
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
    setHeaderData(0, Qt::Horizontal, QString("HELLO"), Qt::DisplayRole);
}


QModelIndex MocapModel::index(int row, int column, const QModelIndex &parent)const
{
    return QModelIndex();
};

QModelIndex MocapModel::parent(const QModelIndex &child)const
{
    return QModelIndex();
};

int         MocapModel::rowCount(const QModelIndex &parent)const
{
    return -1;
};

int         MocapModel::columnCount(const QModelIndex &parent)const
{
    return -1;
};

QVariant    MocapModel::data(const QModelIndex &index, int role)const
{
    return QVariant(0);
};

void MocapModel::update()
{
    QMutexLocker lock(&subjects->subjectMutex);

    //QMap<MocapSubject*, QStandardItem*>::iterator it;
    //QList<MocapSubject*> deleteList = subjectItems.keys();

    // For each Subject
    for(QList<MocapSubject*>::iterator i = subjects->items.begin(); i != subjects->items.end(); i++)
    {
        MocapSubject *subject = *i;

        //deleteList.removeAll(subject);
        QStandardItem *subjectNode = NULL;
        //it = subjectItems.find(subject);
        /*if(it != subjectItems.end())
        {
            subjectNode = *it;
        }
        else
        {
            QList<QStandardItem*> subjectItems;
            subjectNode = new QStandardItem(subject->data.name);
            subjectItems << subjectNode;
            for(size_t i=0; i < 8; i++) subjectItems << new QStandardItem();
            //subjectItems.appendRow(items);
            model->appendRow(subjectItems);
        }*/
    }

    /*
    for(QList<MocapSubject *>::iterator j = deleteList.begin();
            j != deleteList.end();  j++)
    {
        MocapSubject*  delSubject = *j;
        QStandardItem* delItem = subjectItems[ delSubject ];
        QModelIndex    delIndex = model->indexFromItem(delItem);
        model->removeRows(0, 1, delIndex);
        subjectItems.remove(delSubject);
        delete delSubject;
        delete delItem;
    }*/

    /*

        // For each segment
        for( QList<MocapSegment>::iterator i = segments.begin(); i != segments.end(); i++)
        {
            MocapSegment &segment = *i;
            modelItems << new QStandardItem(segment.name);
            for(size_t i=0; i < 3; i++) modelItems << new QStandardItem(QString::number(segment.translation[i], 'f', PREC));
            for(size_t i=0; i < 4; i++) modelItems << new QStandardItem(QString::number(segment.localRotation[i], 'f', PREC));
            model->appendRow(modelItems);

            //modelItems[0]->setText(name);
            //for(int i=0; i < 3; i++)  modelItems[i+1]->setText(QString::number(translation[i],   'f', PREC));
            //for(int i=0; i < 4; i++)  modelItems[i+4]->setText(QString::number(localRotation[i], 'f', PREC));
        }

        // For each marker
        for( QList<MocapMarker>::iterator i = markers.begin(); i != markers.end(); i++)
        {
            MocapMarker &marker = *i;
            modelItems << new QStandardItem(marker.name);
            for(size_t i=0; i < 3; i++) modelItems << new QStandardItem(QString::number(segment.translation[i], 'f', PREC));
            modelItems[0]->setText(name);
            for(int i=1; i < 4; i++)  modelItems[i]->setText(QString::number(translation[i-1], 'f', PREC));
            model->appendRow(modelItems);
        }
    }*/
}


