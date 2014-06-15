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
#include <QPushButton>
#include <QTimer>
#include "baseClient.h"
#include "mocapSubject.h"

class TestClient : public BaseClient
{
	Q_OBJECT

public slots:
    void mocapStart();
    void mocapStop();
    void mocapWait();
    void runOne();

public:
    TestClient( MocapSubjectList *subjectList,
                QPushButton *button,
                QLineEdit *statusLine,
                QObject *parent = NULL);

    //! @returns "Test"
    virtual QString ClientStr() { return QString("Test"); }

    //! @returns true if the service is running
    virtual bool isRunning() { return running; }

    virtual bool isConnected() { return running; }

    QTimer *timer;

	bool running;

    float val, mousex, mousey;

};



#endif
