/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Emily Carr University

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
#include <QList>
#include <QMutex>
#include <QTimer>
#include <QElapsedTimer>


#include "mocapSubject.h"


class ServerConnection : public QObject
{
	Q_OBJECT
public:
	ServerConnection(QTcpSocket *socket, QObject *parent = 0);
	QString str();
	QString name;
	QTcpSocket *socket;
};

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(MocapSubjectList*, QObject * = 0);

signals:
    void connectionsChanged(void);
    void outMessage(QString);

public slots:
    void newConnection();
    void listen(int port);
    void process();
    void stop();

private:
    //virtual void run();
    QTcpServer *server;
    int  checkAlive();  // returns number of active connections
    bool running;

public:
    size_t count;
    void setInterval(int i);
    QList<ServerConnection*> connections;
    QMutex listMutex;
    void getConnectionList(QList<QString>&);
	int working;
	MocapSubjectList *subjectList;


};

#endif // MYSERVER_H
