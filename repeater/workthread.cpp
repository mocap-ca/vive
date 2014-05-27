/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Emily Carr University

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

#include "workthread.h"


WorkThread::WorkThread(QObject *parent, MyServer *server) :
    QThread(parent),
    myserver(server),
    isWorking(true)
{
    timer.start();
}

// periodically generates a signalFrame event 
void WorkThread::run()
{
	int frameCount = 0;
	int completedFrames = 0;

    while(isWorking)
    {
		if(!myserver->working)
		{
			emit signalFrame();
            QTime *t = new QTime();
            t->start();
            emit measureLatency(t);
			completedFrames++;
		}

		frameCount++;

        int diff = frameCount * 5 - timer.elapsed() ;  // 200fps = 1000 / 200

        if( diff > 0 )
        {
            this->usleep( diff * 1000);
        }

        if(timer.elapsed() > 1000)
        {
            //emit outFrameRate(completedFrames);
            timer.restart();
            frameCount = 0;
			completedFrames = 0;
        }
    }
}

void WorkThread::measureLatency(QTime *t)
{
    emit outFrameRate(t->elapsed());
    delete t;
}


void WorkThread::stopWorking()
{
    isWorking = false;
}

