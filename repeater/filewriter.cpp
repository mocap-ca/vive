#include "filewriter.h"
#include <QTextStream>

FileWriter::FileWriter(MocapSubjectList* slist,
                       QPushButton* pb,
                       QLineEdit *le,
                       QObject *parent) :
    QObject(parent),
    file(NULL),
    button(pb),
    lineEdit(le),
    subjectList(slist)
{
#ifdef _DEBUG
    lineEdit->setText("c:\\cpp\\x.dat");
#endif
}


void FileWriter::toggleWriting()
{
    if(file == NULL)
    {
        QString fileName = lineEdit->text();
        if(fileName.length() == 0) return;
        file = new QFile(fileName);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            delete file;
            file = NULL;
            return;
        }
        file->write("60\n");
        button->setText("Stop");
        emit isIdle(false);
    }
    else
    {
        file->close();
        delete file;
        file = NULL;
        button->setText("Write");
        emit isIdle(true);
    }
}

void FileWriter::writeFrame()
{
    if( file == NULL) return;

    QString buffer;
    QTextStream ts(&buffer);

    subjectList->read(ts, true);

    file->write(buffer.toUtf8());
    file->write("--END--\n");
}
