#ifndef MOCAP_SUBJECT
#define MOCAP_SUBJECT

#include <QTextStream>

#include <QObject>
#include <QList>
#include <QMutex>
#include <QStandardItemModel>
#include <QStandardItem>

class MocapSegmentList;
class MocapSubjectList;

// A Segment has name, translation and rotation
class MocapSegment
{
    friend class MocapSubject;
private:
    MocapSegment(QString name, double tr[3], double rot[4]);
    QString name;
    QList<QStandardItem*> modelItems;
	double translation[3];
    double localRotation[4];
    friend QTextStream& operator << ( QTextStream&, MocapSegment& );
    void updateModel();
};

class MocapMarker
{
    friend class MocapSubject;
private:
    MocapMarker(QString name, double tr[3]);
    QString name;
    QList<QStandardItem*> modelItems;
    double translation[3];
    friend QTextStream& operator << (QTextStream&, MocapMarker& );
    void updateModel();
};


// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	friend class MocapSubjectList;

	Q_OBJECT
public:
	MocapSubject(QString name, QMutex &mutex, QObject *parent);	

    friend QTextStream& operator << ( QTextStream&, MocapSubject& );

    void setSegment(QString name, double trans[3], double rot[4]);

    void setMarker(QString name, double trans[3]);

    void updateModel();

private :
    QList<MocapSegment> segments;
    QList<MocapMarker> markers;
    QStandardItem *modelItem;
	QString name;
	QMutex &mutex;
};

// A list of mocap subjects
class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);
    ~MocapSubjectList();
	MocapSubject* find(QString name, bool add=true);
    friend QTextStream& operator << ( QTextStream&, MocapSubjectList& );
    QStandardItemModel model;

public slots:
    void updateModel();

protected:
	QMutex subjectMutex;

private:
	QList<MocapSubject*> items;
};


#endif
