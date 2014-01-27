#ifndef MOCAP_SUBJECT
#define MOCAP_SUBJECT

#include <QTextStream>

#include <QObject>
#include <QList>

// A Segment has name, translation and rotation
class MocapSegment
{
public:
	std::string name;
	double translation[3];
	double rotation[3];
	template<typename T>
	T& operator << ( T& );
};

// A list of segments
class MocapSegmentList : public QObject
{
	Q_OBJECT
public:
	MocapSegmentList(QObject *parent = NULL);

	void set(std::string name,
		double trans[3],
		double rot[3]);

	template<typename T>
	T& operator << ( T& );

	QList<MocapSegment> items;
};

// A mocap subject - has a name and a segment list
class MocapSubject : public QObject
{
	Q_OBJECT
public:
	MocapSubject(QString name, QObject *parent);

	template<typename T>
	T& operator << ( T& );

	QString name;
	MocapSegmentList segments;
};

// A list of mocap subjects
class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);

	MocapSubject* find(QString name, bool add=true);

	friend QTextStream& operator << ( QTextStream&, MocapSubjectList& );

	QList<MocapSubject*> items;
};


#endif