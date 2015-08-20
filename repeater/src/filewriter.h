#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QLineEdit>
#include <QFile>
#include <QPushButton>

#include "mocapSubject.h"

class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(MocapSubjectList*, QPushButton*, QLineEdit *, QObject *parent = 0);

    QFile            *file;
    QPushButton      *button;
    QLineEdit        *lineEdit;
    MocapSubjectList *subjectList;


signals:
    bool isIdle(bool);
public slots:

    void toggleWriting();

    void writeFrame();



};

#endif // FILEWRITER_H
