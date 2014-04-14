// UnityDllTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UnityDllTest.h"
#include <Windows.h>

#include <tchar.h>



#define NOTEON  144
#define NOTEOFF 128
#define CTRLCHANGE 176
#define PROGCHANGE 192


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH :
		midiout = new RtMidiOut();
		
		return TRUE;

	case DLL_PROCESS_DETACH :
		delete midiout;
		if (hPipe != INVALID_HANDLE_VALUE) CloseHandle(hPipe);
		return TRUE;
			
	}
	return TRUE;
}


int test()
{
	return 42;
}

#define BUFSIZE 512

void GetError(char *buf, size_t buflen)
{
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, buflen, NULL);
}

void init()
{
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\VIVE");
	hPipe = CreateFile(lpszPipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa365592(v=vs.85).aspx
int getData(char *buf, size_t buflen)
{
	BOOL   fSuccess = FALSE;
	DWORD  cbRead;
	
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\VIVE");
		hPipe = CreateFile(lpszPipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) return 100;
	}

	

	fSuccess = ReadFile(hPipe, buf, buflen , &cbRead, NULL);    // not overlapped 

	if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
	{
		GetError(buf, buflen);
		return 101;
	}
	if (cbRead == 0) return 1;

	buf[cbRead] = 0;

	return 0;
	
}

int peek()
{
	DWORD bytes;
	PeekNamedPipe(hPipe, NULL, NULL, NULL, &bytes, NULL);
	return (int)bytes;
}



unsigned int getPorts()
{
	return midiout->getPortCount();
}

bool setPort(unsigned int port)
{
	if (port >= midiout->getPortCount()) return false;
	midiout->openPort(port);
	return true;
}

void getPortName(unsigned int port, char* buf, size_t buflen)
{
	if (buflen == 0) return;
	std::string s = midiout->getPortName(port);
	size_t len = s.length();
	if (len == 0)
	{
		buf[0] = 0;
		return;
	}
	if (len >= buflen) len = buflen - 1;
	strncpy(buf, s.c_str(), buflen);
	buf[len] = 0;
	return;
}
	
	

void noteOn(unsigned char channel, unsigned char note, unsigned char velocity)
{
	std::vector<unsigned char> message;
	message.push_back(NOTEON | channel);
	message.push_back(note);  // key 
	message.push_back(velocity);  // velocity
	midiout->sendMessage(&message);
}
void noteOff(unsigned char channel, unsigned char note, char velocity)
{
	std::vector<unsigned char> message;
	message.push_back(NOTEOFF | channel);
	message.push_back(note);  // key 
	message.push_back(velocity);  // velocity
	midiout->sendMessage(&message);
}

void progChange(unsigned char channel, unsigned char prog)
{
	std::vector<unsigned char> message;
	message.push_back(PROGCHANGE | channel);
	message.push_back(prog);
}

void ctrlChange(unsigned char channel, unsigned char ctrl, unsigned char value)
{
	std::vector<unsigned char> message;
	message.push_back(CTRLCHANGE | channel);
	message.push_back(ctrl);
	message.push_back(value);
	midiout->sendMessage(&message);

}

