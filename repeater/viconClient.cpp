#include "viconClient.h"


#include <string>
#include <sstream>

using namespace ViconDataStreamSDK::CPP;

ViconClient::ViconClient(MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, subjects(sList)
{
}

bool ViconClient::mocapConnect(std::string host, int port)
{
	std::ostringstream oss;
	oss << host << ":" << port;
	Output_Connect output = mClient.Connect( oss.str() );
	if(output.Result != Result::Success) return false;

	mClient.EnableSegmentData();
	mClient.EnableMarkerData();
	mClient.EnableUnlabeledMarkerData();

	// mClient.SetStreamMode();

	mClient.SetAxisMapping(Direction::Forward, Direction::Up, Direction::Right);

	return true;
}

bool ViconClient::mocapDisconnect()
{
	Output_Disconnect output = mClient.Disconnect();
	return output.Result == Result::Success;
}


void ViconClient::getFrame()
{
	MocapSubject *subject;

	Output_GetFrame rf = mClient.GetFrame();
	if (rf.Result != Result::Success) return;

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

			Output_GetSegmentLocalTranslation trans = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);

			Output_GetSegmentLocalRotationEulerXYZ rot = mClient.GetSegmentLocalRotationEulerXYZ(subjectName, sn.SegmentName);

			subject->segments.set(sn.SegmentName, trans.Translation, rot.Rotation);

		}

	}

}

	