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
