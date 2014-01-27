#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
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


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWin *ui;
    MyServer    *server;
    WorkThread  *worker;

	ViconClient *viconClient;
	TestClient  *testClient;
	MocapSubjectList *subjectList;
};

#endif // MAINWINDOW_H
