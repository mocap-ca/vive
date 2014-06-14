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
#include "testClient.h"
#include "localServer.h"

// Stub any classes that are not required. They will be
// set to null and ignored.
#ifdef VICON_CLIENT
#include "viconClient.h"
#endif

#ifdef NATURALPOINT_CLIENT
#include "naturalpointClient.h"
#endif


// 0.2 - Added local (named pipe) server
// 0.3 - Added base class for clients
// 0.4 - Baseclient inherits from QObject now instead of QThread
//     - Fixed stub and broken connection

#define VIVE_VERSION "0.4"


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
    void processFrame(BaseClient::ClientId, uint);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void mouseMoveEvent(QMouseEvent*);
    Ui::MainWin *ui;
    MyServer    *server;
    LocalServer *localServer;
	QTimer      *timer;
    QStandardItemModel *modelConnections;

    TestClient  *testClient;

    //! List of supported clients
    QList<BaseClient *> clients;

    //! Mutex to allow access to the subjectList
	QMutex           subectMutex;

    //! Contains all subject information for the current frame.
	MocapSubjectList *subjectList;
};

#endif // MAINWINDOW_H
