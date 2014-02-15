
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
    QThread(parent),
    working(false),
    subjectList(sList)
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


void MyServer::run()
{
    // create a timer to call runOne ten times a second
    loopTimer = new QTimer();
    loopTimer->setInterval(20);
    connect(loopTimer, SIGNAL(timeout()), this, SLOT(runOne()));
    //loopTimer.moveToThread(this);
    loopTimer->start();
    exec();
}

void MyServer::stop()
{
    loopTimer->stop();
    delete loopTimer;
    exit();
}


/* perform the operations for a frame:
 *   - check to see if the connections are still alive (checkAlive)
 *      - this will emit connectionsChanged if there is a any change in the connection status
 *   - grab a text stream of the current model data ( stream << *subjectList )
 *   - put the text stream on the wire s->write(...)
*/
void MyServer::runOne()
{
	working = true;
    if(checkAlive() > 0)
    {
        QString buffer;
        QTextStream stream(&buffer);
        stream << *subjectList;

        listMutex.lock();

        // for each connection
        for(QList<ServerConnection *>::iterator i =  connections.begin(); i != connections.end(); i++)
        {
            QTcpSocket *s = (*i)->socket;
            if(s->state() != QAbstractSocket::ConnectedState) continue;

            QString d = QString("%1\nEND\r\n").arg(buffer);
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

