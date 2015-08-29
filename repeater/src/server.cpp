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


ServerConnection::ServerConnection( ConnType cType, QObject *parent, void *socket )
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


bool ServerConnection::connected()
{
    switch(type)
    {
    case CON_TCP :  return tcpSocket->state()   == QAbstractSocket::ConnectedState;
    case CON_PIPE : return localSocket->state() == QLocalSocket::ConnectedState;
    }
    return false;
}

QByteArray ServerConnection::read(qint64 maxlen)
{
    switch(type)
    {
    case CON_TCP:  return tcpSocket->read(maxlen);
    case CON_PIPE: return localSocket->read(maxlen);
    }
    return QByteArray();
}

int ServerConnection::write(QByteArray &data)
{
    switch(type)
    {
    case CON_TCP :  return tcpSocket->write(data);
    case CON_PIPE : return localSocket->write(data);
    }
    return -1;
}

void ServerConnection::flush()
{
    switch(type)
    {
        case CON_TCP :  tcpSocket->flush(); break;
        //case CON_PIPE : localSocket->flush();
        default: break;
    }
}

MyServer::MyServer(ServerConnection::ConnType t, int inPort, MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, port(inPort)
, type(t)
, count(0)
, working(false)
, subjectList(sList)
{}

bool MyServer::listen()
{
    bool ok = true;
    if(type == ServerConnection::CON_TCP)
    {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        ok = tcpServer->listen(QHostAddress::Any, port);
        if(ok)
            emit outMessage(QString("TCP server started, listening on port: %1").arg(port));
        else
            emit outMessage(QString("Unable to start TCP Server: %1").arg(tcpServer->errorString()));
    }
    if(type == ServerConnection::CON_PIPE)
    {
        localServer = new QLocalServer(this);
        connect(localServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        ok = localServer->listen("VIVE");
        if(ok)
            emit outMessage("Local server started");
        else
            emit outMessage(QString("Unable to start Local Server: %1").arg(localServer->errorString()));
    }
    return ok;
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
        if(type == ServerConnection::CON_TCP)
        {
            QTcpSocket *socket = tcpServer->nextPendingConnection();
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
    switch(type)
    {
    case ServerConnection::CON_PIPE : localServer->close(); break;
    case ServerConnection::CON_TCP:   tcpServer->close();   break;
    }
}

bool MyServer::isRunning()
{
    switch(type)
    {
    case ServerConnection::CON_PIPE : return localServer->isListening();
    case ServerConnection::CON_TCP:   return tcpServer->isListening();
    }
    return false;
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
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);
        // The following operation is threadsafe.

        subjectList->data.formatHeader(stream, true);

        IDList* ids = IDList::Instance();

        int16_t len = ids->items.length();
        stream <= len;

        for( IDListType::iterator i = ids->items.begin(); i!= ids->items.end(); i++)
        {
            (*i).second->frame(stream);
        }


        // for each connection
        for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
        {
            // Skip disconnected
            if(!(*i)->connected()) continue;

            int written = (*i)->write(buffer);
            if(written == -1)
            {
                emit outMessage(QString(" Error writing header to %1").arg((*i)->str()));
            }
            else
            {
                (*i)->flush();
            }


#if 0
            // this code will be for reciving data from another repeater
            QByteArray lastPacket;
            QByteArray inData = (*i)->read(1024 * 12);
            backBuffer.append(inData);

            // Find last terminator
            int term1 = backBuffer.lastIndexOf("\nEND\r\n");

            if(term1 > 0)
            {

                // Find second last terminator
                int term2 = backBuffer.lastIndexOf("\nEND\r\n", term1);
                if(term2 == -1) term2 = 0;

                if(term2 > 0)
                {
                    lastPacket = backBuffer.mid(term2, term1 - term2);
                }
            }
#endif
        }

        listMutex.unlock();
    }

    working = false;
}


// Check to see if all connections are alive, if not remove them from the list
// returns number of active connections.
int MyServer::checkAlive()
{
    bool done     = false;
    bool modified = false;
    int  ret      = 0;

    listMutex.lock();

    while(!done)
    {
        done = true;

        for(QList<ServerConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
        {
            if(!(*i)->connected())
            {
                emit outMessage(QString("Disconnected: %1").arg((*i)->str()));
                done     = false;
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

    if(modified) emit connectionsChanged();

	return ret;
}


