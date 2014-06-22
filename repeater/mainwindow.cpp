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
    int defaultPort = 4001;


    // Set up main ui

    ui->setupUi(this);
    setWindowTitle(QString("VIVE Version ") + QString(VIVE_VERSION));
    ui->plainTextEditLog->setMaximumBlockCount(1000);

    QList<int> splitSizes;
    splitSizes << 4;
    splitSizes << 2;
    ui->splitter->setSizes(splitSizes);

    ui->lineEditListenPort->setText( QString("%1").arg(defaultPort));
    ok &= (connect(ui->pushButtonServerToggle, SIGNAL(pressed()), this, SLOT(toggleServer())) != NULL);

    ui->lineEditViveHost->setText(QString("127.0.0.1"));
    ui->lineEditVivePort->setText(QString("4001"));

    // Create the subject list and gui model
	subjectList = new MocapSubjectList(this);
    subjectModel = new MocapModel(this, subjectList);
    ui->treeViewData->setModel(subjectModel);
    //for(int i=1; i < 16; i++)
    //    ui->treeViewData->setColumnWidth(i, 50);

    // Create the connections model
    modelConnections = new QStandardItemModel(this);
    ui->listViewConnections->setModel(modelConnections);


    // Start tcp server
    server = new MyServer(ServerConnection::CON_TCP, defaultPort, subjectList, this);
    ok &= (bool)QObject::connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    ok &= (bool)QObject::connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->listen();

    // Start local server
    localServer = new MyServer(ServerConnection::CON_PIPE, 0, subjectList, this);
    ok &= (bool)QObject::connect(localServer, SIGNAL(connectionsChanged()), this, SLOT(updateConnectionList()));
    ok &= (bool)QObject::connect(localServer, SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    localServer->listen();

    updateServerUI();

#ifdef VICON_CLIENT
    // Vicon Client
    ViconClient *viconClient = new ViconClient(subjectList,
                                               ui->pushButtonConnectVicon,
                                               ui->lineEditViconStatus,
                                               ui->lineEditViconHost,
                                               ui->lineEditViconPort,
                                               ui->checkBoxViconYUp,
                                               this);
    clients.append(viconClient);
#else
//    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->ViconTab));
#endif

#ifdef NATURALPOINT_CLIENT
    // NaturalPoint Client
    NaturalPointClient *naturalPointClient = new NaturalPointClient(subjectList,
                                                                    ui->pushButtonNPConnect,
                                                                    ui->lineEditNPStatus,
                                                                    ui->lineEditNPHost,
                                                                    ui->lineEditNPPort,
                                                                    this);
    clients.append(naturalPointClient);
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->NPTab));
#endif

    // Stub Client
    testClient  = new TestClient(subjectList, ui->pushButtonStub, ui->lineEditStubStatus, this);
    clients.append(testClient);

    // Vive Client
    viveClient = new ViveClient(subjectList,
                                ui->pushButtonViveConnect,
                                ui->lineEditViveStatus,
                                ui->lineEditViveHost,
                                ui->lineEditVivePort,
                                this);
    clients.append(viveClient);

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

void MainWindow::toggleServer()
{
    if(server->isRunning())
    {
        server->stop();
        showMessage("Stopped Server");
    }
    else
    {
        int port = ui->lineEditListenPort->text().toInt();
        if(port == 0)
        {
                QMessageBox::critical(this, "Error", "Invalid Port");
                return;
        }
        server->setPort(port);
        if(server->listen())
        {
            showMessage("Started Server");
        }
    }
    updateServerUI();
}

void MainWindow::updateServerUI()
{
    if(server->isRunning())
    {
        ui->pushButtonServerToggle->setText("Stop");
        ui->lineEditListenPort->setEnabled(false);
    }
    else
    {
        ui->pushButtonServerToggle->setText("Start");
        ui->lineEditListenPort->setEnabled(true);
    }
}


void MainWindow::processFrame(ClientId, uint)
{
    server->process();
    localServer->process();
}



void MainWindow::timerClick()
{
    // Prepare the status bar message
    QStringList msgs;
    msgs.append( QString("FPS:  ") );

    int    serverConnections = server->numberOfConnections();
    size_t serverFps         = server->count;
    ui->lineEditServerFPS->setText(QString("%1").arg(serverFps));
    if(serverConnections > 0) msgs.append( QString("TCP: #%1 @ %2   ").arg(serverConnections).arg(serverFps) );
    server->count = 0;

    int    localConnections  = localServer->numberOfConnections();
    size_t localServerFps    = localServer->count;
    ui->lineEditLocalFPS->setText(QString("%1").arg(localServerFps));
    if(localConnections > 0)  msgs.append( QString("Local: #%1 @ %2   ").arg(localConnections).arg(localServerFps) );
    localServer->count = 0;

    for(QList<BaseClient*>::iterator i = clients.begin(); i!= clients.end(); i++)
    {
        BaseClient *c = *i;
        if(!c->isConnected() || c->count == 0) continue;
        QString name = c->ClientStr();
        size_t count = c->count;
        msgs.append( QString("%1 @ %2").arg(name).arg(count));
    }

    this->statusBar()->showMessage(msgs.join(" - "));

    // Send a tick to each client (resets counters)
    for(QList<BaseClient*>::iterator i = clients.begin(); i != clients.end(); i++)
    {
        (*i)->tick();
    }


    subjectModel->update();

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
    //testClient->mousex = e->x() * 4.0;
    //testClient->mousey = e->y() * 4.0;
}

