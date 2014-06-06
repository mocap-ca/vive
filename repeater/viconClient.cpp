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

ViconClient::ViconClient(MocapSubjectList *sList,
                         QPushButton *button,
                         QLineEdit *statusLine,
                         QLineEdit *inHostField,
                         QLineEdit *inPortField,
                         QObject *parent)
: BaseClient(BaseClient::CL_Vicon, sList, button, statusLine, parent)
, running(false)
, port(0)
, frameError(false)
, streamMode(SERVER_PUSH)
, hostField(inHostField)
, portField(inPortField)
{
    QStringList wordList;
    wordList << "192.168.11.1" << "127.0.0.1";
    QCompleter *completer = new QCompleter(wordList, inHostField);
    hostField->setCompleter(completer);
    hostField->setText(wordList[0]);
    portField->setText("801");

}

// This function must emit a connected event before returning, to renable the button
bool ViconClient::mocapConnect()
{
    emit stateConnecting();

    QString connectionString;
    QTextStream stream(&connectionString);
    stream << host << ":" << port;
    outMessage(QString("Connecting to: %1").arg(connectionString));

    Output_Connect output = mClient.Connect( connectionString.toUtf8().data() );
    if(output.Result != Result::Success)
    {
        switch(output.Result)
        {
            case Result::InvalidHostName : outMessage("Error: Invalid host name"); break;
            case Result::ClientAlreadyConnected : outMessage("Error: Client Already Connected"); break;
            case Result::ClientConnectionFailed : outMessage("Error: Connection Failed"); break;
            default: outMessage("Error: Could not connect");
        }
        emit stateDisconnected();
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
        outMessage("Could not set Axis");
    }

    connected = true;

    outMessage("Connected to vicon server.");

    emit stateConnected();

	return true;
}

bool ViconClient::mocapDisconnect()
{

    if(!connected)
    {
        emit stateDisconnected();
        return false;
    }

    emit stateDisconnecting();

    outMessage("Disconnecting from Vicon");
	Output_Disconnect output = mClient.Disconnect();
    connected = false;

    emit stateDisconnected();

	return output.Result == Result::Success;
}


void ViconClient::run()
{
	MocapSubject *subject;

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

		for(unsigned int i=0; i < subjectCount; i++)
		{
			Output_GetSubjectName rsn = mClient.GetSubjectName(i);
			if(rsn.Result != Result::Success) continue;

			std::string subjectName = rsn.SubjectName;
			subject = subjects->find(QString(subjectName.c_str()));

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
		}

        emit newFrame(frameNumber);
        count++;
	}

    outMessage("Service finished");

    mocapDisconnect();
}


void ViconClient::mocapStart()
{
    host = hostField->text();
    port = portField->text().toInt();
    if(port == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Port", QMessageBox::Ok);
        return;
    }

    // start (connection is handled on other thread as it can be slow)
    this->start();
}

void ViconClient::UIConnectingState()
{
    QString host_ = hostField->text();
    int port_ = portField->text().toInt();
    if(port == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Port", QMessageBox::Ok);
        return;
    }

    // start (connection is handled on other thread as it can be slow)
    this->host = host_;
    this->port = port_;

    BaseClient::UIConnectingState();
}


void ViconClient::mocapStop()
{
    running = false;
}



	
