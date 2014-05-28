/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr University

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QObject>
#include <QList>
#include <QThread>
#include "mocapSubject.h"

class TestClient : public QThread
{
	Q_OBJECT

public:
	TestClient( MocapSubjectList *subjectList, QObject *parent = NULL);

	friend QTextStream& operator << (QTextStream &, TestClient &);

	MocapSubjectList *subjects;

	virtual void run();

	bool running;

	size_t count;

    float val, mousex, mousey;

signals:
    void outMessage(QString);
    void newFrame(int);
};



#endif
