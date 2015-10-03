#include "repeater.h"

#include <string>
#include "RepeaterListener.h"
#include "peel/Socket.h"

RepeaterListener *listener;





bool Repeater::initialize()
{
	return peel::SocketsInit();
}

bool Repeater::shutdown()
{
	return peel::SocketsCleanup();
}

bool Repeater::startListening(int port)
{
	listener = new RepeaterListener(port);
	return listener->start();
	 
}



