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

    QStringList wordList;
    wordList << "127.0.0.1" << "192.168.11.1";
    QCompleter *completer = new QCompleter(wordList, this);
    ui->lineEditHost->setCompleter(completer);

    for(size_t i=1; i < 16; i++)
        ui->treeViewData->setColumnWidth(i, 50);


    // Start tcp server
    server = new MyServer(subjectList);
    connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->listen(4001);

    // Start local server
    localServer = new LocalServer(subjectList);
    connect(localServer, SIGNAL(connectionsChanged()), this, SLOT(updateConnectionList()));
    connect(localServer, SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    localServer->listen();

#ifdef VICON_CLIENT
    // Vicon Client
    viconClient = new ViconClient(subjectList, this);
    //ui->lineEditHost->setText("192.168.11.1");
    ui->lineEditHost->setText("127.0.0.1");
    ui->lineEditPort->setText("801");
    connect(viconClient, SIGNAL(outMessage(QString)),   this, SLOT(showMessage(QString)));
    connect(viconClient, SIGNAL(connectedEvent(bool)),  this, SLOT(viconConnected(bool)));
    connect(ui->pushButtonConnect, SIGNAL(clicked()),   this, SLOT(doViconConnect()));
    connect(viconClient, SIGNAL(newFrame(uint)),      server, SLOT(process()));
    connect(viconClient, SIGNAL(newFrame(uint)), localServer, SLOT(process()));
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->ViconTab));
#endif

#ifdef NATURALPOINT_CLIENT
    // NaturalPoint Client
    naturalPointClient = new NaturalPointClient(subjectList, this);
    ui->lineEditNPHost->setText(naturalPointClient->host);
    ui->lineEditNPPort->setText(QString::number(naturalPointClient->port));
    connect(naturalPointClient, SIGNAL(outMessage(QString)),   this, SLOT(showMessage(QString)));
    connect(naturalPointClient, SIGNAL(connectedEvent(bool)),  this, SLOT(naturalPointConnected(bool)));
    connect(ui->pushButtonNPConnect, SIGNAL(clicked()),   this, SLOT(doNaturalPointConnect()));
    connect(naturalPointClient, SIGNAL(newFrame(uint)),      server, SLOT(process()));
    connect(naturalPointClient, SIGNAL(newFrame(uint)), localServer, SLOT(process()));
#else
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->NPTab));
#endif

    // Stub Client
    testClient  = new TestClient(subjectList, this);
    connect(testClient,         SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    connect(testClient,         SIGNAL(newFrame(int)),        server, SLOT(process()));
    connect(testClient,         SIGNAL(newFrame(int)),        localServer, SLOT(process()));
    connect(ui->pushButtonStub, SIGNAL(clicked()),            this, SLOT(doStub()));

    // Window refresh timer
	timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerClick()));
    timer->setInterval(1000);
	timer->start();
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

