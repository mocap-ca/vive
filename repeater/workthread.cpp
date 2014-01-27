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

