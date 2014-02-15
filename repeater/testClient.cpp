#include "testClient.h"


#include <string>
#include <sstream>
#include <windows.h>


TestClient::TestClient(MocapSubjectList *sList, QObject *parent)
: QThread(parent)
, subjects(sList)
, running(false)
, count(0)
, tx(0)
, ty(0)
, tz(0)
, rx(0)
, ry(0)
, rz(0)
{}


void TestClient::run()
{
    running = true;
    outMessage("Test client starting.");
	while(running)
	{
		MocapSubject *subject;

		subject = subjects->find(QString("TEST"));

        ry += 0.001f;
        if(ry >= 3.14) ry = 0;

        double tr[3] = { tx, ty, tz };
        double ro[3] = { rx, ry, rz };
        //tr[1] = (double)qrand() / (double)RAND_MAX;
        subject->set("root", tr, ro);

        this->usleep(4000);
		count++;
	}
    outMessage("Test client ended.");
}


