#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWin )
{
    ui->setupUi(this);

	subjectList = new MocapSubjectList(this);

	viconClient = new ViconClient(subjectList, this);
    connect(viconClient, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));

	testClient  = new TestClient(subjectList, this);
	connect(testClient,  SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));


    items = new QStandardItemModel;
    ui->listView->setModel(items);

    // Start tcp server
    server = new MyServer(subjectList, this);
    connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->start();

/*    worker = new WorkThread(this, server);
    connect(worker, SIGNAL(outMessage(QString)), this,   SLOT(showMessage(QString)));
    connect(worker, SIGNAL(outFrameRate(int)),   this,   SLOT(setFrameRate(int)));
	connect(worker, SIGNAL(signalFrame()),       server, SLOT(doFrame()));
    worker->start();
	*/

    ui->lineEditHost->setText("localhost");
    ui->lineEditPort->setText("8811");
    connect(ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(doConnect()));

	viconClient->start();
	testClient->start();

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerClick()));
	timer->setInterval(1000);
	timer->start();
}

void MainWindow::timerClick()
{
	ui->lineEditStats->setText(QString("%1").arg(testClient->count));
	testClient->count = 0;
}

// the connection list has changed, update the model/display
void MainWindow::updateConnectionList(void)
{
    QList<QString> connections;
    items->clear();
    server->getConnectionList(connections);
    for(QList<QString>::iterator i = connections.begin(); i != connections.end(); i++)
        items->appendRow( new QStandardItem( *i ));
}

// update the display with a new frame rate value
void MainWindow::setFrameRate(int rate)
{
    QString msg("%1");
    msg = msg.arg(rate);
    ui->lineEditStats->setText(msg);
}

// append some text to the log window
void MainWindow::showMessage(QString msg)
{
    ui->textEditLog->append(msg);
}

void MainWindow::doConnect()
{
    showMessage("CONNECT");
    if(!viconClient->connected)
    {
        QString host = ui->lineEditHost->text();
        int port = ui->lineEditPort->text().toInt();
        if(port == 0)
        {
            QMessageBox::warning(this,"Error", "Invalid Port", QMessageBox::Ok);
            return;
        }
        viconClient->mocapConnect(host, port);

    }
    else
    {
        viconClient->mocapDisconnect();
    }

    if(viconClient->connected)
    {
        ui->pushButtonConnect->setText("Disconnect");
    }
    else
    {
        ui->pushButtonConnect->setText("Connect");
    }
}

MainWindow::~MainWindow()
{
	viconClient->running=false;
	testClient->running=false;
	viconClient->wait();
	testClient->wait();
    //worker->stopWorking();
    //worker->wait(3000);
    delete ui;
    //delete items;
}
