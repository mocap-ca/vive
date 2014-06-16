/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macoeod, Emily Carr University

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

#include "viconClient.h"
#include <string>
#include <sstream>
#include <QMessageBox>
#include <QCompleter>

using namespace ViconDataStreamSDK::CPP;

ViconConnector::ViconConnector(QObject *parent)
    : BaseConnector(parent)
    , running(false)
    , host("")
    , port(0)
    , streamMode(SERVER_PUSH)
{}

void ViconConnector::run()
{
    emit connecting();
    if(!connect())
    {
        emit disconnected();
        return;
    }
    emit connected();

    SubjectData *subject;

    running = true;

    bool frameError = false;

    while(running)
    {
        Output_GetFrame rf = mClient.GetFrame();
        if(rf.Result == Result::NoFrame) continue;
        if(rf.Result != Result::Success)
        {
            // Only show this error once, otherwise it will fill up the log
            if(!frameError) outMessage("Error getting frame");
            frameError =true;
            continue;
        }

        frameError = false;

        Output_GetFrameNumber rfn = mClient.GetFrameNumber();
        unsigned int frameNumber = 0;
        unsigned int subjectCount = 0;

        if (rfn.Result == Result::Success)
            frameNumber = rfn.FrameNumber;

        Output_GetSubjectCount rsc = mClient.GetSubjectCount();
        if (rsc.Result == Result::Success)
            subjectCount = rsc.SubjectCount;

        // For each subject
        for(unsigned int i=0; i < subjectCount; i++)
        {
            Output_GetSubjectName rsn = mClient.GetSubjectName(i);
            if(rsn.Result != Result::Success) continue;

            std::string subjectName = rsn.SubjectName;

            subject = new SubjectData(subjectName.c_str(), CL_Vicon);

            Output_GetSubjectRootSegmentName srs = mClient.GetSubjectRootSegmentName(subjectName);
            if(srs.Result != Result::Success) continue;

            Output_GetSegmentCount sc = mClient.GetSegmentCount(subjectName);
            if(sc.Result  != Result::Success) continue;

            for(unsigned int i = 0; i < sc.SegmentCount; i++)
            {
                Output_GetSegmentName sn = mClient.GetSegmentName(subjectName, i);
                Output_GetSegmentLocalTranslation         trans     = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);
                Output_GetSegmentLocalRotationQuaternion  localRot  = mClient.GetSegmentLocalRotationQuaternion(subjectName, sn.SegmentName);

                // Convert to unity coordinate system
                // TODO: convert to opengl instead
                double unityTrans[3] = { -trans.Translation[0] / 100. , -trans.Translation[2] / 100., trans.Translation[1] / 100. };
                double unityRot[4] = { localRot.Rotation[0], localRot.Rotation[2], -localRot.Rotation[1],  localRot.Rotation[3] };

                std::string segname = sn.SegmentName;
                subject->setSegment(QString(segname.c_str()) ,unityTrans, unityRot);
            }

            Output_GetMarkerCount mc = mClient.GetMarkerCount(subjectName);
            for(unsigned int i=0; i < mc.MarkerCount; i++)
            {
                Output_GetMarkerName mn = mClient.GetMarkerName(subjectName, i);
                Output_GetMarkerGlobalTranslation trans = mClient.GetMarkerGlobalTranslation(subjectName, mn.MarkerName);
                std::string markername = mn.MarkerName;
                double unityTrans[3] = { -trans.Translation[0] / 100. , -trans.Translation[2] / 100., -trans.Translation[1] / 100. };
                subject->setMarker(QString(markername.c_str()), unityTrans);
            }
            emit updateSubject(subject);
        }

        emit newFrame(frameNumber);
        count++;
    }

    emit outMessage("Disconnecting from Vicon");
    mClient.Disconnect();

    emit disconnected();
}




void ViconConnector::stop()
{
    emit disconnecting();
    running = false;
}

// This function must emit a connected event before returning, to renable the button
bool ViconConnector::connect()
{
    QString connectionString;
    QTextStream stream(&connectionString);
    stream << host << ":" << port;
    emit outMessage(QString("Connecting to: %1").arg(connectionString));

    Output_Connect output = mClient.Connect( connectionString.toUtf8().data() );
    if(output.Result != Result::Success)
    {
        switch(output.Result)
        {
            case Result::InvalidHostName :        emit outMessage("Error: Invalid host name"); break;
            case Result::ClientAlreadyConnected : emit outMessage("Error: Client Already Connected"); break;
            case Result::ClientConnectionFailed : emit outMessage("Error: Connection Failed"); break;
            default: emit outMessage("Error: Could not connect");
        }
        return false;
    }

    mClient.EnableSegmentData();
    mClient.EnableMarkerData();
    //mClient.EnableUnlabeledMarkerData();

    switch(streamMode)
    {
        case SERVER_PUSH : mClient.SetStreamMode(StreamMode::ServerPush); break;
        case CLIENT_PULL : mClient.SetStreamMode(StreamMode::ClientPull); break;
        case CLIENT_PULL_PRE_FETCH : mClient.SetStreamMode(StreamMode::ClientPullPreFetch); break;
    }

    Output_SetAxisMapping axisResult = mClient.SetAxisMapping(Direction::Forward, Direction::Up, Direction::Right);
    if(axisResult.Result != Result::Success)
    {
        emit outMessage("Could not set Axis");
    }

    emit outMessage("Connected to vicon server.");

    return true;
}


ViconClient::ViconClient(MocapSubjectList *sList,
                         QPushButton *button,
                         QLineEdit *statusLine,
                         QLineEdit *inHostField,
                         QLineEdit *inPortField,
                         QObject *parent)
: BaseClient(CL_Vicon, sList, button, statusLine, parent)
, running(false)
, frameError(false)
, hostField(inHostField)
, portField(inPortField)
{
    vicon = new ViconConnector(this);
    linkConnector(vicon);

    QStringList wordList;
    wordList << "192.168.11.1" << "127.0.0.1";
    QCompleter *completer = new QCompleter(wordList, inHostField);
    hostField->setCompleter(completer);
    hostField->setText(wordList[0]);
    portField->setText("801");

}

bool ViconClient::isConnected()
{
    return vicon->running;
}

void ViconClient::mocapStop()
{
    vicon->stop();
}

void ViconClient::mocapStart()
{
    if(vicon->running)
    {
        emit outMessage("Skipping attempt to start already running vicon server... this is probably a bug");
        return;
    }
    vicon->host = hostField->text();
    vicon->port = portField->text().toInt();

    if(vicon->port == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Port", QMessageBox::Ok);
        return;
    }

    // start (connection is handled on other thread as it can be slow)
    vicon->start();
}

void ViconClient::mocapWait()
{
    vicon->wait();
}

