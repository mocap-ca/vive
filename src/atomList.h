#ifndef ATOM_LIST_H
#define ATOM_LIST_H

#include "mocapData.h"
#include "defines.h"
#include "dataStream.h"

typedef PAIR<int, MocapItem*> IDItemType;  // Map id -> item
typedef LIST<IDItemType> IDListType; // list of id => items


class IDList
{
public:
	static IDList* Instance();
	int lastId;

	IDListType items;

	//! obtain a new id for the mocap item.  Sets the id on the item if it is -1
	int getId(MocapItem*);
	void setData(MocapItem **items, size_t len);
	MocapItem *find(int);

	bool str(TSTREAM&, bool localOnly);

private:
	IDList() {}
	IDList(IDList const&){}
	IDList& operator=(IDList const&){}
	static IDList *inst;

};

#endif