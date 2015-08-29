#ifndef VIVE_MOCAP_DATA
#define VIVE_MOCAP_DATA

#include <stdint.h>

#ifdef VIVESTL
#include <sstream>
#include <vector>
#include <string>

class DataStream
{
public:
	DataStream();
	DataStream(const char *, size_t);

    std::string  str();
    DataStream&  readBytes(char*, size_t);
    DataStream&  writeBytes(const char *, size_t);
    bool         good();

    std::vector<char> data;
    int    state;
	size_t ptr;

};

#define DSTREAM DataStream
#define TSTREAM std::stringstream
#define STRING std::string
#define LIST   std::vector
#define PAIR   std::pair
#define FLOAT  float
#define uint   unsigned int


#else

#include <QTextStream>
#include <QDataStream>
#include <QString>
#include <QList>

#define DSTREAM QDataStream
#define TSTREAM QTextStream
#define STRING  QString
#define LIST    QList
#define FLOAT   float
#define PAIR    QPair


#endif

DSTREAM & operator<= (DSTREAM& stream, std::string const& s);
DSTREAM & operator<= (DSTREAM& stream, const char *s);
DSTREAM & operator<= (DSTREAM& stream, int16_t const& i);
DSTREAM & operator<= (DSTREAM& stream, int32_t const& i);
DSTREAM & operator<= (DSTREAM& stream, uint16_t const& i);
DSTREAM & operator<= (DSTREAM& stream, uint32_t const& i);
DSTREAM & operator<= (DSTREAM& stream, float const& f);

DSTREAM & operator>= (DSTREAM& stream, std::string &s);
DSTREAM & operator>= (DSTREAM& stream, float &f);
DSTREAM & operator>= (DSTREAM& stream, int16_t &i);

bool streamOk(DSTREAM&);
bool streamOk(TSTREAM&);

DSTREAM& setBad(DSTREAM&);


#define MARKER_TYPE 0
#define SEGMENT_TYPE 1

// Mocap Item

class MocapItem
{
public:
	MocapItem(int type, STRING name);
    MocapItem( int type, int id );
    MocapItem( int type);
    virtual MocapItem *copy() = 0;

    int id;
    int type;
    STRING name;
	bool isLocal;

    bool operator == (const MocapItem&) const;

    virtual void update(const MocapItem *other) = 0;

    virtual void frame( DSTREAM& ) = 0;
    virtual void header( DSTREAM& );
    virtual void parseFrame( DSTREAM& ) = 0;
    virtual void parseHeader( DSTREAM& );
    virtual void str( TSTREAM& ) = 0;

    friend TSTREAM& operator << ( TSTREAM &, MocapItem& );

};

TSTREAM& operator <= (TSTREAM &stream, MocapItem& );
TSTREAM& operator >= (TSTREAM &stream, MocapItem&);

typedef PAIR<int, MocapItem*> IDItemType;
typedef LIST<IDItemType> IDListType;

class IDList
{
public:
    static IDList* Instance();
    int lastId;

    IDListType items;

    //! obtain a new id for the mocap item.  Sets the id on the item if it is -1
    int getId( MocapItem*);
    MocapItem* find(int);

	bool parseHeader(DSTREAM&); // reads all the header data
	bool parseFrame(DSTREAM&);  // updates a single id.

	bool formatHeader(DSTREAM&, bool localOnly);  // pushes the raw header
	bool formatAll(DSTREAM&);  // pushes all frames
	bool parseAll(DSTREAM&);

	bool str(TSTREAM&, bool localOnly);

private:
    IDList() {}
    IDList(IDList const&){}
    IDList& operator=(IDList const&){}
    static IDList *inst;

};



class ItemList
{
public:
    LIST< MocapItem* > data;

    int update(const ItemList&);
    void append( MocapItem *);
};



// Mocap Marker

class MocapMarker : public MocapItem
{
public:
    MocapMarker( STRING name, FLOAT tr[3]);
    MocapMarker( int id );
    MocapMarker();
    MocapItem* copy();
    FLOAT translation[3];
    void update(const MocapItem* other);

    void frame( DSTREAM& );
    void header( DSTREAM& );
    void parseFrame( DSTREAM& );
    void parseHeader( DSTREAM& );
    void str( TSTREAM& );

};

inline bool operator == (const MocapMarker &a, const MocapMarker &b);




// Mocap Segment


class MocapSegment : public MocapItem
{
public:
    MocapSegment( const STRING n, const FLOAT tr[3], const FLOAT rot[4]);
    MocapSegment( int id );
    MocapSegment();
    MocapItem* copy();
	FLOAT translation[3];
	FLOAT rotation[4];
    void update( const MocapItem *other );

    void frame( DSTREAM& );
    void header( DSTREAM& );
    void parseFrame( DSTREAM& );
    void parseHeader( DSTREAM& );
    void str( TSTREAM& );

};

//OSTREAM& operator << ( OSTREAM& stream, MocapSegment &segment );
DSTREAM& operator << ( DSTREAM& stream, MocapSegment &segment );
DSTREAM& operator >> ( DSTREAM& stream, MocapSegment &segment );


bool operator == (const MocapSegment &a, const MocapSegment &b);









#endif
