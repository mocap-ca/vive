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

#ifndef VICONCLIENT_H
#define VICONCLIENT_H

#include <QObject>
#include <QList>
#include "baseClient.h"
#include "ViconStreamClient.h"
#include <QLineEdit>
#include <QCheckBox>

// Inherits from qthread - cannot modify gui
class ViconConnector : public BaseConnector
{
    Q_OBJECT

public:

    enum eStreamMode
    {
        SERVER_PUSH,
        CLIENT_PULL,
        CLIENT_PULL_PRE_FETCH
    };

    ViconConnector(QObject *parent);

    virtual void run();
    virtual void stop();

    bool connect();

    bool    running;
    QString host;
    int     port;
    size_t  count;

    ViconDataStreamSDK::CPP::Client mClient;
    eStreamMode   streamMode;

    bool  yUp;
    void reorientPos(const double vicon[3],  double unityTrans[3]);
    void reorientRot(const double vicon[4],  double unityRot[4]);

};


// Inherits from qobject - runs under main thread.
class ViconClient : public BaseClient
{
	Q_OBJECT

public:
    ViconClient( QPushButton *button,
                 QLineEdit *statusLine,
                 QLineEdit *hostField,
                 QLineEdit *portField,
                 QCheckBox *checkBoxYUp,
                 QObject *parent = NULL);



    //! @returns "Vicon"
    virtual QString ClientStr() { return QString("Vicon"); }

    //! True if thread loop is currently running
    /*! Set to false just before thread loop completes */
    bool running;

    //! see BaseClient::mocapStart()
    virtual void mocapStart();

    //! see BaseClient::mocapStop()
    virtual void mocapStop();

    //! see BaseClient::mocapWait();
    virtual void mocapWait();

    //! @returns true if the service is running
    virtual bool isRunning() { return running; }

    //! @return true if we are connected
    virtual bool isConnected();

    ViconConnector *vicon;
    QLineEdit *hostField;
    QLineEdit *portField;
    QCheckBox *checkBoxYUp;


private:
    bool          frameError;


public slots:
    void changeYUp(bool);

};



#endif
