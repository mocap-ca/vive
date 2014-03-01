#include "profile.h"

#include <QTextStream>
QList< ProfileTimer* > profileTimers;

void startProfile(QString name)
{
    for( int i = 0; i < profileTimers.length(); i++)
    {
        if(profileTimers[i]->name == name)
        {
            profileTimers[i]->timer.restart();
            return;
        }
    }

    ProfileTimer *p = new ProfileTimer(name);
    p->timer.start();
    profileTimers.append( p );
}

void stopProfile(QString name)
{
    for( int i = 0; i < profileTimers.length(); i++)
    {
        if(profileTimers[i]->name == name)
        {
            ProfileTimer *p = profileTimers[i];
            p->time += p->timer.nsecsElapsed();
            return;
        }
    }

}

void finishProfile(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for( int i = 0; i < profileTimers.length(); i++)
    {
        out << profileTimers[i]->name << "\t" << profileTimers[i]->time << "\n";
    }
    file.close();

}
