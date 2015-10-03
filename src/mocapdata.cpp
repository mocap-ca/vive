#include "mocapData.h"
#include <iostream>
#include <stdlib.h>

/**********************
 *     Mocap Item     *
 **********************/

MocapItem::MocapItem(int type, int id, const char* name_)
 : id(id)
 , type(type)
 , name(NULL)
{
	size_t len = strlen(name_) + 1;
	name = (char*)malloc(len);
	strcpy_s(this->name, len, name_);
}

MocapItem::MocapItem(int type, const char* name_)
	: id(-1)
	, type(type)
{
	size_t len = strlen(name_) + 1;
	name = (char*)malloc(len);
	strcpy_s(this->name, len, name_);
}

MocapItem::~MocapItem()
{
	if (name != NULL) free(name);
}

MocapItem::MocapItem(int type)
    : id(-1)
    , type(type)
	, name(NULL)
{}

bool MocapItem::operator==(const MocapItem& other) const
{
    if(other.type != type) return false;

    if(other.id != -1 && id != -1)
        return other.id == id && other.type == type;

	return strcmp(other.name, name) == 0;
}




/**********************
*   Mocap Marker
**********************/

MocapMarker::MocapMarker(const char* name, float tr[3])
	: MocapItem(MARKER_TYPE, name)
{
	for (int i = 0; i < 3; i++) translation[i] = tr[i];
}

MocapMarker::MocapMarker(int id, const char* name)
	: MocapItem(MARKER_TYPE, id, name)
{
	for (int i = 0; i < 3; i++) translation[i] = 0.f;
}

MocapMarker::MocapMarker()
	: MocapItem(MARKER_TYPE)
{
	for (int i = 0; i < 3; i++) translation[i] = 0.f;
}


inline bool operator == (const MocapMarker &a, const MocapMarker &b)
{
	return a.name == b.name;
}




/**********************
*    Mocap Segment
*********************/

MocapSegment::MocapSegment()
	: MocapItem(SEGMENT_TYPE)
{
	for (int i = 0; i < 3; i++) translation[i] = 0.f;
	for (int i = 0; i < 4; i++) rotation[i] = 0.f;
}

MocapSegment::MocapSegment(int id, const char* name)
	: MocapItem(SEGMENT_TYPE, id, name)
{
	for (int i = 0; i < 3; i++) translation[i] = 0.f;
	for (int i = 0; i < 4; i++) rotation[i] = 0.f;
}


MocapSegment::MocapSegment(const char* name, const float tr[3], const float rot[4])
	: MocapItem(SEGMENT_TYPE, name)
{
	for (int i = 0; i < 3; i++) translation[i] = tr[i];
	for (int i = 0; i < 4; i++) rotation[i] = rot[i];
}





