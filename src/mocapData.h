#ifndef VIVE_MOCAP_DATA
#define VIVE_MOCAP_DATA


#define MARKER_TYPE 0
#define SEGMENT_TYPE 1

// Mocap Item

// The base class for a single mocap item, e.g. a marker or a segment.

class MocapItem
{
public:
	MocapItem( int type, const char* name);  // live mocap data
    MocapItem( int type, int id, const char *name ); // data copy
    MocapItem( int type ); // blank instance
	~MocapItem(); // frees name
	bool operator == (const MocapItem&) const;

    int id;
    int type;
    char* name;

};


// Mocap Marker

class MocapMarker : public MocapItem
{
public:
    MocapMarker( const char* name, float tr[3]);  // incoming live mocap data
    MocapMarker( int id, const char* name );  // used by the decoder
    MocapMarker();
	float translation[3];
};




// Mocap Segment
class MocapSegment : public MocapItem
{
public:
    MocapSegment( const char* name, const float tr[3], const float rot[4]);  // incoming mocap data
    MocapSegment( int id, const char* name ); // used by decoder
    MocapSegment();
	float translation[3];
	float rotation[4];
};



#endif
