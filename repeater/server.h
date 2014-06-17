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

    ServerConnection( ConnType cType, QObject *parent, void *socket )
        : QObject(parent)
        , type(cType)
    {
        if(type == CON_TCP)
        {
            tcpSocket = static_cast<QTcpSocket*>(socket);
            name = tcpSocket->peerAddress().toString();
        }
        if(type == CON_PIPE)
        {
            localSocket = static_cast<QLocalSocket*>(socket);
            name = localSocket->fullServerName();
        }
    }

    union
    {
        QTcpSocket*   tcpSocket;
        QLocalSocket* localSocket;
    };

    bool  connected()
    {
        switch(type)
        {
        case CON_TCP : return tcpSocket->state() == QAbstractSocket::ConnectedState;
        case CON_PIPE : return localSocket->state() == QLocalSocket::ConnectedState;
        }
        return false;
    }

    int write(QByteArray &data)
    {
        switch(type)
        {
        case CON_TCP : return tcpSocket->write(data);
        case CON_PIPE : return localSocket->write(data);
        }
        return -1;
    }

    void flush()
    {
        switch(type)
        {
        case CON_TCP :  tcpSocket->flush();
        case CON_PIPE : localSocket->flush();
        }
    }


    QString str() { return name; }
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
    void listen();
    void process();
    void stop();

private:
    //virtual void run();
    union
    {
        QTcpServer *server;
        QLocalServer *localServer;
    };
    int  checkAlive();  // returns number of active connections
    bool running;
    int  port;

public:
    ServerConnection::ConnType type;
    size_t count;
    void setInterval(int i);
    QList<ServerConnection*> connections;
    QMutex listMutex;
    void getConnectionList(QList<QString>&);
	int working;
	MocapSubjectList *subjectList;
    int numberOfConnections();


};

#endif // MYSERVER_H
