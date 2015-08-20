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

#include "testClient.h"


#include <string>
#include <sstream>
// #include <windows.h>
#include <QLineEdit>
#include <QPushButton>

TestConnector::TestConnector(QObject *parent)
    : BaseConnector(parent)
    , running(false)
    , val(0)
    , mousex(0)
    , mousey(0)
    , count(0)
{}

void TestConnector::run()
{
    running = true;
    emit conConnecting();
    QThread::sleep(2);
    emit conConnected();
    while(running)
    {
        SubjectData *subject1 = new SubjectData(QString("TEST1"), CL_Stub);

        val += 0.1f;
        if(val >= 10.0f) val = 0.0f;

        double tr[3] = { mousex, val, mousey };
        double ro[4] = { 0, 0, 0, 0 };
        //tr[1] = (double)qrand() / (double)RAND_MAX;
        subject1->setSegment("root", tr, ro);

        tr[0] = 10.0f;
        tr[2] = 10.0f;
        subject1->setMarker("marker1", tr);

        emit conUpdateSubject(subject1);

        SubjectData *subject2 = new SubjectData(QString("Oculus"), CL_Stub);
        subject2->setSegment("Oculus", tr, ro);
        emit conUpdateSubject(subject2);

        emit conNewFrame();

        QThread::msleep(1000 / 60);
    }
    emit conDisconnecting();
    QThread::sleep(2);
    emit conDisconnected();
}

bool TestConnector::connect()
{
    return true;
}

void TestConnector::stop()
{
    running = false;
}

TestClient::TestClient(QPushButton *button,
                       QLineEdit *statusLine,
                       QObject *parent)
    : BaseClient(CL_Stub, button, statusLine, parent)
, connector(NULL)
{
    connector = new TestConnector(this);
    linkConnector(connector);
}


void TestClient::mocapStart()
{
    if(connector->running)
    {
        emit outMessage("Skipping attempt to start already running test server... this is probably a bug");
        return;
    }

    // Handed on other thread to mimic other servers
    connector->start();
}

void TestClient::mocapStop()
{
    connector->stop();
}

void TestClient::mocapWait()
{
    connector->wait();
    return;
}



