#include "baseclient.h"

#include <QString>

BaseClient::BaseClient(ClientId clientId,
        MocapSubjectList *subjectList,
        QPushButton *pushButton,
        QLineEdit *lineEditStatus,
        QObject *parent)
: QThread(parent)
, subjects(subjectList)
, connected(false)
, count(0)
, id(clientId)
, button(pushButton)
, statusLine(lineEditStatus)
{
    QObject::connect(button, SIGNAL(clicked()),                  this,   SLOT(handleButtonClick()),     Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(stateConnecting()),          this,   SLOT(UIConnectingState()),     Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(stateConnected()),           this,   SLOT(UIConnectedState()),      Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(stateDisconnecting()),       this,   SLOT(UIDisconnectingState()),  Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(stateDisconnected()),        this,   SLOT(UIDisconnectedState()),   Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(connectedEvent(bool)),       this,   SLOT(stateChange(bool)),        Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(outMessage_(QString)),       parent, SLOT(showMessage(QString)),     Qt::QueuedConnection);
    QObject::connect(this,   SIGNAL(newFrame_(ClientId, uint)),  parent, SLOT(newFrame(ClientId, uint)), Qt::QueuedConnection);
}

void BaseClient::tick()
{
    if(isRunning())
    {
        statusLine->setText(QString("%1").arg(count));
        count=0;
    }
    else
    {
        statusLine->setText(QString("Not Connected"));
    }
}

void BaseClient::outMessage(QString msg)
{
    emit outMessage_(QString("%1 - %2").arg(this->ClientStr()).arg(msg));
}

void BaseClient::newFrame(unsigned int i)
{
    emit newFrame_(this->id, i);
}


void BaseClient::handleButtonClick()
{
    if(connected)
    {
        mocapStop();
    }
    else
    {
        mocapStart();

        this->start();
    }
}

void BaseClient::UIConnectingState()
{
    button->setEnabled(false);
    button->setText("Connecting");
}

void BaseClient::UIConnectedState()
{
    button->setEnabled(true);
    button->setText("Disconnect");
}

void BaseClient::UIDisconnectingState()
{
    button->setEnabled(false);
    button->setText("Disconnecting");
}

void BaseClient::UIDisconnectedState()
{
    button->setEnabled(true);
    button->setText("Connect");
}


