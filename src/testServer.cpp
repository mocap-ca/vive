// testServerVS2013.cpp : Defines the entry point for the console application.
//


#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include "atomList.h"

#include "testConnector.h"
#include "naturalPointConnector.h"

#include "decoder.h"
#include "encoder.h"

#pragma comment(lib, "Ws2_32.lib")

void __stdcall frameFunc(void *ptr)
{
	size_t markerLen, segmentLen;
	MocapMarker **markers = NULL;
	MocapSegment **segments = NULL;

	NaturalPointConnector::getMarkers(markers, markerLen);
	NaturalPointConnector::getSegments(segments, segmentLen);

	for (size_t i = 0; i < markerLen; i++)
	{
		std::cout << "M" << markers[i]->name << "  ";
		std::cout << markers[i]->translation[0] << "  ";
		std::cout << markers[i]->translation[1] << "  ";
		std::cout << markers[i]->translation[2] << std::endl;
	}
	for (size_t i = 0; i < segmentLen; i++)
	{
		std::cout << segments[i]->name << "  ";
		std::cout << segments[i]->translation[0] << "  ";
		std::cout << segments[i]->translation[1] << "  ";
		std::cout << segments[i]->translation[2] << "     ";
		std::cout << segments[i]->rotation[0] << "  ";
		std::cout << segments[i]->rotation[1] << "  ";
		std::cout << segments[i]->rotation[2] << std::endl;
	}

}

void __stdcall messageFunc(void *ptr, const char* val)
{
	std::cout << "-" << val << std::endl;
}



int main(int argc, char *argv[])
{


	NaturalPointConnector::Initialize(NULL,
		NaturalPointConnector::MULTICAST,
		"192.168.1.99",
		"192.168.1.20",
		1510,
		1511);


	NaturalPointConnector::setMessageCallback(messageFunc);
	NaturalPointConnector::setFrameCallback(frameFunc);

	NaturalPointConnector::start();

#if 0
	TestConnector::Initialize(NULL);

	TestConnector::messageCallback(controlFunc);
	TestConnector::streamCallback(headerFunc, streamFunc);
#endif

	
	while (1)
	{
		Sleep(500);

		//TestConnector::test();
		//NaturalPointConnector::test();



	}

}