#include <stdio.h>
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
        mParser.parse(szData, (int)datagramSize );

        /*
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

                //Output_GetSegmentLocalTranslation trans = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);

                //Output_GetSegmentLocalRotationEulerXYZ rot = mClient.GetSegmentLocalRotationEulerXYZ(subjectName, sn.SegmentName);


                Output_GetSegmentLocalTranslation         trans     = mClient.GetSegmentLocalTranslation(subjectName, sn.SegmentName);
                Output_GetSegmentLocalRotationQuaternion  localRot  = mClient.GetSegmentLocalRotationQuaternion(subjectName, sn.SegmentName);
                //Output_GetSegmentStaticRotationQuaternion staticRot = mClient.GetSegmentStaticRotationQuaternion(subjectName, sn.SegmentName);
                //Output_GetSegmentGlobalRotationQuaternion globalRot = mClient.GetSegmentGlobalRotationQuaternion(subjectName, sn.SegmentName);

                std::string segname = sn.SegmentName;
                subject->setSegment(QString(segname.c_str()), trans.Translation, localRot.Rotation);

            }

            Output_GetMarkerCount mc = mClient.GetMarkerCount(subjectName);
            for(unsigned int i=0; i < mc.MarkerCount; i++)
            {
                Output_GetMarkerName mn = mClient.GetMarkerName(subjectName, i);
                Output_GetMarkerGlobalTranslation trans = mClient.GetMarkerGlobalTranslation(subjectName, mn.MarkerName);
                std::string markername = mn.MarkerName;
                subject->setMarker(QString(markername.c_str()), trans.Translation);

            }


        }

        emit newFrame(frameNumber);
        count++;
        */
    }

    outMessage("NaturalPoint Service has finished");

    mocapDisconnect();
}
