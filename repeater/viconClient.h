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

	bool mocapConnect(std::string host, int port);
	bool mocapDisconnect();

	
    ViconDataStreamSDK::CPP::Client mClient;

	MocapSubjectList *subjects;

public slots:
	void getFrame();
};



#endif
