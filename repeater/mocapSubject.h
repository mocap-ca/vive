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
    MocapSegment(QString name, double tr[3], double ro[3]);
    QString name;
    QStandardItem *modelItem;
	double translation[3];
	double rotation[3];
    friend QTextStream& operator << ( QTextStream&, MocapSegment& );
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

    void set(QString name, double trans[3], double rot[3]);

    void updateModel();

private :
    QList<MocapSegment> segments;
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
