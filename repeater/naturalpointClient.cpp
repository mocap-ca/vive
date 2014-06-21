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

#include "naturalpointClient.h"
#include <stdio.h>
#include <assert.h>

#include <QMessageBox>
#include <QCompleter>





//--------------------------------

NaturalPointClient::NaturalPointClient(MocapSubjectList *sList,
                                       QPushButton *button,
                                       QLineEdit *statusLine,
                                       QLineEdit *inHostField,
                                       QLineEdit *inPortField,
                                       QObject *parent)
    : BaseClient(CL_NaturalPoint, sList, button, statusLine, parent)
    , running(false)
    , hostField(inHostField)
    , portField(inPortField)
{
    QStringList wordList;
    wordList << "239.255.42.99";
    QCompleter *completer = new QCompleter(wordList, inHostField);
    hostField->setCompleter(completer);
    hostField->setText(wordList[0]);
    portField->setText("1511");

    socket = new QUdpSocket();

    connect(socket, SIGNAL(readyRead()),  this, SLOT(readPendingDatagrams()));

}

bool NaturalPointClient::isConnected()
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

//bool NaturalPointClient::mocapDisconnect()
//{
//    if(!connected)
//    {
//        emit stateDisconnected();
//        return false;
//    }
//
//    outMessage("Disconnecting from NaturalPoint");
//    connected = false;
//    socket->leaveMulticastGroup(connectGroupAddress);
//    socket->close();
//
//    emit stateDisconnected();
//
//    return true;
//}
//

void NaturalPointClient::mocapStart()
{

    QString host = hostField->text();
    int     port = portField->text().toInt();
    if(port == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Port", QMessageBox::Ok);
        return;
    }

    UIConnectingState();

    if( socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress) == false ){
        outMessage("Failed to bind NaturalPoint port");
        UIDisconnectedState();
        return;
    }

    connectGroupAddress = QHostAddress(host);
    if( socket->joinMulticastGroup(QHostAddress(host)) == false ) {
        outMessage("Error joining multicast group:");
        outMessage(socket->errorString());
        delete socket;
        socket = NULL;
        UIDisconnectedState();
        return;
    }

    UIConnectedState();

    outMessage("Listening for NaturalPoint data over multicast.");
    return;

    
}

void NaturalPointClient::mocapStop()
{
    UIDisconnectingState();
    outMessage("Disconnecting from NaturalPoint");
    socket->leaveMulticastGroup(connectGroupAddress);
    socket->close();
    UIDisconnectedState();
}

void NaturalPointClient::mocapWait()
{
    // no thread, so nothing to wait on.
}

void NaturalPointClient::readPendingDatagrams()
{

    SubjectData *subject = NULL;
    char  szData[20000];
    qint64 datagramSize = socket->readDatagram(szData, sizeof(szData));
    sFrameOfMocapData *frameData = mParser.parse(szData, (int)datagramSize );

    if( frameData == NULL ) {
        static bool errorNotice = false;
        if( !errorNotice ) {
            outMessage("Error getting NaturalPoint frame");
            errorNotice = true;
        }
        return;
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
        subject = new SubjectData(QString(subjectName), CL_NaturalPoint);

        // Load the subject's translation and rotation quaternion.
        double t[3] = {rb->x * 10, rb->y * 10 , -rb->z * 10 };
        double r[4] = {-rb->qx, -rb->qy, rb->qz, rb->qw};
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

        emit update(subject);
    }

    emit newFrame(frameData->iFrame);
    count++;
}



