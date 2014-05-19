#include "testClient.h"


#include <string>
#include <sstream>
// #include <windows.h>


TestClient::TestClient(MocapSubjectList *sList, QObject *parent)
: QThread(parent)
, subjects(sList)
, running(false)
, count(0)
, val(0)
, mousex(0)
, mousey(0)
{}


void TestClient::run()
{
    running = true;
    outMessage("Test client starting.");
	while(running)
	{
		MocapSubject *subject;

		subject = subjects->find(QString("TEST"));

        val += 0.1f;
        if(val >= 10.0f) val = 0.0f;

        double tr[3] = { mousex, val, mousey };
        double ro[4] = { 0, 0, 0, 0 };
        //tr[1] = (double)qrand() / (double)RAND_MAX;
        subject->setSegment("root", tr, ro);


        tr[0] = 10.0f;
        tr[2] = 10.0f;
        subject->setMarker("marker1", tr);

        emit newFrame(0);

        this->usleep(10000);
		count++;
	}
    outMessage("Test client ended.");
}


