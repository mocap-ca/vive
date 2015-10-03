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


#ifndef TEST_CONNECTOR_H
#define TEST_CONNECTOR_H

#ifdef _WINDLL
#define DLL_API __declspec(dllexport) 
#else
#define DLL_API __declspec(dllimport) 
#endif


#include "mocapData.h"
#include "connector.h"


namespace TestConnector
{

	enum ConnectionType { MULTICAST, UNICAST };

	DLL_API bool Initialize(void*);

	DLL_API void setFrameCallback(FrameCallback);
	DLL_API void setMessageCallback(MessageCallback);

	DLL_API const char *clientStr();
	DLL_API bool   isConnected();
	DLL_API bool   connect();
	DLL_API bool   disconnect();
	DLL_API bool   start();
	DLL_API bool   stop();
	DLL_API void   wait();
	DLL_API void   test();


};


#endif // NATURALPOINTCLIENT_H
