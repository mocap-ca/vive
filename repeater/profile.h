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

#ifndef MOCAP_SOCKET_PROFILE_TIMER
#define MOCAP_SOCKET_PROFILE_TIMER

#include <QElapsedTimer>
#include <QList>
#include <QPair>
#include <QString>
#include <QFile>

class ProfileTimer
{
public:
    ProfileTimer(QString n) : name(n), time(0) {}
    QString name;
    QElapsedTimer timer;
    qint64 time;
};

extern QList< ProfileTimer* > profileTimers;

void startProfile(QString name);

void stopProfile(QString name);

void finishProfile(QString fileName);

#endif
