#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QTime>
#include "server.h"

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = 0, MyServer* server = NULL);

private:
    void run();

    MyServer *myserver;
    bool     isWorking;
    QTime    timer;

signals:
    void connectionsChanged();
    void outMessage(QString);
    void outFrameRate(int);
	void signalFrame();

public slots:

    void stopWorking();
    void measureLatency(QTime*);

};

#endif // WORKTHREAD_H
