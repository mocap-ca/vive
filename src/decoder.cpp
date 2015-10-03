#include "decoder.h"
#include "atomList.h"

#include <vector>
#include <map>

int decodeHeaders(DSTREAM &stream, MocapItem ***itemList, size_t& itemCount)
{
	size_t pos = 0;

	int16_t header1, header2, count, id;
	STRING type, name;

	stream >= header1;
	if (header1 != (int16_t)0xFA01) return -1;

	stream >= header2;
	stream >= count;

	if (!streamOk(stream)) return -2;

	std::vector<MocapItem*> items;

	for (int16_t i = 0; i < count; i++)
	{
		stream >= id;
		stream >= type;
		stream >= name;

		MocapItem *item = NULL;
		if (type == "MARKER")  item = new MocapMarker(id, name.c_str());
		if (type == "SEGMENT") item = new MocapSegment(id, name.c_str());
		if (item != NULL) items.push_back(item);
	}

	// hack
	itemCount = items.size();
	*itemList = (MocapItem**)malloc(sizeof(void*) * items.size());
	memcpy(*itemList, &items[0], sizeof(void*)*items.size());

	return (int)header2;
}

int decodeFrameStream(DSTREAM &stream)
{
	int16_t count;
	stream >= count;
	for (int16_t i = 0; i < count; i++)
	{
		decodeFrame(stream);
	}

	return 0;

}

int decodeFrame(DSTREAM &stream)
{
	/*
	int16_t id;
	stream >= id;

	if (!streamOk(stream)) return -1;

	IDList *list = IDList::Instance();

	MocapItem *item = list->find(id);
	if (item == NULL) return -2;

	if (item->type == MARKER_TYPE)
	{
		MocapMarker *marker = dynamic_cast<MocapMarker*>(item);
		stream >= marker->translation[0];
		stream >= marker->translation[1];
		stream >= marker->translation[2];
	}

	if (item->type == SEGMENT_TYPE)
	{
		MocapSegment *segment = dynamic_cast<MocapSegment*>(item);
		for (size_t i = 0; i < 3; i++) stream >= segment->translation[i];
		for (size_t i = 0; i < 4; i++) stream >= segment->rotation[i];
	}
	return 0;*/
	return 0;

}

