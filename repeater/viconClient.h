/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Emily Carr University

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
