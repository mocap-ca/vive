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
    QTimer *loopTimer;
    size_t interval;
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
