// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UNITYDLLTEST_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UNITYDLLTEST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
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
