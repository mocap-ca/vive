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

#ifdef VICON_CLIENT
#include "viconClient.h"
#endif

#ifdef NATURALPOINT_CLIENT
#include "naturalpointClient.h"
#endif


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
    ui->treeViewData->setModel(&subjectList->model);


    for(size_t i=1; i < 16; i++)
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
    viconClient = new ViconClient(subjectList, this);
    QStringList wordList;
    wordList << "192.168.11.1" << "127.0.0.1";
    QCompleter *completer = new QCompleter(wordList, this);
    ui->lineEditHost->setCompleter(completer);
    ui->lineEditHost->setText(wordList[0]);

    ui->lineEditPort->setText("801");
    ok &= (bool)QObject::connect(viconClient, SIGNAL(outMessage(QString)),   this, SLOT(showMessage(QString)));
    ok &= (bool)QObject::connect(ui->pushButtonConnect, SIGNAL(clicked()),   this, SLOT(doViconConnect()));
    ok &= (bool)QObject::connect(viconClient, SIGNAL(connectedEvent(bool)),  this, SLOT(viconConnected(bool)));
    ok &= (bool)QObject::connect(viconClient, SIGNAL(newFrame(uint)),      server, SLOT(process()));
    ok &= (bool)QObject::connect(viconClient, SIGNAL(newFrame(uint)), localServer, SLOT(process()));
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->ViconTab));
#endif

#ifdef NATURALPOINT_CLIENT
    // NaturalPoint Client
    naturalPointClient = new NaturalPointClient(subjectList, this);
    ui->lineEditNPHost->setText(naturalPointClient->host);
    ui->lineEditNPPort->setText(QString::number(naturalPointClient->port));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(outMessage(QString)),   this, SLOT(showMessage(QString)));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(connectedEvent(bool)),  this, SLOT(naturalPointConnected(bool)));
    ok &= (bool)QObject::connect(ui->pushButtonNPConnect, SIGNAL(clicked()),   this, SLOT(doNaturalPointConnect()));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(newFrame(uint)),      server, SLOT(process()));
    ok &= (bool)QObject::connect(naturalPointClient, SIGNAL(newFrame(uint)), localServer, SLOT(process()));
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->NPTab));
#endif

    // Stub Client
    testClient  = new TestClient(subjectList, this);
    ok &= (bool)QObject::connect(testClient,         SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    ok &= (bool)QObject::connect(testClient,         SIGNAL(newFrame(int)),        server, SLOT(process()));
    ok &= (bool)QObject::connect(testClient,         SIGNAL(newFrame(int)),        localServer, SLOT(process()));
    ok &= (bool)QObject::connect(ui->pushButtonStub, SIGNAL(clicked()),            this, SLOT(doStub()));

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

#ifdef VICON_CLIENT
    bool vRunning = viconClient->running;
    if(vRunning) viconClient->running=false;
    if(vRunning) viconClient->wait();
#endif

    bool tRunning = testClient->running;
    if(tRunning) testClient->running=false;
    if(tRunning) testClient->wait();

    delete ui;
}


void MainWindow::doStub()
{
    if(!testClient->running)
    {
        testClient->start();
        ui->pushButtonStub->setText("Stop");
        ui->treeViewData->expandAll();
    }
    else
    {
        testClient->running = false;
        ui->pushButtonStub->setText("Start");
    }



}

void MainWindow::timerClick()
{
    ui->lineEditServerFPS->setText(QString("%1").arg(server->count));
    server->count = 0;
    ui->lineEditLocalFPS->setText(QString("%1").arg(localServer->count));
    localServer->count = 0;

#ifdef VICON_CLIENT
    if(viconClient->running)
    {
        ui->lineEditViconStatus->setText(QString("%1").arg(viconClient->count));
        ui->lineEditViconFPS->setText(QString("%1").arg(viconClient->count));
        viconClient->count = 0;
    }
    else
#endif
    {
        ui->lineEditViconStatus->setText("Not Connected");
    }

#ifdef NATURALPOINT_CLIENT
    if(naturalPointClient->running)
    {
        ui->lineEditNPStatus->setText(QString("%1").arg(naturalPointClient->count));
//        ui->lineEditNPFPS->setText(QString("%1").arg(naturalPointClient->count));
        naturalPointClient->count = 0;
    }
    else
#endif
    {
        ui->lineEditNPStatus->setText("Not Connected");
    }

    if(testClient->running)
    {
        ui->lineEditStubStatus->setText(QString("%1").arg(testClient->count));
        testClient->count = 0;
    }
    else
    {
        ui->lineEditStubStatus->setText(QString(""));
    }

    subjectList->updateModel();

    QString data;
    QTextStream stream(&data);

    stream << *subjectList;
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

#ifdef VICON_CLIENT
void MainWindow::viconConnected(bool con)
{
    if(con)
    {
        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonConnect->setText("Disconnect");
    }
    else
    {
        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonConnect->setText("Connect");
    }
}

void MainWindow::doViconConnect()
{
    if(!viconClient->connected)
    {
        QString host = ui->lineEditHost->text();
        int port = ui->lineEditPort->text().toInt();
        if(port == 0)
        {
            QMessageBox::warning(this,"Error", "Invalid Port", QMessageBox::Ok);
            return;
        }

        // start (connection is handled on other thread as it can be slow)
        viconClient->host = host;
        viconClient->port = port;
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonConnect->setText("Connecting");
        viconClient->start();
    }
    else
    {
        viconClient->running = false;
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonConnect->setText("Disconnecting");
        // Stop the thread from running, this will call disconnect
        viconClient->running = false;

    }
}
#endif // VICON_CLIENT

#ifdef NATURALPOINT_CLIENT

void MainWindow::naturalPointConnected(bool con)
{
    if(con)
    {
        ui->pushButtonNPConnect->setEnabled(true);
        ui->pushButtonNPConnect->setText("Disconnect");
    }
    else
    {
        ui->pushButtonNPConnect->setEnabled(true);
        ui->pushButtonNPConnect->setText("Connect");
    }
}

void MainWindow::doNaturalPointConnect()
{
    if(naturalPointClient->connected == false) {
        QString host = ui->lineEditNPHost->text();

        int port = ui->lineEditNPPort->text().toInt();
        if(port == 0)
        {
            QMessageBox::warning(this,"Error", "Invalid Port", QMessageBox::Ok);
            return;
        }

        naturalPointClient->host = host;
        naturalPointClient->port = port;

        ui->pushButtonNPConnect->setEnabled(false);
        ui->pushButtonNPConnect->setText("Connecting");
        naturalPointClient->start();
    } else {
        naturalPointClient->running = false;
        ui->pushButtonNPConnect->setEnabled(false);
        ui->pushButtonNPConnect->setText("Disconnecting");
    }
}

#endif

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    testClient->mousex = e->x() * 4.0;
    testClient->mousey = e->y() * 4.0;
}

