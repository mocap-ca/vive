
#include "server.h"
#include <QTextStream>
#include <sstream>

ServerConnection::ServerConnection(QTcpSocket *socket, QObject *parent)
: QObject(parent)
, name(socket->peerAddress().toString())
, socket(socket)
{};

QString ServerConnection::str()
{
	return name;
}

MyServer::MyServer(MocapSubjectList *sList, QObject *parent) :
    QObject(parent), 
	working(false)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

	subjectList = sList;
}

void MyServer::start()
{
    if(!server->listen(QHostAddress::Any, 1234))
    {
        emit outMessage("Could not start server.");
    }
    else
    {
        emit outMessage("Server Started.  Listening on port 1234.");
    }


}

// There is a new connection, server sends newConnection signal to me.
void MyServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
	if(socket == NULL) return;

	ServerConnection *con = new ServerConnection(socket, this);

    listMutex.lock();
    connections.append(con);
    listMutex.unlock();

    emit outMessage(QString("Connection from: ") + con->str());
    emit connectionsChanged();
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


// perform the operations for a frame
void MyServer::doFrame()
{
	working = true;
	//if(checkAlive() > 0)
		work();
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

// Do any work needing done for this loop.
void MyServer::work()
{
	QTextStream stream;
	stream << *subjectList;


    listMutex.lock();

	// for each connection
    for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
    {
        QTcpSocket *s = (*i)->socket;
		if(s->state() != QAbstractSocket::ConnectedState) continue;

        QString d = QString("%1\nEND\n").arg(stream.readAll());
        int written = s->write(d.toUtf8());
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

}
