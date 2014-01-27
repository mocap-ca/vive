#include "testClient.h"


#include <string>
#include <sstream>


TestClient::TestClient(MocapSubjectList *sList, QObject *parent)
: QObject(parent)
, subjects(sList)
{
}


void TestClient::getFrame()
{

	MocapSubject *subject;

	subject = subjects->find(QString("TEST"));

	double tr[3] = { 0., 0., 0. };
	double ro[3] = { 0., 0., 0. };
	tr[1] = (double)qrand() / (double)RAND_MAX;
	subject->segments.set("ROOT", tr, ro);

	return;
}


