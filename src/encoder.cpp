#include "encoder.h"


void encodeHeaders(MocapMarker**markers, size_t markerCount, MocapSegment **segments, size_t segmentCount, DSTREAM& stream)
{
	stream <= (int16_t)0xFA01;
	stream <= (int16_t)0x0001;
	stream <= (int16_t)(markerCount + segmentCount);
	for (size_t i = 0; i < markerCount; i++)
	{
		stream <= (int16_t)markers[i]->id;
		stream <= STRING("MARKER");
		stream <= STRING(markers[i]->name);
	}
	for (size_t i = 0; i < segmentCount; i++)
	{
		stream <= (int16_t)segments[i]->id;
		stream <= STRING("SEGMENT");
		stream <= STRING(segments[i]->name);
	}

}



void encodeSegment(MocapSegment *segment, DSTREAM& stream)
{
	stream <= (int16_t)segment->id;
	for (size_t i = 0; i < 3; i++) stream <= segment->translation[i];
	for (size_t i = 0; i < 4; i++) stream <= segment->rotation[i];
}

void encodeMarker(MocapMarker *marker, DSTREAM& stream)
{
	stream <= (int16_t)marker->id;
	stream <= marker->translation[0];
	stream <= marker->translation[1];
	stream <= marker->translation[2];
}

void encodeSegment(MocapSegment *segment, TSTREAM& stream)
{
	stream << "S> " << segment->name << " ";
	for (size_t i = 0; i < 3; i++) stream << segment->translation[i] << " ";
	for (size_t i = 0; i < 4; i++)
	{
		stream << segment->rotation[i];
		if (i < 3) stream << " ";
	}
	stream << "\n";
}

void encodeMarker(MocapMarker *marker, TSTREAM& stream)
{
	stream << "M> " << marker->name << " ";
	stream << marker->translation[0] << " ";
	stream << marker->translation[1] << " ";
	stream << marker->translation[2] << "\n";
}