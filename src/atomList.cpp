#include "atomList.h"



/***********************
*     Id List 
**********************/

IDList* IDList::inst = NULL;

IDList* IDList::Instance()
{
	if (inst) return inst;
	inst = new IDList();
	return inst;
}

int IDList::getId(MocapItem *item)
{
	if (item->id != -1) return item->id;

	int maxid = 0;
	for (LIST< PAIR< int, MocapItem* > >::iterator i = items.begin(); i != items.end(); i++)
	{
		int        thisId = (*i).first;
		MocapItem *thisItem = (*i).second;
		if (*item == *thisItem)
		{
			item->id = thisItem->id;
			delete thisItem;
			(*i).second = item;
			return thisId;
		}
		if (thisId > maxid) maxid = thisId;
	}

	maxid++;
	item->id = maxid;
#ifdef VIVEQT
	items.append(IDItemType(maxid, item));
#else
	items.push_back(IDItemType(maxid, item));
#endif
	return maxid;
}

MocapItem* IDList::find(int id)
{
	for (LIST< PAIR< int, MocapItem* > >::const_iterator i = items.begin(); i != items.end(); i++)
	{
		if ((*i).first == id) return (*i).second;
	}

	return NULL;
}

void IDList::setData(MocapItem **itemList, size_t len)
{
	items.clear();
	for (size_t i = 0; i < len; i++)
	{
		MocapItem *thisItem = itemList[i];
		items.push_back( IDItemType( thisItem->id, thisItem ) );
	}
}



/*


// parse a raw frame, updating the item in the list
bool IDList::parseFrame(DSTREAM &stream)
{
	int16_t id;
	stream >= id;

	if (!streamOk(stream)) return false;

	MocapItem *item = find(id);
	if (item == NULL) return false;

	item->parseFrame(stream);

	return streamOk(stream);
}

bool IDList::parseAll(DSTREAM &stream)
{
	int16_t len;
	stream >= len;

	for (int16_t i = 0; i < len; i++)
	{
		if (!parseFrame(stream)) return false;
	}
	return true;
}




// Push all the header data on to the stream
bool IDList::formatHeader(DSTREAM &stream, bool localOnly)
{
	stream <= (int16_t)0x01FA;
	stream <= (int16_t)items.size();

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		MocapItem *item = (*i).second;
		if (localOnly && !item->isLocal) continue;
		item->header(stream);
	}
	return streamOk(stream);
}

bool IDList::formatAll(DSTREAM& stream, bool localOnly)
{
	stream <= (int16_t)items.size();

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		MocapItem *item = (*i).second;
		if (localOnly && !item->isLocal) continue;
		item->frame(stream);
	}

	return streamOk(stream);

}


// Push all the frame data on to the text stream, for debugging.
bool IDList::str(TSTREAM &stream, bool localOnly)
{
	stream << items.size() << "\n";

	for (IDListType::iterator i = items.begin(); i != items.end(); i++)
	{
		if (localOnly && !(*i).second->isLocal) continue;
		stream << *((*i).second);
	}

	return streamOk(stream);
}
*/