#include <stdio.h>

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <string>
#include <sstream>
#include <iostream>

#include "mocapData.h"

#pragma comment(lib, "Ws2_32.lib") 

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		fprintf(stderr, "Could not init sockets");
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		fprintf(stderr, "Could not create socket");
		WSACleanup();
		return -1;
	}

	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));
	u_short port = 4001;

	serv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port = htons(port);

	//struct hostent *server = gethostbyname("localhost");
	//memcpy((void*)serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	if (connect(s, (SOCKADDR*) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{ 
		fprintf(stderr, "Could not connect");
		closesocket(s);
		WSACleanup();
		return -1;
	}

	std::string data, last;

	char buf[255];
	while (1)
	{
		int r = recv(s, buf, 255, 0);
		if (r < 1) break;

		data.append(buf, r);
		size_t a = 0, z = 0, next = 0, frames = 0;

		while (1)
		{
			next = data.find("END\r\n", z);
			if (next == -1) break;
			a = z;
			z = next + 5;
			frames++; 
		}

		if (frames == 0) continue;

		last = data.substr(a, z - a - 5);
		data = data.substr(z);

		std::istringstream  ss(last);

		int items, markers, segments;
		std::string name, line;

		ss >> items;

		for (int item = 0; item < items; item++)
		{

			ss >> name;
			ss >> markers;
			ss >> segments;

			for (int i = 0; i < markers; i++)
			{
				std::getline(ss, line, '\n');
				std::istringstream ssmarker(line);
				std::string markerName;
				float tx, ty, tz;
				ssmarker >> markerName;
				ssmarker >> tx;
				ssmarker >> ty;
				ssmarker >> tz;

				std::cout << markerName << "\t" << tx << "\t" << ty << "\t" << tz << std::endl;
			}


			for (int i = 0; i < segments; i++)
			{
				std::getline(ss, line, '\n');
				std::istringstream ssegment(line);
				std::string segmentName;
				float tx, ty, tz, rx, ry, rz;
				ssegment >> segmentName;
				ssegment >> tx;
				ssegment >> ty;
				ssegment >> tz;
				ssegment >> rx;
				ssegment >> ry;
				ssegment >> rz;

				std::cout << segmentName << "\t" << tx << "\t" << ty << "\t" << tz << "\t" << rx << "\t" << ry << "\t" << rz << std::endl;

			}

			if (ss.bad()) break;

		}




		//+last	"1\nTEST\t1\t1\nroot\t0\t3.1\t0\t0\t0\t0\t0\nmarker1\t10\t3.1\t10\n\n"	std::basic_string<char, std::char_traits<char>, std::allocator<char> >



	}


	closesocket(s);
	WSACleanup();



}