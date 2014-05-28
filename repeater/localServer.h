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

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QString>

#include "mocapSubject.h"

class LocalConnection : public QObject
{
    Q_OBJECT
public:
    LocalConnection(QLocalSocket  *socket, QObject *parent = 0)
      : QObject(parent)
      , name(socket->fullServerName())
      , socket(socket) {}
    QString str() { return name; }
    QString name;
    QLocalSocket  *socket;
};

class LocalServer : public QObject
{
    Q_OBJECT
public:
    explicit LocalServer(MocapSubjectList*, QObject *parent = 0);

signals:
    void connectionsChanged(void);
    void outMessage(QString);

public slots:
    void newConnection();
    void listen();
    void process();
    void stop();

private:
    QLocalServer *server;
    int  checkAlive();
    bool running;

public:
    size_t count;
    QList<LocalConnection*> connections;
    QMutex listMutex;
    void getConnectionList(QList<QString>&);
    int working;
    MocapSubjectList *subjectList;


};

#endif // LOCALSERVER_H
