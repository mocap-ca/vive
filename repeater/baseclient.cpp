#include "baseclient.h"

#include <QString>
#include <QMutexLocker>

BaseConnector::BaseConnector(QObject *parent)
: QThread(parent)
{}



BaseClient::BaseClient(ClientId clientId,
        QPushButton *pushButton,
        QLineEdit *lineEditStatus,
        QObject *parent)
: QObject(parent)
, button(pushButton)
, statusLine(lineEditStatus)
, count(0)
, id(clientId)
{
    QObject::connect(button, SIGNAL(clicked()),                  this,   SLOT(handleButtonClick()));
    QObject::connect(this,   SIGNAL(stateConnecting()),          this,   SLOT(UIConnectingState()));
    QObject::connect(this,   SIGNAL(stateConnected()),           this,   SLOT(UIConnectedState()));
    QObject::connect(this,   SIGNAL(stateDisconnecting()),       this,   SLOT(UIDisconnectingState()));
    QObject::connect(this,   SIGNAL(stateDisconnected()),        this,   SLOT(UIDisconnectedState()));
    QObject::connect(this,   SIGNAL(outMessage_(QString)),       parent, SLOT(showMessage(QString)));
    QObject::connect(this,   SIGNAL(updateFrame(ClientId,uint)), parent, SLOT(processFrame(ClientId, uint)));
}

void BaseClient::tick()
{
    if(isConnected())
    {
        statusLine->setText(QString("%1").arg(count));
        count=0;
    }
    else
    {
        statusLine->setText(QString("Not Connected"));
    }
}

void BaseClient::addCount()
{
    count++;
}

void BaseClient::outMessage(QString msg)
{
    emit outMessage_(QString("%1 - %2").arg(this->ClientStr()).arg(msg));
}

void BaseClient::newFrame(uint i)
{
    emit updateFrame(this->id, i);
}

void BaseClient::handleButtonClick()
{
    if(isConnected()) { mocapStop();  }
    else              { mocapStart(); }
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


bool BaseClient::linkConnector(BaseConnector *c)
{
    bool ok = true;
    ok &= (bool)connect(c, SIGNAL(connecting()),                this, SLOT(UIConnectingState()));
    ok &= (bool)connect(c, SIGNAL(connected()),                 this, SLOT(UIConnectedState()));
    ok &= (bool)connect(c, SIGNAL(disconnecting()),             this, SLOT(UIDisconnectingState()));
    ok &= (bool)connect(c, SIGNAL(disconnected()),              this, SLOT(UIDisconnectedState()));
    ok &= (bool)connect(c, SIGNAL(outMessage(QString)),         this, SLOT(outMessage(QString)));
    ok &= (bool)connect(c, SIGNAL(newFrame(uint)),              this, SLOT(newFrame(uint)));
    ok &= (bool)connect(c, SIGNAL(newFrame(uint)),              this, SLOT(addCount()));
    ok &= (bool)connect(c, SIGNAL(updateSubject(SubjectData*)), this, SLOT(emitUpdateSubject(SubjectData*)));
    return ok;
}

void BaseClient::emitUpdateSubject(SubjectData *d)
{
     emit updateSubject(d);
}
