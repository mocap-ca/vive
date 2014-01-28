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

    QStandardItemModel *items;


public slots:
    void updateConnectionList(void);
    void showMessage(QString);
    void setFrameRate(int);
    void doConnect(void);
	void timerClick(void);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWin *ui;
    MyServer    *server;
    WorkThread  *worker;
	QTimer      *timer;

	ViconClient *viconClient;
	TestClient  *testClient;

	QMutex           subectMutex;
	MocapSubjectList *subjectList;
};

#endif // MAINWINDOW_H
