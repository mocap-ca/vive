//
//  CMNatNetPacketParser.cpp
//  PacketClient
//
//  Created by Aaron Hilton on 2014-05-12.
//  Copyright (c) 2014 Conquer Mobile. All rights reserved.
//

#include "CMNatNetPacketParser.h"
#include <string.h>
#include <assert.h>

#define strlcpy strncpy

CMNatNetPacketParser::CMNatNetPacketParser():
    m_iFrame(0)
{
    for(int i=0; i<2; i++) {
        zeroFrame(&m_frame[i]);
    }
}

CMNatNetPacketParser::~CMNatNetPacketParser() {
    for(int i=0; i<2; i++) {
        deallocFrame(&m_frame[i]);
    }
}

#pragma mark - Data Parsing

bool DecodeTimecode(unsigned int inTimecode, unsigned int inTimecodeSubframe, int* hour, int* minute, int* second, int* frame, int* subframe)
{
	bool bValid = true;
    
	*hour = (inTimecode>>24)&255;
	*minute = (inTimecode>>16)&255;
	*second = (inTimecode>>8)&255;
	*frame = inTimecode&255;
	*subframe = inTimecodeSubframe;
    
	return bValid;
}

bool TimecodeStringify(unsigned int inTimecode, unsigned int inTimecodeSubframe, char *Buffer, int BufferSize)
{
	bool bValid;
	int hour, minute, second, frame, subframe;
	bValid = DecodeTimecode(inTimecode, inTimecodeSubframe, &hour, &minute, &second, &frame, &subframe);
    
    snprintf(Buffer,BufferSize,"%2d:%2d:%2d:%2d.%d",hour, minute, second, frame, subframe);
	for(unsigned int i=0; i<strlen(Buffer); i++)
		if(Buffer[i]==' ')
			Buffer[i]='0';
    
	return bValid;
}

sFrameOfMocapData *CMNatNetPacketParser::parse(char *packet, size_t /*length*/) {

    int major = 2; //NatNetVersion[0];
    int minor = 3; //NatNetVersion[1];
    
    char *ptr = packet;
    
    //printf("Begin Packet\n-------\n");
    
    // message ID
    int MessageID = 0;
    memcpy(&MessageID, ptr, 2); ptr += 2;
    //printf("Message ID : %d\n", MessageID);
    
    // size
    int nBytes = 0;
    memcpy(&nBytes, ptr, 2); ptr += 2;
    //printf("Byte count : %d\n", nBytes);
    
    if(MessageID == 7)      // FRAME OF MOCAP DATA packet
    {
        sFrameOfMocapData *frame = nextFrameBuffer();

        // frame number
        frame->iFrame = 0; memcpy(&frame->iFrame, ptr, 4); ptr += 4;
        //printf("Frame # : %d\n", frame->iFrame);
        
        // number of data sets (markersets, rigidbodies, etc)
        frame->nMarkerSets = 0; memcpy(&frame->nMarkerSets, ptr, 4); ptr += 4;
        //printf("Marker Set Count : %d\n", frame->nMarkerSets);
        
        for (int i=0; i < frame->nMarkerSets; i++)
        {
            sMarkerSetData *markerSet = &frame->MocapData[i];
            
            // Markerset name
            strlcpy(markerSet->szName, ptr, sizeof(markerSet->szName));
            int nDataBytes = (int) strlen(markerSet->szName) + 1;
            ptr += nDataBytes;
            //printf("Model Name: %s\n", markerSet->szName);
            
            // marker data
            markerSet->nMarkers = 0; memcpy(&markerSet->nMarkers, ptr, 4); ptr += 4;
            //printf("Marker Count : %d\n", markerSet->nMarkers);
            assert( markerSet->nMarkers>=0 && markerSet->nMarkers < MAX_MARKERS);
            markerSet->Markers = (MarkerData*)malloc(4*3*markerSet->nMarkers);
            for(int j=0; j < markerSet->nMarkers; j++)
            {
                MarkerData &marker = markerSet->Markers[j];
                marker[0] = 0; memcpy(&marker[0], ptr, 4); ptr += 4;
                marker[1] = 0; memcpy(&marker[1], ptr, 4); ptr += 4;
                marker[2] = 0; memcpy(&marker[2], ptr, 4); ptr += 4;
                //printf("\tMarker %d : [x=%3.2f,y=%3.2f,z=%3.2f]\n",j,marker[0],marker[1],marker[2]);
            }
        }
        
        // unidentified markers
        frame->nOtherMarkers = 0; memcpy(&frame->nOtherMarkers, ptr, 4); ptr += 4;
        //printf("Unidentified Marker Count : %d\n", frame->nOtherMarkers);
        assert( frame->nOtherMarkers>=0 && frame->nOtherMarkers < MAX_MARKERS);
        frame->OtherMarkers = (MarkerData*)malloc(4*3*frame->nOtherMarkers);
        for(int j=0; j < frame->nOtherMarkers; j++)
        {
            MarkerData &marker = frame->OtherMarkers[j];
            marker[0] = 0; memcpy(&marker[0], ptr, 4); ptr += 4;
            marker[1] = 0; memcpy(&marker[1], ptr, 4); ptr += 4;
            marker[2] = 0; memcpy(&marker[2], ptr, 4); ptr += 4;
            //printf("\tMarker %d : pos = [%3.2f,%3.2f,%3.2f]\n",j,marker[0],marker[1],marker[2]);
        }
        
        // rigid bodies
        frame->nRigidBodies = 0;
        memcpy(&frame->nRigidBodies, ptr, 4); ptr += 4;
        //printf("Rigid Body Count : %d\n", frame->nRigidBodies);
        for (int j=0; j < frame->nRigidBodies; j++)
        {
            sRigidBodyData *rb = &frame->RigidBodies[j];
            // rigid body pos/ori
            rb->ID = 0; memcpy(&rb->ID, ptr, 4); ptr += 4;
            rb->x = 0.0f; memcpy(&rb->x, ptr, 4); ptr += 4;
            rb->y = 0.0f; memcpy(&rb->y, ptr, 4); ptr += 4;
            rb->z = 0.0f; memcpy(&rb->z, ptr, 4); ptr += 4;
            rb->qx = 0; memcpy(&rb->qx, ptr, 4); ptr += 4;
            rb->qy = 0; memcpy(&rb->qy, ptr, 4); ptr += 4;
            rb->qz = 0; memcpy(&rb->qz, ptr, 4); ptr += 4;
            rb->qw = 0; memcpy(&rb->qw, ptr, 4); ptr += 4;
            //printf("ID : %d\n", rb->ID);
            //printf("pos: [%3.2f,%3.2f,%3.2f]\n", rb->x,rb->y,rb->z);
            //printf("ori: [%3.2f,%3.2f,%3.2f,%3.2f]\n", rb->qx,rb->qy,rb->qz,rb->qw);
            
            // associated marker positions
            rb->nMarkers = 0;  memcpy(&rb->nMarkers, ptr, 4); ptr += 4;
            //printf("Marker Count: %d\n", rb->nMarkers);
            assert(rb->nMarkers>=0 && rb->nMarkers<MAX_RBMARKERS);
            int nBytes = rb->nMarkers*3*sizeof(float);
            rb->Markers = (MarkerData*)malloc(nBytes);
            memcpy(rb->Markers, ptr, nBytes);
            ptr += nBytes;
            
            if(major >= 2)
            {
                // associated marker IDs
                nBytes = rb->nMarkers*sizeof(int32_t);
                rb->MarkerIDs = (int*)malloc(nBytes);
                memcpy(rb->MarkerIDs, ptr, nBytes);
                ptr += nBytes;
                
                // associated marker sizes
                nBytes = rb->nMarkers*sizeof(float);
                rb->MarkerSizes = (float*)malloc(nBytes);
                memcpy(rb->MarkerSizes, ptr, nBytes);
                ptr += nBytes;
                
//                for(int k=0; k < rb->nMarkers; k++)
//                {
//                    printf("\tMarker %d: id=%d\tsize=%3.1f\tpos=[%3.2f,%3.2f,%3.2f]\n",
//                           k,
//                           rb->MarkerIDs[k], rb->MarkerSizes[k], rb->Markers[k][0], rb->Markers[k][1],rb->Markers[k][2]);
//                }
                
//                if(markerIDs)
//                    free(markerIDs);
//                if(markerSizes)
//                    free(markerSizes);
//                
            }
            else
            {
//                for(int k=0; k < rb->nMarkers; k++)
//                {
//                    printf("\tMarker %d: pos = [%3.2f,%3.2f,%3.2f]\n",
//                           k,
//                           rb->Markers[k][0], rb->Markers[k][1],rb->Markers[k][2]);
//                }
            }
//            if(markerData)
//                free(markerData);
            
            if(major >= 2)
            {
                // Mean marker error
                rb->MeanError = 0.0f; memcpy(&rb->MeanError, ptr, 4); ptr += 4;
                //printf("Mean marker error: %3.2f\n", rb->MeanError);
            }
        } // next rigid body
        
        
        // skeletons (version 2.1 and later)
        if( ((major == 2)&&(minor>0)) || (major>2))
        {
            int nSkeletons = 0;
            memcpy(&nSkeletons, ptr, 4); ptr += 4;
            //printf("Skeleton Count : %d\n", nSkeletons);
            for (int j=0; j < nSkeletons; j++)
            {
                // skeleton id
                int skeletonID = 0;
                memcpy(&skeletonID, ptr, 4); ptr += 4;
                // # of rigid bodies (bones) in skeleton
                int nRigidBodies = 0;
                memcpy(&nRigidBodies, ptr, 4); ptr += 4;
                //printf("Rigid Body Count : %d\n", nRigidBodies);
                for (int j=0; j < nRigidBodies; j++)
                {
                    // rigid body pos/ori
                    int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;
                    float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
                    float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
                    float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;
                    float qx = 0; memcpy(&qx, ptr, 4); ptr += 4;
                    float qy = 0; memcpy(&qy, ptr, 4); ptr += 4;
                    float qz = 0; memcpy(&qz, ptr, 4); ptr += 4;
                    float qw = 0; memcpy(&qw, ptr, 4); ptr += 4;
                    //printf("ID : %d\n", ID);
                    //printf("pos: [%3.2f,%3.2f,%3.2f]\n", x,y,z);
                    //printf("ori: [%3.2f,%3.2f,%3.2f,%3.2f]\n", qx,qy,qz,qw);
                    
                    // associated marker positions
                    int nRigidMarkers = 0;  memcpy(&nRigidMarkers, ptr, 4); ptr += 4;
                    //printf("Marker Count: %d\n", nRigidMarkers);
                    int nBytes = nRigidMarkers*3*sizeof(float);
                    float* markerData = (float*)malloc(nBytes);
                    memcpy(markerData, ptr, nBytes);
                    ptr += nBytes;
                    
                    // associated marker IDs
                    nBytes = nRigidMarkers*sizeof(int);
                    int* markerIDs = (int*)malloc(nBytes);
                    memcpy(markerIDs, ptr, nBytes);
                    ptr += nBytes;
                    
                    // associated marker sizes
                    nBytes = nRigidMarkers*sizeof(float);
                    float* markerSizes = (float*)malloc(nBytes);
                    memcpy(markerSizes, ptr, nBytes);
                    ptr += nBytes;
                    
                    for(int k=0; k < nRigidMarkers; k++)
                    {
                        //printf("\tMarker %d: id=%d\tsize=%3.1f\tpos=[%3.2f,%3.2f,%3.2f]\n", k, markerIDs[k], markerSizes[k], markerData[k*3], markerData[k*3+1],markerData[k*3+2]);
                    }
                    
                    // Mean marker error
                    float fError = 0.0f; memcpy(&fError, ptr, 4); ptr += 4;
                    //printf("Mean marker error: %3.2f\n", fError);
                    
                    // release resources
                    if(markerIDs)
                        free(markerIDs);
                    if(markerSizes)
                        free(markerSizes);
                    if(markerData)
                        free(markerData);
                    
                } // next rigid body
                
            } // next skeleton
        }
        
        // labeled markers (version 2.3 and later)
        if( ((major == 2)&&(minor>=3)) || (major>2))
        {
            int nLabeledMarkers = 0;
            memcpy(&nLabeledMarkers, ptr, 4); ptr += 4;
            //printf("Labeled Marker Count : %d\n", nLabeledMarkers);
            for (int j=0; j < nLabeledMarkers; j++)
            {
                // id
                int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;
                // x
                float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
                // y
                float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
                // z
                float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;
                // size
                float size = 0.0f; memcpy(&size, ptr, 4); ptr += 4;
                
                //printf("ID  : %d\n", ID);
                //printf("pos : [%3.2f,%3.2f,%3.2f]\n", x,y,z);
                //printf("size: [%3.2f]\n", size);
            }
        }
        
        // latency
        float latency = 0.0f; memcpy(&latency, ptr, 4);	ptr += 4;
        
        // timecode
        uint32_t timecode = 0; 	memcpy(&timecode, ptr, 4);	ptr += 4;
        uint32_t timecodeSub = 0; memcpy(&timecodeSub, ptr, 4); ptr += 4;
        char szTimecode[128] = "";
        TimecodeStringify(timecode, timecodeSub, szTimecode, 128);

        //printf("time: %s, latency : %3.3f\n", szTimecode, latency);

        // end of data tag
        int eod = 0; memcpy(&eod, ptr, 4); ptr += 4;
        //printf("End Packet\n-------------\n");
        return frame;
    }
    else if(MessageID == 5) // Data Descriptions
    {
        // number of datasets
        int nDatasets = 0; memcpy(&nDatasets, ptr, 4); ptr += 4;
        //printf("Dataset Count : %d\n", nDatasets);
        
        for(int i=0; i < nDatasets; i++)
        {
            //printf("Dataset %d\n", i);
            
            int type = 0; memcpy(&type, ptr, 4); ptr += 4;
            //printf("Type : %d\n", type);
            
            if(type == 0)   // markerset
            {
                // name
                char szName[256];
                strlcpy(szName, ptr, sizeof(szName));
                int nDataBytes = (int) strlen(szName) + 1;
                ptr += nDataBytes;
                //printf("Markerset Name: %s\n", szName);
                
                // marker data
                int nMarkers = 0; memcpy(&nMarkers, ptr, 4); ptr += 4;
                //printf("Marker Count : %d\n", nMarkers);
                
                for(int j=0; j < nMarkers; j++)
                {
                    char szName[256];
                    strlcpy(szName, ptr, sizeof(szName));
                    int nDataBytes = (int) strlen(szName) + 1;
                    ptr += nDataBytes;
                    //printf("Marker Name: %s\n", szName);
                }
            }
            else if(type ==1)   // rigid body
            {
                if(major >= 2)
                {
                    // name
                    char szName[MAX_NAMELENGTH];
                    strcpy(szName, ptr);
                    ptr += strlen(ptr) + 1;
                    //printf("Name: %s\n", szName);
                }
                
                int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                //printf("ID : %d\n", ID);
                
                int parentID = 0; memcpy(&parentID, ptr, 4); ptr +=4;
                //printf("Parent ID : %d\n", parentID);
                
                float xoffset = 0; memcpy(&xoffset, ptr, 4); ptr +=4;
                //printf("X Offset : %3.2f\n", xoffset);
                
                float yoffset = 0; memcpy(&yoffset, ptr, 4); ptr +=4;
                //printf("Y Offset : %3.2f\n", yoffset);
                
                float zoffset = 0; memcpy(&zoffset, ptr, 4); ptr +=4;
                //printf("Z Offset : %3.2f\n", zoffset);
                
            }
            else if(type ==2)   // skeleton
            {
                char szName[MAX_NAMELENGTH];
                strcpy(szName, ptr);
                ptr += strlen(ptr) + 1;
                //printf("Name: %s\n", szName);
                
                int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                //printf("ID : %d\n", ID);
                
                int nRigidBodies = 0; memcpy(&nRigidBodies, ptr, 4); ptr +=4;
                //printf("RigidBody (Bone) Count : %d\n", nRigidBodies);
                
                for(int i=0; i< nRigidBodies; i++)
                {
                    if(major >= 2)
                    {
                        // RB name
                        char szName[MAX_NAMELENGTH];
                        strcpy(szName, ptr);
                        ptr += strlen(ptr) + 1;
                        //printf("Rigid Body Name: %s\n", szName);
                    }
                    
                    int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                    //printf("RigidBody ID : %d\n", ID);
                    
                    int parentID = 0; memcpy(&parentID, ptr, 4); ptr +=4;
                    //printf("Parent ID : %d\n", parentID);
                    
                    float xoffset = 0; memcpy(&xoffset, ptr, 4); ptr +=4;
                    //printf("X Offset : %3.2f\n", xoffset);
                    
                    float yoffset = 0; memcpy(&yoffset, ptr, 4); ptr +=4;
                    //printf("Y Offset : %3.2f\n", yoffset);
                    
                    float zoffset = 0; memcpy(&zoffset, ptr, 4); ptr +=4;
                    //printf("Z Offset : %3.2f\n", zoffset);
                }
            }
            
        }   // next dataset
        
        //printf("End Packet\n-------------\n");
        return NULL;
    }
    else
    {
        //printf("Unrecognized Packet Type.\n");
        return NULL;
    }
}

#pragma mark - Frame Data Management

// Just zero the data structs.
void CMNatNetPacketParser::zeroFrame(sFrameOfMocapData *frame)
{
    memset(frame, 0, sizeof(sFrameOfMocapData));
}

// Look through data structs, free and zero them.
void CMNatNetPacketParser::deallocFrame(sFrameOfMocapData *frame)
{
    int i;
    for(i=0;i<frame->nMarkerSets; i++) {
        free(frame->MocapData[i].Markers);
        frame->MocapData[i].Markers = NULL;
    }
    
    if(frame->OtherMarkers) {
        free(frame->OtherMarkers);
        frame->OtherMarkers = NULL;
    }
    
    for(i=0; i < frame->nRigidBodies; i++)
    {
        sRigidBodyData *rb = &frame->RigidBodies[i];
        if( rb->Markers ) {
            free(rb->Markers);
            rb->Markers = NULL;
        }
        
        if( rb->MarkerIDs ) {
            free(rb->MarkerIDs);
            rb->MarkerIDs = NULL;
        }
        
        if( rb->MarkerSizes ) {
            free(rb->MarkerSizes);
            rb->MarkerSizes = NULL;
        }
    }
    
    zeroFrame(frame);
}


// Make sure the frame is prepped for usage in the parser.
sFrameOfMocapData *CMNatNetPacketParser::nextFrameBuffer() {
    sFrameOfMocapData *frame = &m_frame[m_iFrame];
    m_iFrame=(m_iFrame+1)&1;
    deallocFrame(frame); // Clear any old data.
    return frame;
}
