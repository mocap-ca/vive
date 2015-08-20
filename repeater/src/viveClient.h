#ifndef VIVECLIENT_H
#define VIVECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

#include "baseClient.h"


class ViveClient : public BaseClient
{
    Q_OBJECT
public:
    ViveClient( QPushButton* button,
               QLineEdit*   statusLine,
               QLineEdit*   hostField,
               QLineEdit*   portField,
               QObject *parent = 0);

    virtual QString ClientStr() { return QString("Vive"); }

    QTcpSocket *socket;

    virtual void mocapStart();
    virtual void mocapStop();
    virtual void mocapWait();
    virtual bool isRunning() { return socket->isOpen(); }
    virtual bool isConnected();

    QLineEdit *hostField;
    QLineEdit *portField;

public slots:
    void readData();
    void setConnected();
    void setDisconnected();
    void error(QAbstractSocket::SocketError);

};

#endif // VIVECLIENT_H
