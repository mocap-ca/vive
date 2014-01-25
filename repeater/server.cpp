
#include "server.h"

ServerConnection::ServerConnection(QTcpSocket *socket, QObject *parent)
: QObject(parent)
, name(socket->peerAddress().toString())
, socket(socket)
{};

QString ServerConnection::str()
{
	return name;
}

MyServer::MyServer(QObject *parent) :
    QObject(parent), 
	working(false)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
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

	if(!viconClient.mocapConnect("localhost", 8811))
	{
		emit outMessage("Could not connect to vicon");
	}
	else
	{
		emit outMessage("Connected to vicon");
	}
}

void MyServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

	ServerConnection *con = new ServerConnection(socket, this);

    listMutex.lock();
    connections.append(con);
    listMutex.unlock();

    emit outMessage(QString("Connection from: ") + con->str());
    emit connectionsChanged();
}

 void MyServer::getConnectionList(QList<QString>&items)
{
    listMutex.lock();
    for(QList<ServerConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
    {
        items.append((*i)->str());
    }
    listMutex.unlock();
}



 void MyServer::doFrame()
 {
	 working = true;
	 if(checkAlive() > 0)
		 work();
	 working = false;
 }

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

void MyServer::work()
{

    listMutex.lock();

    for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
    {
        QTcpSocket *s = (*i)->socket;
		if(s->state() != QAbstractSocket::ConnectedState) continue;

        float r = (float)qrand() / RAND_MAX;

        QString d = QString("%1\nEND\n").arg(r);
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
