#include "filewriter.h"
#include <QDataStream>
#include <QByteArray>


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

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);

    subjectList->data.formatHeader(stream, true);
    file->write(buffer);

    IDList* ids = IDList::Instance();

    int16_t len = ids->items.length();
    stream <= len;


    for( IDListType::iterator i = ids->items.begin(); i!= ids->items.end(); i++)
    {
        (*i).second->frame(stream);
    }
    file->write(buffer);

}
