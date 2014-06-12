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

#include "server.h"
#include "profile.h"
#include "localServer.h"
#include <sstream>

LocalServer::LocalServer(MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, working(false)
, subjectList(sList)
, count(0)
, running(true)
{}

void LocalServer::listen()
{
    server = new QLocalServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen("VIVE"))
    {
        emit outMessage("Could not start local server.");
    }
    else
    {
        emit outMessage(QString("Local Server Started."));
    }
}

// There is a new connection, server sends newConnection signal to me.
void LocalServer::newConnection()
{
    bool change = false;
    while(1)
    {
        QLocalSocket *socket = server->nextPendingConnection();
        if(socket == NULL) break;

        change = true;

        LocalConnection *con = new LocalConnection(socket, this);

        listMutex.lock();
        connections.append(con);
        listMutex.unlock();

        emit outMessage(QString("Local connection from: ") + con->str());
    }

    if(change)
    {
        emit connectionsChanged();
    }
}


// threadsafe grab a list of connections.
void LocalServer::getConnectionList(QList<QString>&items)
{
    listMutex.lock();
    for(QList<LocalConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
    {
        items.append((*i)->str());
    }
    listMutex.unlock();
}


void LocalServer::stop()
{
    running = false;
}


/* perform the operations for a frame:
 *   - check to see if the connections are still alive (checkAlive)
 *      - this will emit connectionsChanged if there is a any change in the connection status
 *   - grab a text stream of the current model data ( stream << *subjectList )
 *   - put the text stream on the wire s->write(...)
*/
void LocalServer::process()
{
    working = true;

    int alive = checkAlive();

    char xxx[1];
    xxx[0] =0;


    if(alive > 0)
    {
        count++;
        QString buffer;
        QTextStream stream(&buffer);
        subjectList->read(stream, true);  // this is threadsafe.

        listMutex.lock();

        // for each connection
        for(QList<LocalConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
        {
            QLocalSocket *s = (*i)->socket;
            if(s->state() != QLocalSocket::ConnectedState) continue;

            QString d = QString("%1\nEND\r\n").arg(buffer);

            int written = s->write(d.toUtf8());
            s->write(xxx, 0);

            if(written == -1)
            {
                emit outMessage(QString(" Error writing to %1").arg((*i)->str()));
            }
            else
            {
                s->flush();
            }
        }

        listMutex.unlock();

    }

    working = false;




}


// Check to see if all connections are alive, if not remove them from the list
// returns number of active connections.
int LocalServer::checkAlive()
{
    listMutex.lock();

    bool done = false;
    bool modified = false;
    int  ret = 0;

    while(!done)
    {
        done = true;

        for(QList<LocalConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
        {
            QLocalSocket *s = (*i)->socket;
            if(s->state() == QLocalSocket::UnconnectedState)
            {
                emit outMessage(QString("Local Disconnect: %1").arg((*i)->str()));
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

    if(modified) emit connectionsChanged();

    return ret;
}

