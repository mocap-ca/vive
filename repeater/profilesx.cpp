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
