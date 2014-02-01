#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>
#include "server.h"
#include "workthread.h"
#include "viconClient.h"
#include "testClient.h"

namespace Ui {
class MainWin;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public slots:
    void updateConnectionList(void);
    void showMessage(QString);
    void doConnect(void);
	void timerClick(void);
    void doStub(void);
    void viconConnected(bool);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWin *ui;
    MyServer    *server;
    WorkThread  *worker;
	QTimer      *timer;
    QStandardItemModel *modelConnections;

	ViconClient *viconClient;
	TestClient  *testClient;

	QMutex           subectMutex;
	MocapSubjectList *subjectList;
};

#endif // MAINWINDOW_H
