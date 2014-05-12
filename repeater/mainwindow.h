#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>
#include "server.h"
#include "workthread.h"
#include "testClient.h"
#include "localServer.h"

#ifdef VICON_CLIENT
class ViconClient;
#endif

#ifdef NATURALPOINT_CLIENT
class NaturalPointClient;
#endif


// 0.2 - Added local (named pipe) server

#define VIVE_VERSION "0.2"


namespace Ui {
class MainWin;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public slots:
    void updateConnectionList(void);
    void showMessage(QString);
	void timerClick(void);
    void doStub(void);

#ifdef VICON_CLIENT
    void doViconConnect(void);
    void viconConnected(bool);
#endif

#ifdef NATURALPOINT_CLIENT
    void doNaturalPointConnect(void);
    void naturalPointConnected(bool);
#endif

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void mouseMoveEvent(QMouseEvent*);
    Ui::MainWin *ui;
    MyServer    *server;
    LocalServer *localServer;
    WorkThread  *worker;
	QTimer      *timer;
    QStandardItemModel *modelConnections;

#ifdef VICON_CLIENT
    ViconClient *viconClient;
#endif

#ifdef NATURALPOINT_CLIENT
    NaturalPointClient *naturalPointClient;
#endif

    TestClient  *testClient;

	QMutex           subectMutex;
	MocapSubjectList *subjectList;
};

#endif // MAINWINDOW_H
