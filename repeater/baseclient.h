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

#ifndef BASECLIENT_H
#define BASECLIENT_H

#include <QObject>
#include <QThread>
#include <QPushButton>
#include <QLineEdit>
#include "mocapSubject.h"
#include "defines.h"

class BaseConnector : public QThread
{
    Q_OBJECT

public:
    BaseConnector(QObject *parent);

    //virtual bool connect() = 0;
    virtual void stop()    = 0;

signals:
    void connecting();
    void connected();
    void disconnecting();
    void disconnected();
    void newFrame(uint);
    void outMessage(QString);
    void updateSubject(SubjectData *);

};

class BaseClient : public QObject
{
    Q_OBJECT
public:

    explicit BaseClient(ClientId id,
                        QPushButton*,
                        QLineEdit*,
                        QObject *parent);

    //! Push button that shows connection/disconnection for this client
    QPushButton *button;

    //! Line edit to display the status
    QLineEdit *statusLine;

    //! Frame counter
    size_t count;

    //! Id of this client.  Should be unique per subclass.
    ClientId id;

    //! @returns a string of the client id, e.g. "Vicon".
    virtual QString ClientStr() = 0;

    //! @returns true if client is currently connected
    virtual bool isConnected() = 0;

    //! links base connector signals
    bool linkConnector(BaseConnector *);

public slots:
    //! signals a message to be displayed
    void outMessage(QString);

    //! signal that a new frame is available and should be pushed out
    void newFrame(uint i);

    //! Subclass should prepare then start the thread/timer
    virtual void mocapStart() = 0;

    //! Subclass this to cleanly break out of the thread loop
    virtual void mocapStop() = 0;

    //! Subclass should wait for threads to stop and return
    virtual void mocapWait() = 0;

    //! Change ui to show we are connected
    /*! This must be called from the main window only */
    virtual void UIConnectingState();

    //! Change ui to show we are disconnecting
    /*! This must be called from the main window only */
    virtual void UIDisconnectingState();

    //! Change ui to show that we are connected
    /*! This must be called from the main window only */
    virtual void UIConnectedState();

    //! Change ui to show that we are disconnected
    /*! This must be called from the main window only */
    virtual void UIDisconnectedState();

    //! Called when the button is clicked
    void handleButtonClick();

    //! Called once a second by the main thread
    void tick();

    //! Adds one to the fps counter
    void addCount();

    //! pass on the subect
    void emitUpdateSubject(SubjectData *d);


signals:
    void stateConnecting();
    void stateConnected();
    void stateDisconnecting();
    void stateDisconnected();
    void outMessage_(QString);
    void updateFrame(ClientId, uint);
    void updateSubject(SubjectData*);  //!< Update a mocap subject in the model
};

#endif // BASECLIENT_H
