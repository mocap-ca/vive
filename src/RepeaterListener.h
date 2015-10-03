#ifndef REPEATER_LISTENER_H
#define REPEATER_LISTENER_H

#include "peel/ListenThread.h"

class RepeaterListener : public peel::ListenThread
{
public:
	RepeaterListener(int port) : peel::ListenThread(port) {};

	virtual bool receiveData(peel::Socket *s);
};

#endif