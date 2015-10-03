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

#include "testConnector.h"
#include "encoder.h"
#include "mocapData.h"
#include "atomList.h"
#include <stdio.h>
#include <assert.h>


static void   command(STRING);

static void*            mPtr          = NULL;
static FrameCallback    mFrameFunc    = NULL;
static MessageCallback  mMessageFunc  = NULL;
static bool             mConnected    = false;

//--------------------------------

bool TestConnector::Initialize(
	void *ptr)
{
	mPtr = ptr;
	mConnected = false;
	return true;
}

void TestConnector::setFrameCallback( FrameCallback   frameFunc)
{
	mFrameFunc = frameFunc;
}

void TestConnector::setMessageCallback( MessageCallback messageFunc)
{
	mMessageFunc = messageFunc;
}


const char * TestConnector::clientStr() { return "Test"; }


bool TestConnector::isConnected()
{
	return mConnected;
}

bool TestConnector::connect()
{
	return true;
}

bool TestConnector::disconnect()
{
	return true;
}

bool TestConnector::start()
{
	mConnected = true;
	command("CONNECTED");
	return true;
}

bool TestConnector::stop()
{
	mConnected = false;
	return true;
}


void TestConnector::wait()
{
	return;
}

void command(STRING msg)
{
	if (mMessageFunc) mMessageFunc(mPtr, msg.c_str());
}

void TestConnector::test()
{	
	float t[3];
	t[0] = 0.0f;
	t[1] = (float)rand() / RAND_MAX;
	t[2] = 1.0;

	MocapMarker *marker = new MocapMarker("test", t);
	IDList::Instance()->getId(marker);

	t[0] = 1.0f;
	t[1] = (float)rand() / RAND_MAX;
	t[2] = 0.1f;


	float r[4];
	r[0] = 0.0f;
	r[1] = 0.0f;
	r[2] = 0.0f;
	r[3] = 1.0f;

	MocapSegment *segment = new MocapSegment("segment", t, r);
	IDList::Instance()->getId(segment);


	std::vector<MocapItem*> data;
	IDListType x = IDList::Instance()->items;

	for (IDListType::iterator i = x.begin(); i != x.end(); i++)
	{
		data.push_back((*i).second);

	}

	mFrameFunc(mPtr);

		
}