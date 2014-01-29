#include "testClient.h"


#include <string>
#include <sstream>
#include <windows.h>


TestClient::TestClient(MocapSubjectList *sList, QObject *parent)
: QThread(parent)
, subjects(sList)
, running(true)
, count(0)
{}


void TestClient::run()
{
	while(running)
	{
		MocapSubject *subject;

		subject = subjects->find(QString("TEST"));

		double tr[3] = { 0., 0., 0. };
		double ro[3] = { 0., 0., 0. };
		tr[1] = (double)qrand() / (double)RAND_MAX;
        subject->set("ROOT", tr, ro);

		this->usleep(7000);
		count++;
	}
}


