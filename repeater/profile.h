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
