#include "mocapModel.h"
#include <QMutex>

MocapModel::MocapModel(QObject *parent, MocapSubjectList *s)
    : QObject(parent)
    , subjects(s)
{
    model = new QStandardItemModel(this);
    QStringList headers;
    headers << "Name";
    headers << "tx";
    headers << "ty";
    headers << "tz";
    headers << "rx";
    headers << "ry";
    headers << "rz";
    headers << "rw";
    model->setHorizontalHeaderLabels(headers);
}

void MocapModel::updateModel()
{
    QMutexLocker lock(&subjects->subjectMutex);

    // For each Subject
   /* for(QList<MocapSubject*>::iterator i = subjects->items.begin(); i != subjects->items.end(); i++)
    {
        MocapSubject *subject = *i;
        QList<QStandardItem*> subjectItems;
        QStandardItem *subjectModel = new QStandardItem();
        subjectItems << subjectModel;
        for(size_t i=0; i < 8; i++) subjectItems << new QStandardItem();
        subjectItems.appendRow(items);
        model->appendRow(subjectItems);

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


