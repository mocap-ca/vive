#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include "baseClient.h"


class FileClient : public BaseClient
{
    Q_OBJECT

public:
    FileClient(QPushButton *, QLineEdit *, QLineEdit *, QObject *);

    QFile*  file;
    QTimer* timer;
    int     fps;

private:
    QLineEdit *lineEditPath;

private slots:
    void tick();

protected:

    virtual QString ClientStr() { return "File"; }

    virtual bool isConnected();
    virtual void mocapStart();
    virtual void mocapStop();
    virtual void mocapWait();

    virtual void UIConnectingState();
    virtual void UIDisconnectingState();
    virtual void UIConnectedState();
    virtual void UIDisconnectedState();

signals:
    void newFrame(uint);
    bool isIdle(bool);

};

#endif // FILEDATA_H
