#ifndef VIVE_DECODER_H
#define VIVE_DECODER_H

#include "mocapdata.h"
#include "defines.h"
#include "dataStream.h"


#include <map>

int decodeHeaders(DSTREAM &stream, MocapItem***itemList, size_t& itemCount);
int decodeFrameStream(DSTREAM &stream);
int decodeFrame(DSTREAM &stream);


#endif