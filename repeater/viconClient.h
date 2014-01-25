#ifndef VICONCLIENT_H
#define VICONCLIENT_H

#include <QObject>
#include <QList>
#include "Client.h"
#include "mocapSubject.h"

class ViconClient : public QObject
{
	Q_OBJECT

public:
	ViconClient(QObject *parent = NULL);

	bool mocapConnect(std::string host, int port);
	bool mocapDisconnect();

	bool getFrame();
	
	template<typename T> T& operator << (T&);

	ViconDataStreamSDK::CPP::Client mClient;

	MocapSubjectList subjects;

};



#endif