#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWin )
{
    ui->setupUi(this);

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

    connect(ui->lineEditServerDelay, SIGNAL(textChanged(QString)), this, SLOT(updateServerDelay(QString)));


    // Start tcp server
    server = new MyServer(subjectList);
    connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->listen(1234);


    // Vicon Client
    viconClient = new ViconClient(subjectList, this);
    //ui->lineEditHost->setText("192.168.11.1");
    ui->lineEditHost->setText("127.0.0.1");
    ui->lineEditPort->setText("801");
    connect(viconClient, SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    connect(viconClient, SIGNAL(connectedEvent(bool)), this, SLOT(viconConnected(bool)));
    connect(ui->pushButtonConnect, SIGNAL(clicked()),  this, SLOT(doConnect()));
    connect(viconClient, SIGNAL(newFrame(uint)),       server, SLOT(process()));

    // Stub Client
    testClient  = new TestClient(subjectList, this);
    connect(testClient,  SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    connect(ui->pushButtonStub, SIGNAL(clicked()), this, SLOT(doStub()));


    // Window refresh timer
	timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerClick()));
    timer->setInterval(1000);
	timer->start();
}

MainWindow::~MainWindow()
{
    server->stop();

    bool vRunning = viconClient->running;
    bool tRunning = testClient->running;

    if(vRunning) viconClient->running=false;
    if(tRunning) testClient->running=false;

    if(vRunning) viconClient->wait();
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

    if(viconClient->running)
    {
        ui->lineEditViconStatus->setText(QString("%1").arg(viconClient->count));
        ui->lineEditViconFPS->setText(QString("%1").arg(viconClient->count));
        viconClient->count = 0;
    }
    else
    {
        ui->lineEditViconStatus->setText("Not Connected");
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
    for(QList<QString>::iterator i = connections.begin(); i != connections.end(); i++)
        modelConnections->appendRow( new QStandardItem( *i ));
}


// append some text to the log window
void MainWindow::showMessage(QString msg)
{
    ui->plainTextEditLog->appendPlainText(msg);
}

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

void MainWindow::doConnect()
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

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    testClient->tx = e->x() * 4.0;
    testClient->tz = e->y() * 4.0;
}

void MainWindow::updateServerDelay(QString val)
{
    bool ok;
    int ival = val.toInt(&ok);
    if(!ok) ival = 100;
    if(ival < 1) ival = 100;
    server->setInterval(ival);
}
