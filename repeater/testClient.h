


#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QObject>
#include <QList>
#include "mocapSubject.h"

class TestClient : public QObject
{
	Q_OBJECT

public:
	TestClient( MocapSubjectList *subjectList, QObject *parent = NULL);

	
	friend QTextStream& operator << (QTextStream &, TestClient &);

	MocapSubjectList *subjects;

public slots:
	void getFrame();
};



#endif