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

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QThread>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLocalServer>
#include <QLocalSocket>
#include <QUdpSocket>
#include <QList>
#include <QMutex>
#include <QTimer>
#include <QElapsedTimer>


#include "mocapSubject.h"

class ServerConnection : public QObject
{
    Q_OBJECT
public:
   typedef enum { CON_TCP, CON_PIPE } ConnType;

    ServerConnection( ConnType cType, QObject *parent, void *socket );

    bool       connected();
    QByteArray read(qint64 maxlen);
    int        write(QByteArray &data);
    void       flush();
    QString    str() { return name; }

    union
    {
        QTcpSocket*   tcpSocket;
        QLocalSocket* localSocket;
    };

    QString name;
    ConnType type;

};


class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(ServerConnection::ConnType, int port, MocapSubjectList*, QObject * = 0);

signals:
    void connectionsChanged(void);
    void outMessage(QString);


public slots:
    void newConnection();

    void process();
    void stop();
    bool isRunning();

private:
    //virtual void run();
    union
    {
        QTcpServer   *tcpServer;
        QLocalServer *localServer;
    };

    int  checkAlive();  // returns number of active connections

    int     port;
    QString udpHost;


public:
    ServerConnection::ConnType type;
    size_t                     count;
    QList<ServerConnection*>   connections;
    QMutex                     listMutex;
    int                        working;
    MocapSubjectList*          subjectList;
    QByteArray                 backBuffer;


    bool listen();
    void setPort(int p)        { port = p; }
    int  numberOfConnections() { return connections.length(); }
    void setInterval(int i);
    void getConnectionList(QList<QString>&);

};

#endif // MYSERVER_H
