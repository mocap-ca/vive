#ifndef VICONCLIENT_H
#define VICONCLIENT_H

#include <QObject>
#include <QList>
#include "ViconStreamClient.h"
#include "mocapSubject.h"

class ViconClient : public QObject
{
	Q_OBJECT

public:
	ViconClient( MocapSubjectList *subjectList, QObject *parent = NULL);

    bool mocapConnect(QString, int port);
	bool mocapDisconnect();

    bool connected;

	
    ViconDataStreamSDK::CPP::Client mClient;

	MocapSubjectList *subjects;

signals:
    void outMessage(QString);

public slots:
	void getFrame();
};



#endif
