#include "viveClient.h"
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMessageBox>


ViveClient::ViveClient(MocapSubjectList *sList,
                       QPushButton* button,
                       QLineEdit*   statusLine,
                       QLineEdit*   inHostField,
                       QLineEdit*   inPortField,
                       QObject*     parent) :
    BaseClient(CL_Vive, sList, button, statusLine, parent)
  , hostField(inHostField)
  , portField(inPortField)
  , socket( new QTcpSocket(this) )
{
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void ViveClient::error(QAbstractSocket::SocketError e)
{
    emit outMessage(QString("Vive Client Error: %1").arg(socket->errorString()));
}

bool ViveClient::isConnected()
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void ViveClient::mocapStop()
{
    UIDisconnectingState();
    socket->disconnectFromHost();
    //UIDisconnectedState();
}

void ViveClient::mocapWait()   { }

void ViveClient::mocapStart()
{
    QString host = hostField->text();
    int     port = portField->text().toInt();

    if(port == 0)
    {
        QMessageBox::warning(NULL,"Error", "Invalid Port", QMessageBox::Ok);
        return;
    }

    // start the connection
    UIConnectingState();
    socket->connectToHost(host, port);
}

void ViveClient::readData()
{
    QByteArray x = socket->readAll();
    outMessage(QString("Read: %1").arg(x.length()));
    count++;
}

void ViveClient::setConnected()
{
    UIConnectedState();
}

void ViveClient::setDisconnected()
{
    // This does not appear to be called (?)
    UIDisconnectedState();
}
