/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Emily Carr University

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


#ifdef UNITYDLLTEST_EXPORTS
#define UNITYDLLTEST_API __declspec(dllexport)
#else
#define UNITYDLLTEST_API __declspec(dllimport)
#endif 

#include "RtMidi.h"
#include <string>
#include <Windows.h>

RtMidiOut *midiout;
HANDLE hPipe;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

extern "C" {

	UNITYDLLTEST_API int  test();
	UNITYDLLTEST_API void init();
	UNITYDLLTEST_API int  peek();
	UNITYDLLTEST_API int getData(char *buf, size_t buflen);
	UNITYDLLTEST_API unsigned int getPorts();
	UNITYDLLTEST_API void getPortName(unsigned int port, char buf, size_t buflen);
	UNITYDLLTEST_API bool setPort(unsigned int);
	UNITYDLLTEST_API void noteOn(unsigned char chan, unsigned char note, unsigned char velocity);
	UNITYDLLTEST_API void noteOff(unsigned char chan, unsigned char note, char velocity);
	UNITYDLLTEST_API void progChange(unsigned char chan, unsigned char prog);
	UNITYDLLTEST_API void ctrlChange(unsigned char chan, unsigned char ctrl, unsigned char val);

}
