#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWin )
{
    ui->setupUi(this);

    items = new QStandardItemModel;
    ui->listView->setModel(items);

    // Start server last
    server = new MyServer(this);
    connect(server, SIGNAL(connectionsChanged()),    this, SLOT(updateConnectionList()));
    connect(server, SIGNAL(outMessage(QString)),     this, SLOT(showMessage(QString)));
    server->start();

    worker = new WorkThread(this, server);
    connect(worker, SIGNAL(outMessage(QString)), this,   SLOT(showMessage(QString)));
    connect(worker, SIGNAL(outFrameRate(int)),   this,   SLOT(setFrameRate(int)));
	connect(worker, SIGNAL(signalFrame()),       server, SLOT(doFrame()));
    worker->start();
}

void MainWindow::updateConnectionList(void)
{
    QList<QString> connections;
    items->clear();
    server->getConnectionList(connections);
    for(QList<QString>::iterator i = connections.begin(); i != connections.end(); i++)
        items->appendRow( new QStandardItem( *i ));

}

void MainWindow::setFrameRate(int rate)
{
    QString msg("%1");
    msg = msg.arg(rate);
    ui->lineEditStats->setText(msg);
}

void MainWindow::showMessage(QString msg)
{
    ui->textEditLog->append(msg);
}

MainWindow::~MainWindow()
{
    worker->stopWorking();
    worker->wait(3000);
    delete ui;
    delete items;
}
