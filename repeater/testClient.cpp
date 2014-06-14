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

TestClient::TestClient(MocapSubjectList *sList,
                       QPushButton *button,
                       QLineEdit *statusLine,
                       QObject *parent)
    : BaseClient(BaseClient::CL_Stub, sList, button, statusLine, parent)
, running(false)
, val(0)
, mousex(0)
, mousey(0)
{
    timer = new QTimer(this);
    timer->setInterval(1000 / 60);
    connect(timer, SIGNAL(timeout()), this, SLOT(runOne()));
}

void TestClient::mocapStart()
{
    outMessage("Test client starting.");
    timer->start();
    emit stateConnected();
    running = true;
    connected = true;
}

void TestClient::mocapStop()
{
    timer->stop();
    running = false;
    connected = false;
    emit stateDisconnected();
    outMessage("Test client ended.");
}

void TestClient::mocapWait()
{
    return;
}

void TestClient::runOne()
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

    emit updateFrame(BaseClient::CL_Stub, (uint)count);

    count++;
}


