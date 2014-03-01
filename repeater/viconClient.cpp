#include "viconClient.h"


#include <string>
#include <sstream>

using namespace ViconDataStreamSDK::CPP;

ViconClient::ViconClient(MocapSubjectList *sList, QObject *parent)
: QThread(parent)
, subjects(sList)
, connected(false)
, running(false)
, count(0)
, port(0)
{
}


// This function must emit a connected event before returning, to renable the button
bool ViconClient::mocapConnect()
{
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
        emit connectedEvent(false);
        return false;
    }

	mClient.EnableSegmentData();
	mClient.EnableMarkerData();
	mClient.EnableUnlabeledMarkerData();

	// mClient.SetStreamMode();

    Output_SetAxisMapping axisResult = mClient.SetAxisMapping(Direction::Forward, Direction::Up, Direction::Right);
    if(axisResult.Result != Result::Success)
    {
        outMessage("Could not set Axis");
    }

    connected = true;

    outMessage("Connected to vicon server.");

    emit connectedEvent(true);


	return true;
}

bool ViconClient::mocapDisconnect()
{
    if(!connected)
    {
        emit connectedEvent(false);
        return false;
    }

    outMessage("Disconnecting from Vicon");
	Output_Disconnect output = mClient.Disconnect();
    connected = false;

    emit connectedEvent(false);

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
            outMessage("Error getting frame");
            continue;
        }

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

                //Output_GetSegmentLocalTranslation trans = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);

                //Output_GetSegmentLocalRotationEulerXYZ rot = mClient.GetSegmentLocalRotationEulerXYZ(subjectName, sn.SegmentName);


                Output_GetSegmentLocalTranslation         trans     = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);
                Output_GetSegmentStaticRotationQuaternion staticRot = mClient.GetSegmentStaticRotationQuaternion(subjectName, sn.SegmentName);
                Output_GetSegmentLocalRotationQuaternion  localRot  = mClient.GetSegmentLocalRotationQuaternion(subjectName, sn.SegmentName);
                Output_GetSegmentGlobalRotationQuaternion globalRot = mClient.GetSegmentGlobalRotationQuaternion(subjectName, sn.SegmentName);

                std::string segname = sn.SegmentName;
                subject->set(QString(segname.c_str()), trans.Translation, staticRot.Rotation, localRot.Rotation, globalRot.Rotation);

			}

		}

        emit newFrame(frameNumber);
        count++;
	}

    outMessage("Vicon Service has finished");

    mocapDisconnect();
}

	
