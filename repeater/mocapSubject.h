#ifndef MOCAP_SUBJECT
#define MOCAP_SUBJECT

#include <QObject>
#include <QList>

class MocapSegment
{
public:
	std::string name;
	double translation[3];
	double rotation[3];
	template<typename T>
	T& operator << ( T& );
};

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

class MocapSubjectList : public QObject
{
	Q_OBJECT
public:
	MocapSubjectList(QObject *parent = NULL);

	MocapSubject* find(QString name, bool add=true);

	template<typename T>
	T& operator << ( T& );

	QList<MocapSubject*> items;
};


#endif