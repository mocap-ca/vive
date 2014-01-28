#ifndef MOCAP_SUBJECT
#define MOCAP_SUBJECT

#include <QTextStream>

#include <QObject>
#include <QList>
#include <QMutex>

class MocapSegmentList;
class MocapSubjectList;

// A Segment has name, translation and rotation
class MocapSegment
{
	friend class MocapSegmentList;
private:
	std::string name;
	double translation[3];
	double rotation[3];
    friend QTextStream& operator << ( QTextStream&, MocapSegment& );

};

// A list of segments
class MocapSegmentList : public QObject
{
	Q_OBJECT

public:
	MocapSegmentList(QMutex &mutex, QObject *parent = NULL);

	void set(std::string name,
		double trans[3],
		double rot[3]);

    friend QTextStream& operator << ( QTextStream&, MocapSegmentList& );

private:
	QList<MocapSegment> items;
	QMutex &mutex;
};

// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	friend class MocapSubjectList;

	Q_OBJECT
public:
	MocapSubject(QString name, QMutex &mutex, QObject *parent);	

    friend QTextStream& operator << ( QTextStream&, MocapSubject& );

	MocapSegmentList segments;

private :
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

protected:
	QMutex subjectMutex;

private:
	QList<MocapSubject*> items;
};


#endif
