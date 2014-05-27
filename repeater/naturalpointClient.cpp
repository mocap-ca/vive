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



#include <stdio.h>
#include <assert.h>
#include "naturalpointClient.h"

NaturalPointClient::NaturalPointClient(MocapSubjectList *sList, QObject *parent)
    : QThread(parent)
    , connected(false)
    , subjects(sList)
    , running(false)
    , count(0)
    , host("239.255.42.99")
    , port(1511)

{
}

// This function must emit a connected event before returning, to renable the button
// from ViaconClient: ViconDataStreamSDK::CPP::Client mClient;

bool NaturalPointClient::mocapConnect()
{
    socket = new QUdpSocket();
    if( socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress) == false ){
        outMessage("Failed to bind NaturalPoint port");
        return false;
    }

    connectGroupAddress = QHostAddress(host);
//    outMessage("Connecting to group:");
//    outMessage(connectGroupAddress->toString());
    if( socket->joinMulticastGroup(QHostAddress(host)) ) {
        outMessage("Listening for NaturalPoint data over multicast.");
        connected = true;
        emit connectedEvent(true);
        return true;
    } else {
        outMessage("Error joining multicast group:");
        outMessage(socket->errorString());
//        delete connectGroupAddress;
//        connectGroupAddress = NULL;
        return false;
    }
}

bool NaturalPointClient::mocapDisconnect()
{
    if(!connected)
    {
        emit connectedEvent(false);
        return false;
    }

    outMessage("Disconnecting from NaturalPoint");
    connected = false;
    socket->leaveMulticastGroup(connectGroupAddress);
    socket->close();

    emit connectedEvent(false);

    return true;
}


void NaturalPointClient::run()
{
    MocapSubject *subject = NULL;
    running = true;

    if(!mocapConnect())
    {
        running = false;
        return;
    }

    while(running)
    {

        if(!connected)
        {
            this->msleep(100);
            continue;
        }

        if( socket->hasPendingDatagrams() == false) {
            continue;
        }

        char  szData[20000];
        qint64 datagramSize = socket->readDatagram(szData, sizeof(szData));
        sFrameOfMocapData *frameData = mParser.parse(szData, (int)datagramSize );
        
        if( frameData == NULL ) {
            static bool errorNotice = false;
            if( !errorNotice ) {
                outMessage("Error getting NaturalPoint frame");
                errorNotice = true;
            }
            continue;
        }

        unsigned int subjectCount = frameData->nRigidBodies;
        for(unsigned int iSubject=0; iSubject < subjectCount; iSubject++)
        {
            sRigidBodyData *rb = &frameData->RigidBodies[iSubject];

            // ASSUMPTION:  rb => frameData->MocapData[rb->ID-1].szName
            int markerSetID = rb->ID-1;
            assert(markerSetID>=0 && markerSetID<frameData->nMarkerSets);
            sMarkerSetData *markerSet = &frameData->MocapData[rb->ID-1];

            // Find/Create the subject by name.
            char* subjectName = markerSet->szName;
            subject = subjects->find(QString(subjectName));

            // Load the subject's translation and rotation quaternion.
            double t[3] = {rb->x, rb->y, rb->z};
            double r[4] = {rb->qx, rb->qy, rb->qz, rb->qw};
            subject->setSegment(QString("root"), t, r);

            for(int iMarker=0; iMarker < rb->nMarkers; iMarker++)
            {
                MarkerData *m = &rb->Markers[iMarker];
                double markerPos[3] = {*m[0], *m[1], *m[2]};
                if( rb->MarkerIDs ) {
                    subject->setMarker(QString::number(rb->MarkerIDs[iMarker]), markerPos);
                } else {
                    subject->setMarker(QString::number(iMarker), markerPos);
                }
            }
        }

        emit newFrame(frameData->iFrame);
        count++;
    }

    outMessage("NaturalPoint Service has finished");

    mocapDisconnect();
}
