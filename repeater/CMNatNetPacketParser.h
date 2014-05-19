//
//  CMNatNetPacketParser.h
//  PacketClient
//
//  Created by Aaron Hilton on 2014-05-12.
//  Copyright (c) 2014 Conquer Mobile. All rights reserved.
//

#ifndef __PacketClient__CMNatNetPacketParser__
#define __PacketClient__CMNatNetPacketParser__

#include <iostream>
#include "NatNetTypes.h"

class CMNatNetPacketParser {
public:
    CMNatNetPacketParser();
    ~CMNatNetPacketParser();

    sFrameOfMocapData *parse(char *packet, size_t length);
private:
    void zeroFrame(sFrameOfMocapData *frame);  // Just zero the data structs.
    void deallocFrame(sFrameOfMocapData *frame); // Look through data structs, free and zero them.
    
    // TODO: NOT THREADSAFE (yet)
    // Double-buffer frame data, so threaded parsing of data can work together.
    int m_iFrame;
    sFrameOfMocapData m_frame[2];
    sFrameOfMocapData *nextFrameBuffer();
};

#endif /* defined(__PacketClient__CMNatNetPacketParser__) */
