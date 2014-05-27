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


#include "server.h"
#include "profile.h"

#include <sstream>

ServerConnection::ServerConnection(QTcpSocket *socket, QObject *parent)
: QObject(parent)
, name(socket->peerAddress().toString())
, socket(socket)


{}

QString ServerConnection::str()
{
	return name;
}

MyServer::MyServer(MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, working(false)
, subjectList(sList)
, count(0)
, running(true)
{}

void MyServer::listen(int port)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, port))
    {
        emit outMessage("Could not start server.");
    }
    else
    {
        emit outMessage(QString("Server Started.  Listening on port: %1").arg(port));
    }
}

// There is a new connection, server sends newConnection signal to me.
void MyServer::newConnection()
{
    bool change = false;
    while(1)
    {
        QTcpSocket *socket = server->nextPendingConnection();
        if(socket == NULL) break;

        change = true;

        ServerConnection *con = new ServerConnection(socket, this);

        listMutex.lock();
        connections.append(con);
        listMutex.unlock();

        emit outMessage(QString("Connection from: ") + con->str());
    }

    if(change)
    {
        emit connectionsChanged();
    }
}


// threadsafe grab a list of connections.
void MyServer::getConnectionList(QList<QString>&items)
{
    listMutex.lock();
    for(QList<ServerConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
    {
        items.append((*i)->str());
    }
    listMutex.unlock();
}



void MyServer::stop()
{
    running = false;
}


/* perform the operations for a frame:
 *   - check to see if the connections are still alive (checkAlive)
 *      - this will emit connectionsChanged if there is a any change in the connection status
 *   - grab a text stream of the current model data ( stream << *subjectList )
 *   - put the text stream on the wire s->write(...)
*/
void MyServer::process()
{
    stopProfile("Other");

    working = true;

    startProfile("checkAlive");
    int alive = checkAlive();
    stopProfile("checkAlive");

    if(alive > 0)
    {
        startProfile("Serve");

        count++;
        QString buffer;
        QTextStream stream(&buffer);
        // The following operation is threadsafe.
        startProfile("Fetch");
        stream << *subjectList;
        stopProfile("Fetch");

        startProfile("Wait");
        listMutex.lock();
        stopProfile("Wait");

        // for each connection
        for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
        {
            QTcpSocket *s = (*i)->socket;
            if(s->state() != QAbstractSocket::ConnectedState) continue;

            QString d = QString("%1\nEND\r\n").arg(buffer);

            startProfile("Write");
            int written = s->write(d.toUtf8());
            stopProfile("Write");

            if(written == -1)
            {
                emit outMessage(QString(" Error writing to %1").arg(s->peerAddress().toString()));
            }
            else
            {
                s->flush();
            }
        }

        listMutex.unlock();

        stopProfile("Serve");
    }

    working = false;


    startProfile("Other");


}


// Check to see if all connections are alive, if not remove them from the list
// returns number of active connections.
int MyServer::checkAlive()
{
    listMutex.lock();

    bool done = false;
    bool modified = false;
	int  ret = 0;

    while(!done)
    {
        done = true;

        for(QList<ServerConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
        {
            QTcpSocket *s = (*i)->socket;
            if(s->state() == QAbstractSocket::UnconnectedState)
            {
                emit outMessage(QString("Disconnected: %1").arg((*i)->str()));
                done = false;
                modified = true;
                delete *i;
                connections.erase(i);
                break;
            }

			if(s->state() == QAbstractSocket::ConnectedState)
				ret++;
        }

    }
    listMutex.unlock();

    if(modified) 
		emit connectionsChanged();

	return ret;
}

