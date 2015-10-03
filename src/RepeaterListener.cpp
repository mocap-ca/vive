#include "RepeaterListener.h"

#include <string>

#include <ostream>

#pragma comment(lib, "Ws2_32.lib")

bool RepeaterListener::receiveData(peel::Socket *s)
{
	std::string data;
	s->Receive(data);

	std::cout << data;

	return true;
}