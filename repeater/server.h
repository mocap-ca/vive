#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMutex>

#include "viconClient.h"


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
    explicit MyServer(QObject *parent = 0);

signals:
    void connectionsChanged(void);
    void outMessage(QString);

public slots:
    void newConnection();
    void start();
	void doFrame();

private:
    QTcpServer *server;
    int  checkAlive();  // returns number of active connections
	void work();


public:
    QList<ServerConnection*> connections;
	ViconClient viconClient;
    QMutex listMutex;
    void getConnectionList(QList<QString>&);
	int working;


};

#endif // MYSERVER_H
