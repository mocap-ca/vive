#ifndef VIVE_ENCODER_H
#define VIVE_ENCODER_H

#include "mocapdata.h"
#include "defines.h"
#include "dataStream.h"

void encodeHeaders(MocapMarker**markers, size_t markerCount, MocapSegment **segments, size_t segmentCount, DSTREAM& stream);
void encodeMarker(MocapMarker*, DSTREAM&);
void encodeSegment(MocapSegment, DSTREAM&);
void encodeMarker(MocapMarker *, TSTREAM&);
void encodeSegment(MocapSegment *, TSTREAM&);



#endif