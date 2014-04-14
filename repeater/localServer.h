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
