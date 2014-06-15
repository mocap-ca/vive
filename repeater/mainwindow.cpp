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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QCompleter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWin )
{

    bool ok = true;

    ui->setupUi(this);

    setWindowTitle(QString("VIVE Version ") + QString(VIVE_VERSION));

    ui->plainTextEditLog->setMaximumBlockCount(1000);

    QList<int> splitSizes;
    splitSizes << 4;
    splitSizes << 2;
    ui->splitter->setSizes(splitSizes);

	subjectList = new MocapSubjectList(this);
    modelConnections = new QStandardItemModel(this);
    ui->listViewConnections->setModel(modelConnections);
    //ui->treeViewData->setModel(&subjectList->model);

    for(int i=1; i < 16; i++)
        ui->treeViewData->setColumnWidth(i, 50);

    // Start tcp server
    server = new MyServer(subjectList);
    ok &= (bool)QObject::connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    ok &= (bool)QObject::connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->listen(4001);

    // Start local server
    localServer = new LocalServer(subjectList);
    ok &= (bool)QObject::connect(localServer, SIGNAL(connectionsChanged()), this, SLOT(updateConnectionList()));
    ok &= (bool)QObject::connect(localServer, SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    localServer->listen();

#ifdef VICON_CLIENT
    // Vicon Client
    ViconClient *viconClient = new ViconClient(subjectList,
                                               ui->pushButtonConnectVicon,
                                               ui->lineEditViconStatus,
                                               ui->lineEditViconHost,
                                               ui->lineEditViconPort,
                                               this);
    clients.append(viconClient);
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->ViconTab));
#endif

#ifdef NATURALPOINT_CLIENT
    // NaturalPoint Client
    NaturalPointClient *naturalPointClient = new NaturalPointClient(subjectList,
                                                                    ui->pushButtonNPConnect,
                                                                    ui->lineEditNPStatus,
                                                                    ui->lineEditNPHost,
                                                                    ui->lineEditNPPort,
                                                                    this);
//    ui->lineEditNPHost->setText(naturalPointClient->host);
//    ui->lineEditNPPort->setText(QString::number(naturalPointClient->port));
//    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(outMessage(QString)),   this, SLOT(showMessage(QString)));
//    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(connectedEvent(bool)),  this, SLOT(naturalPointConnected(bool)));
//    ok &= (bool)QObject::connect(ui->pushButtonNPConnect, SIGNAL(clicked()),   this, SLOT(doNaturalPointConnect()));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(newFrame(uint)),      server, SLOT(process()));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(newFrame(uint)), localServer, SLOT(process()));
    clients.append(naturalPointClient);
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->NPTab));
#endif

    // Stub Client
    testClient  = new TestClient(subjectList,
                                 ui->pushButtonStub,
                                 ui->lineEditStubStatus,
                                 this);
    clients.append(testClient);

    // Window refresh timer
	timer = new QTimer(this);
    ok &= (bool)QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerClick()));
    timer->setInterval(1000);
	timer->start();

#ifdef _DEBUG
    if(!ok) showMessage("There was an initialization error");
#endif
}

MainWindow::~MainWindow()
{
    server->stop();
    localServer->stop();

    // Stop all clients
    for( QList<BaseClient *>::iterator i = clients.begin(); i!=clients.end(); i++)
    {
        (*i)->mocapStop();
    }

    // Wait for all clients to stop
    for( QList<BaseClient *>::iterator i = clients.begin(); i!=clients.end(); i++)
    {
        (*i)->mocapWait();
    }

    delete ui;
}


void MainWindow::processFrame(BaseClient::ClientId, uint)
{
    server->process();
    localServer->process();
}



void MainWindow::timerClick()
{
    QString msg = QString("TCP FPS: %1   Pipe FPS: %2").arg(server->count).arg(localServer->count);
    this->statusBar()->showMessage(msg);

    for(QList<BaseClient*>::iterator i = clients.begin(); i != clients.end(); i++)
    {
        (*i)->tick();
    }


    //subjectList->updateModel();

    QString data;
    QTextStream stream(&data);

    subjectList->read(stream, false);
    ui->textEditData->setText(data);
}

// the connection list has changed, update the model/display
void MainWindow::updateConnectionList(void)
{
    QList<QString> connections;
    modelConnections->clear();
    server->getConnectionList(connections);
    localServer->getConnectionList(connections);
    for(QList<QString>::iterator i = connections.begin(); i != connections.end(); i++)
        modelConnections->appendRow( new QStandardItem( *i ));
}

// append some text to the log window
void MainWindow::showMessage(QString msg)
{
    ui->plainTextEditLog->appendPlainText(msg);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    testClient->mousex = e->x() * 4.0;
    testClient->mousey = e->y() * 4.0;
}

