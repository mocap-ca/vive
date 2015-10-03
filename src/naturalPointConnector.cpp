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

#include "naturalpointConnector.h"
#include "defines.h"
#include <stdio.h>
#include <assert.h>

// These includes should be in the .cpp file only, not in .h
#include "NatNetTypes.h"
#include "NatNetClient.h"



static void   command(STRING);

static void*            mPtr;
static FrameCallback    mFrameFunc = NULL;
static MessageCallback  mMessageFunc = NULL;
static NaturalPointConnector::ConnectionType mNetworkMode; // 0 multi, 1 uni 
static STRING           mNetworkInterface;
static STRING           mMulticastAddress;
static int              mCommandPort;
static int              mDataPort;



NatNetClient* gClient = NULL;

void NPdataCallback(sFrameOfMocapData*, void*);
void NPmessageCallback(int, char *);



//--------------------------------

bool NaturalPointConnector::Initialize(
	void *ptr,
	NaturalPointConnector::ConnectionType networkMode,
	char*  networkInterface,
	char*  multicastAddress,
	int    commandPort,
	int    dataPort)
{
	mPtr = ptr;
	mNetworkMode = networkMode;
	mNetworkInterface = networkInterface;
	mMulticastAddress = multicastAddress;
	mCommandPort = commandPort;
	mDataPort = dataPort;

	if (gClient != NULL) NaturalPointConnector::stop();

	return true;
}

void NaturalPointConnector::setFrameCallback(FrameCallback   frameFunc)
{
	mFrameFunc = frameFunc;
}

void NaturalPointConnector::setMessageCallback(MessageCallback messageFunc)
{
	mMessageFunc = messageFunc;
}

void NaturalPointConnector::getMarkers(MocapMarker ** &markers, size_t& len)
{
	len = markerList.size();
	if (len == 0) return;

	MocapMarker** markerDataPtr = markerList.data();

	markers = (MocapMarker**)malloc(len * sizeof(void*));
	//std::copy(markerList.front(), markerList.back(), *markers);
	//memcpy(markers, markerList.data(), len * sizeof(void*));
	for (size_t i = 0; i < len; i++)
	{		markers[i] = markerDataPtr[i];}
}

void NaturalPointConnector::getSegments(MocapSegment **& segments, size_t& len)
{
	len = segmentList.size();
	if (len == 0) return;

	MocapSegment** segmentDataPtr = segmentList.data();

	segments = (MocapSegment**)malloc(len * sizeof(void*));
	//std::copy(segmentList.front(), segmentList.back(), *segments);
	//memcpy(&segments, segmentList.data(), len * sizeof(void*));
	for (size_t i = 0; i < len; i++)
	{
		segments[i] = segmentDataPtr[i];
	}
}

const char * clientStr() { return "NaturalPoint"; }


bool NaturalPointConnector::isConnected()
{
	return gClient != NULL;
}


bool NaturalPointConnector::start()
{
	stop();

	if (mCommandPort == 0)
	{
		command("ERROR Invalid Command Port");
		return false;
	}

	if (mDataPort == 0)
	{
		command("ERROR Invalid data port");
		return false;
	}

	command("CONNECTING");

	// 0 = multicast, 1 = unicast (see nat net docs)
	int type = (mNetworkMode == MULTICAST) ? 0 : 1;

	std::string stype;
	if (type == 0) stype = "Multicast";
	if (type == 1) stype = "Unicast";

	std::stringstream oss;
	oss << "MESSAGE Connecting to Naturalpoint: " << mMulticastAddress << " (" << stype << ")";
	command(oss.str());

	gClient = new NatNetClient(type);

	char *iface = (char*)mNetworkInterface.c_str();
	char *mc = (char*)mMulticastAddress.c_str();

	int ret = gClient->Initialize(iface, mc, mCommandPort, mDataPort);

	if (ret == 0)
	{
		if (gClient->SetDataCallback(&NPdataCallback, NULL) != 0)
		{
			command("ERROR Stting data callback");
		}
		if (gClient->SetMessageCallback(&NPmessageCallback) != 0 )
		{
			command("ERROR Setting Message Callback");
		}
		command("CONNECTED");
		return true;
	}
	else
	{
		command("DISCONNECTED");
		delete gClient;
		gClient = NULL;
		return false;
	}
}

bool NaturalPointConnector::stop()
{
	if (gClient == NULL) return true;
	command("DISCONNECTING");
	gClient->Uninitialize();
	delete gClient;
	gClient = NULL;
	command("DISCONNECTED");
	return true;
}

void NPmessageCallback(int id, char *msg)
{
	std::stringstream ss;
	ss << msg << "(" << id << ")";
	command(ss.str());

}

static int counter = 1095328;

void NaturalPointConnector::test()
{
	char buf[1024];
	sprintf_s(buf, 1024, "c:\\data\\rb.%04d.data", counter);
	FILE *fp;
	if( fopen_s(&fp, buf, "rb") == 0)
	{
		counter++;
	}
	else
	{
		counter = 1095328;
		return;
	}

	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);

	char *data = (char*)malloc(sz);
	fseek(fp, 0L, SEEK_SET);

	fread(data, sz, 1, fp);
	
	NPdataCallback((sFrameOfMocapData*)data, mPtr);

	free(data);
	fclose(fp);
}

void NPdataCallback(sFrameOfMocapData* frameData, void *ptr)
{
	/*
	char buf[1024];
	sprintf_s(buf, 1024, "c:\\data\\ch.%04d.data", frameData->iFrame);
	FILE *fp;
	fopen_s(&fp, buf, "wb");
	size_t xxx = sizeof(*frameData);
	fwrite((void*)frameData, sizeof(*frameData), 1, fp);
	fclose(fp);
	*/

	if (frameData == NULL)
	{
		command("ERROR Error getting Naturalpoint frame");
		return;
	}

	// Clear the lists
	for (std::vector< MocapMarker*>::iterator i = NaturalPointConnector::markerList.begin();
		i != NaturalPointConnector::markerList.end(); i++)
	{ delete (*i); }
	NaturalPointConnector::markerList.clear();


	for (std::vector< MocapSegment*>::iterator i = NaturalPointConnector::segmentList.begin();
		i != NaturalPointConnector::segmentList.end(); i++)
	{ delete (*i); }
	NaturalPointConnector::segmentList.clear();



	// Rigidbodies
	unsigned int subjectCount = frameData->nRigidBodies;
	for (unsigned int iSubject = 0; iSubject < subjectCount; iSubject++)
	{
		sRigidBodyData *rb = &frameData->RigidBodies[iSubject];

		// ASSUMPTION:  rb => frameData->MocapData[rb->ID-1].szName
		int markerSetID = rb->ID - 1;
		assert(markerSetID >= 0 && markerSetID<frameData->nMarkerSets);
		sMarkerSetData *markerSet = &frameData->MocapData[rb->ID - 1];

		// Find/Create the subject by name.
		STRING subjectName(markerSet->szName);

		// Load the subject's translation and rotation quaternion.
		float t[3] = { rb->x, rb->y, -rb->z };
		float r[4] = { -rb->qx, -rb->qy, rb->qz, rb->qw };
		std::string segName(subjectName + ":root");
		MocapSegment *segment = new MocapSegment(segName.c_str(), t, r);
		NaturalPointConnector::segmentList.push_back(segment);

		for (int iMarker = 0; iMarker < rb->nMarkers; iMarker++)
		{
			MarkerData *m = &rb->Markers[iMarker];
			float markerPos[3] = { *m[0], *m[1], *m[2] };
			if (rb->MarkerIDs)
			{
				std::stringstream ss;
				ss << rb->MarkerIDs[iMarker];

				MocapMarker *marker = new MocapMarker(ss.str().c_str() , markerPos);
				NaturalPointConnector::markerList.push_back(marker);
			}
			else
			{
				std::stringstream ss;
				ss << iMarker;
				MocapMarker *marker = new MocapMarker(ss.str().c_str(), markerPos);
				NaturalPointConnector::markerList.push_back(marker);
			}
		}

	}

	mFrameFunc(mPtr); //  notify frame is available;


}

void command(STRING data)
{
	if (mMessageFunc) mMessageFunc(mPtr, data.c_str());

}

