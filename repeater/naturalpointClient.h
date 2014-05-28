/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr University

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


#ifndef NATURALPOINTCLIENT_H
#define NATURALPOINTCLIENT_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QUdpSocket>
#include "mocapSubject.h"

#include "NatNetTypes.h"
#include "CMNatNetPacketParser.h"

class NaturalPointClient : public QThread
{
    Q_OBJECT

public:
    NaturalPointClient( MocapSubjectList *subjectList, QObject *parent = NULL);

    bool mocapConnect();
    bool mocapDisconnect();

    bool connected;
    QHostAddress connectGroupAddress;
    QUdpSocket *socket;

    MocapSubjectList *subjects;
    CMNatNetPacketParser mParser;

    virtual void run();

    bool running;
    size_t count;
    QString host;
    int     port;

private:

signals:
    void outMessage(QString);
    void connectedEvent(bool);
    void newFrame(unsigned int);
};

#endif // NATURALPOINTCLIENT_H
