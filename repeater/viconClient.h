#ifndef VICONCLIENT_H
#define VICONCLIENT_H

#include <QObject>
#include <QList>
#include <QThread>
#include "ViconStreamClient.h"
#include "mocapSubject.h"

class ViconClient : public QThread
{
	Q_OBJECT

public:
	ViconClient( MocapSubjectList *subjectList, QObject *parent = NULL);

    bool mocapConnect();
	bool mocapDisconnect();

    bool connected;

	
    ViconDataStreamSDK::CPP::Client mClient;

	MocapSubjectList *subjects;

	virtual void run();

	bool running;
    size_t count;
    QString host;
    int     port;

    enum eStreamMode
    {
        SERVER_PUSH,
        CLIENT_PULL,
        CLIENT_PULL_PRE_FETCH
    };

private:
    bool    frameError;
    eStreamMode   streamMode;

signals:
    void outMessage(QString);
    void connectedEvent(bool);
    void newFrame(unsigned int);

};



#endif
