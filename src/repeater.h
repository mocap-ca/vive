#ifndef REPEATER_H
#define REPEATER_H

#if defined(_WINDLL)
#define DLL_API __declspec(dllexport) 
#else
#define DLL_API __declspec(dllimport) 
#endif

namespace Repeater
{
	DLL_API bool initialize();
	DLL_API bool shutdown();
	DLL_API bool setInterface(const char *);
	DLL_API bool listenPort(int);

	DLL_API bool startListening(int port);
	DLL_API bool stopListening();

}



#endif