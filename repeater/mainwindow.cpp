#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWin )
{
    ui->setupUi(this);

    QList<int> splitSizes;
    splitSizes << 4;
    splitSizes << 2;
    ui->splitter->setSizes(splitSizes);

	subjectList = new MocapSubjectList(this);
    modelConnections = new QStandardItemModel(this);
    ui->listViewConnections->setModel(modelConnections);
    ui->treeViewData->setModel(&subjectList->model);

    ui->treeViewData->setColumnWidth(1, 50);
    ui->treeViewData->setColumnWidth(2, 50);
    ui->treeViewData->setColumnWidth(3, 50);
    ui->treeViewData->setColumnWidth(4, 50);
    ui->treeViewData->setColumnWidth(5, 50);
    ui->treeViewData->setColumnWidth(6, 50);


    // Start tcp server
    server = new MyServer(subjectList, this);
    connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->listen(1234);
    server->start();

    // Vicon Client
    viconClient = new ViconClient(subjectList, this);
    ui->lineEditHost->setText("192.168.11.1");
    ui->lineEditPort->setText("801");
    connect(viconClient, SIGNAL(outMessage(QString)),  this, SLOT(showMessage(QString)));
    connect(viconClient, SIGNAL(connectedEvent(bool)), this, SLOT(viconConnected(bool)));
    connect(ui->pushButtonConnect, SIGNAL(clicked()),  this, SLOT(doConnect()));

    // Stub Client
    testClient  = new TestClient(subjectList, this);
    connect(testClient,  SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    connect(ui->pushButtonStub, SIGNAL(clicked()), this, SLOT(doStub()));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerClick()));
	timer->setInterval(1000);
	timer->start();
}

MainWindow::~MainWindow()
{
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
    if(viconClient->running)
    {
        ui->lineEditViconStatus->setText(QString("%1").arg(viconClient->count));
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
    ui->textEditLog->append(msg);
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
        viconClient->start();
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonConnect->setText("Connecting");
    }
    else
    {
        viconClient->mocapDisconnect();
        viconClient->running = false;
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonConnect->setText("Disconnecting");

    }
}

