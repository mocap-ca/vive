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

#include <QNetworkInterface>



//--------------------------------

NaturalPointClient::NaturalPointClient(MocapSubjectList *sList,
                                       QPushButton *button,
                                       QLineEdit *statusLine,
                                       QComboBox *_localAddrCombo,
                                       QLineEdit *_remoteAddrField,
                                       QLineEdit *_commandPortField,
                                       QLineEdit *_dataPortField,
                                       QObject *parent)
    : BaseClient(CL_NaturalPoint, button, statusLine, parent)
    , running(false)
    , localAddrCombo( _localAddrCombo )
    , remoteAddrField( _remoteAddrField )
    , commandPortField( _commandPortField )
    , dataPortField( _dataPortField  )
    , mConnected(false)
{
    QStringList wordList;
    //wordList << "239.255.42.99";
    wordList << "192.168.1.20";
    QCompleter *completer = new QCompleter(wordList, remoteAddrField);
    remoteAddrField->setCompleter(completer);
    remoteAddrField->setText(wordList[0]);

    QList<QNetworkInterface> interfaces( QNetworkInterface::allInterfaces());
    for ( QList<QNetworkInterface>::iterator i = interfaces.begin(); i != interfaces.end(); i++)
    {
        if( ! ((*i).flags() & QNetworkInterface::IsUp) ) continue;
        if( ! (*i).isValid()) continue;
        QList<QNetworkAddressEntry> addresses( (*i).addressEntries() );
        for( QList<QNetworkAddressEntry>::iterator j = addresses.begin(); j != addresses.end(); j++)
        {
            if( (*j).ip().protocol() != QAbstractSocket::IPv4Protocol) continue;
            localAddrCombo->addItem((*j).ip().toString());
        }
    }

    dataPortField->setText("1511");
    commandPortField->setText("1510");

    client = new NatNetClient();

}



bool NaturalPointClient::isConnected()
{
    return mConnected;
}


void NaturalPointClient::mocapStart()
{

    QString   localHost   = localAddrCombo->currentText();
    QString   remoteHost  = remoteAddrField->text();
    int       commandPort = commandPortField->text().toInt();
    int       dataPort    = dataPortField->text().toInt();

    if(commandPort == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Command Port", QMessageBox::Ok);
        return;
    }

    if(dataPort == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid data Port", QMessageBox::Ok);
        return;
    }

    UIConnectingState();

    QByteArray slocal = localHost.toUtf8();
    QByteArray sremote = remoteHost.toUtf8();

    char local[1024];
    char remote[1024];
    strcpy( local, slocal.data() );
    strcpy( remote, sremote.data() );

    int ret = client->Initialize(local , remote, commandPort, dataPort);

    mConnected = (ret == 0);

    if(mConnected)
    {
        client->SetDataCallback( &::dataCallback, (void*)this );
        UIConnectedState();
        outMessage("Connected");
    }
    else
    {
        UIDisconnectedState();
        outMessage("Could not connect");
    }

    return;
}

void NaturalPointClient::mocapStop()
{
    UIDisconnectingState();
    outMessage("Disconnecting from NaturalPoint");
    client->Uninitialize();
    mConnected = false;
    UIDisconnectedState();
}

void NaturalPointClient::mocapWait()
{
    // no thread, so nothing to wait on.
}

void dataCallback( sFrameOfMocapData* data, void *ptr  )
{
    NaturalPointClient *m = (NaturalPointClient*)ptr;
    m->dataCallback( data );
}

void NaturalPointClient::dataCallback( sFrameOfMocapData *frameData )
{

    SubjectData *subject;

    if( frameData == NULL )
    {
        static bool errorNotice = false;
        if( !errorNotice )
        {
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
        double t[3] = { rb->x ,  rb->y,  -rb->z };
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

        emitUpdateSubject(subject);
    }

    emitNewFrame();
}



