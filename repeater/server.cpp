/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr Universityy

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
#include <sstream>


MyServer::MyServer(ServerConnection::ConnType t, int inPort, MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, running(true)
, port(inPort)
, type(t)
, count(0)
, working(false)
, subjectList(sList)
{}

void MyServer::listen()
{
    bool ok = true;
    if(type == ServerConnection::CON_TCP)
    {
        server = new QTcpServer(this);
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        ok = server->listen(QHostAddress::Any, port);
        if(ok) emit outMessage(QString("TCP server started, listening on port: %1").arg(port));
    }
    else
    {
        localServer = new QLocalServer(this);
        connect(localServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        ok = localServer->listen("VIVE");
        if(ok) emit outMessage("Local server started");
    }
}

// There is a new connection, server sends newConnection signal to me.
void MyServer::newConnection()
{
    bool change = false;
    while(1)
    {
        ServerConnection *connection = NULL;
        if(type == ServerConnection::CON_PIPE)
        {
            QLocalSocket *socket = localServer->nextPendingConnection();
            if(socket == NULL) break;
            change = true;
            connection = new ServerConnection(type, this, socket );

        }
        else
        {
            QTcpSocket *socket = server->nextPendingConnection();
            if(socket == NULL) break;
            change = true;
            connection = new ServerConnection(type, this, socket );
        }

        listMutex.lock();
        connections.append(connection);
        listMutex.unlock();

        emit outMessage(QString("Connection from: ") + connection->str());
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

    working = true;

    int alive = checkAlive();

    if(alive > 0)
    {
        count++;
        QString buffer;
        QTextStream stream(&buffer);
        // The following operation is threadsafe.

        subjectList->read(stream, true);

        listMutex.lock();

        // for each connection
        for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
        {
            if(!(*i)->connected()) continue;

            QString d = QString("%1\nEND\r\n").arg(buffer);

            int written = (*i)->write(d.toUtf8());

            if(written == -1)
            {
                emit outMessage(QString(" Error writing to %1").arg((*i)->str()));
            }
            else
            {
                (*i)->flush();
            }
        }

        listMutex.unlock();

    }

    working = false;


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
            if(!(*i)->connected())
            {
                emit outMessage(QString("Disconnected: %1").arg((*i)->str()));
                done = false;
                modified = true;
                delete *i;
                connections.erase(i);
                break;
            }
            else
            {
                ret++;
            }
       }
    }
    listMutex.unlock();

    if(modified) 
		emit connectionsChanged();

	return ret;
}

int MyServer::numberOfConnections()
{
    return connections.length();
}
