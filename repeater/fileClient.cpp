#include "fileClient.h"

#include <QStringList>


FileClient::FileClient(QPushButton *button,
                    QLineEdit *statusLine,
                    QLineEdit *pathLine,
                    QObject *parent)
    :BaseClient(CL_File, button, statusLine, parent)
    ,fps(0)
    ,lineEditPath(pathLine)
    ,file(NULL)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void FileClient::tick()
{
    if(file == NULL) return;
    QStringList lines;
    QByteArray line;
    while(1)
    {
        line = file->readLine();
        if(line.length() == 0)
        {
            file->seek(0);
            line = file->readLine();
            if(line.length() == 0)
            {
                mocapStop();
                return;
            }
            count = 0;
        }
        if( line.length() > 6 && line.left(7) == "--END--" )
            break;
        lines.append(line);
    }

    int subjects = lines[0].toInt();
    int iLine = 1;
    for(int i=0; i < subjects; i++)
    {
        if(lines.length() < iLine)
            return;
        QStringList s1 = lines[iLine++].split("\t");
        if(s1.length() != 3)
            return;
        QString name = s1[0];
        int joints = s1[1].toInt();
        int markers = s1[2].toInt();

        SubjectData * newSubject = new SubjectData(name, CL_File);

        for(int j = 0; j < joints; j++)
        {
            QStringList s2 = lines[iLine++].split("\t");
            if(s2.length() < 8) continue;
            QString jointName = s2[0];
            double trans[3];
            double rot[4];

            trans[0] = s2[1].toFloat();
            trans[1] = s2[2].toFloat();
            trans[2] = s2[3].toFloat();
            rot[0]   = s2[4].toFloat();
            rot[1]   = s2[5].toFloat();
            rot[2]   = s2[6].toFloat();
            rot[3]   = s2[7].toFloat();

            newSubject->setSegment(jointName, trans, rot);
        }

        for(int j = 0; j < markers; j++)
        {
            QStringList s2 = lines[iLine++].split("\t");
            if(s2.length() < 4) continue;
            QString markerName = s2[0];
            double trans[3];

            trans[0] = s2[1].toFloat();
            trans[1] = s2[2].toFloat();
            trans[2] = s2[3].toFloat();

            newSubject->setMarker(markerName, trans);
        }

        emit updateSubject(newSubject);

    }

    emit updateFrame(CL_File, count++);

}

bool FileClient::isConnected()
{
    if(file==NULL) return false;
    return file->isOpen();
}

void FileClient::mocapStart()
{
    UIConnectingState();
    file = new QFile(lineEditPath->text(), this);
    if(!file->open(QIODevice::ReadOnly))
    {
        UIDisconnectedState();
        return;
    }
    QByteArray header = file->readLine();
    QString s(header);
    fps = s.toInt();
    if(fps < 1)
    {
        mocapStop();
        return;
    }
    timer->setInterval(1000 / fps);
    timer->start();
    UIConnectedState();
}

void FileClient::mocapStop()
{
    UIDisconnectingState();
    if(file == NULL) return;
    if(file->isOpen()) file->close();
    delete file;
    file = NULL;
    timer->stop();
    UIDisconnectedState();
}

void FileClient::mocapWait()
{}

void FileClient::UIConnectingState()
{
    emit isIdle(false);
    BaseClient::UIConnectingState();
}

void FileClient::UIDisconnectingState()
{
    emit isIdle(false);
    BaseClient::UIDisconnectingState();
}

void FileClient::UIConnectedState()
{
    emit isIdle(false);
    BaseClient::UIConnectedState();
}

void FileClient::UIDisconnectedState()
{
    emit isIdle(true);
    BaseClient::UIDisconnectedState();
}
