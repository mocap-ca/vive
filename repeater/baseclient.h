#ifndef BASECLIENT_H
#define BASECLIENT_H

#include <QObject>
#include <QThread>
#include <QPushButton>
#include <QLineEdit>
#include "mocapSubject.h"

class BaseClient : public QObject
{


    Q_OBJECT
public:

    typedef enum eClientId
    {
        CL_None = 0,
        CL_Stub = 1,
        CL_Vicon = 100,
        CL_NaturalPoint
    } ClientId;


    explicit BaseClient(ClientId id,
                        MocapSubjectList *subjectList,
                        QPushButton*,
                        QLineEdit*,
                        QObject *parent);

    //! Reference to the global subject list for all data
    /*! This clients should append any incoming data to this structure
     *  then emit a newFrame signal to tell the server to pass on the data */
    MocapSubjectList *subjects;

    //! Push button that shows connection/disconnection for this client
    QPushButton *button;

    //! Line edit to display the status
    QLineEdit *statusLine;


    //! True if currently connected
    bool connected;

    //! Frame counter
    size_t count;

    //! Id of this client.  Should be unique per subclass.
    ClientId id;

    //! @returns a string of the client id, e.g. "Vicon".
    virtual QString ClientStr() = 0;

    //! signals a message to be displayed
    void outMessage(QString s);

    //! signal that a new frame is available and should be pushed out
    void newFrame(uint i);

public slots:

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

    //! @returns true if the service is currently running
    virtual bool isRunning() = 0;

signals:
    void stateConnecting();
    void stateConnected();
    void stateDisconnecting();
    void stateDisconnected();
    void outMessage_(QString);
    void updateFrame(BaseClient::ClientId, uint);

};

#endif // BASECLIENT_H
