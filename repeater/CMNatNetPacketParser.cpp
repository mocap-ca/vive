//
//  CMNatNetPacketParser.cpp
//  PacketClient
//
//  Created by Aaron Hilton on 2014-05-12.
//  Copyright (c) 2014 Conquer Mobile. All rights reserved.
//

#include "CMNatNetPacketParser.h"
#include <string.h>

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

sFrameOfMocapData *CMNatNetPacketParser::parse(char *packet, size_t length) {
    sFrameOfMocapData *frame = nextFrameBuffer();

    int major = 2; //NatNetVersion[0];
    int minor = 3; //NatNetVersion[1];
    
    char *ptr = packet;
    
    printf("Begin Packet\n-------\n");
    
    // message ID
    int MessageID = 0;
    memcpy(&MessageID, ptr, 2); ptr += 2;
    printf("Message ID : %d\n", MessageID);
    
    // size
    int nBytes = 0;
    memcpy(&nBytes, ptr, 2); ptr += 2;
    printf("Byte count : %d\n", nBytes);
    
    if(MessageID == 7)      // FRAME OF MOCAP DATA packet
    {
        // frame number
        int frameNumber = 0; memcpy(&frameNumber, ptr, 4); ptr += 4;
        printf("Frame # : %d\n", frameNumber);
        
        // number of data sets (markersets, rigidbodies, etc)
        int nMarkerSets = 0; memcpy(&nMarkerSets, ptr, 4); ptr += 4;
        printf("Marker Set Count : %d\n", nMarkerSets);
        
        for (int i=0; i < nMarkerSets; i++)
        {
            // Markerset name
            char szName[256];
            strlcpy(szName, ptr, sizeof(szName));
            int nDataBytes = (int) strlen(szName) + 1;
            ptr += nDataBytes;
            printf("Model Name: %s\n", szName);
            
            // marker data
            int nMarkers = 0; memcpy(&nMarkers, ptr, 4); ptr += 4;
            printf("Marker Count : %d\n", nMarkers);
            
            for(int j=0; j < nMarkers; j++)
            {
                float x = 0; memcpy(&x, ptr, 4); ptr += 4;
                float y = 0; memcpy(&y, ptr, 4); ptr += 4;
                float z = 0; memcpy(&z, ptr, 4); ptr += 4;
                printf("\tMarker %d : [x=%3.2f,y=%3.2f,z=%3.2f]\n",j,x,y,z);
            }
        }
        
        // unidentified markers
        int nOtherMarkers = 0; memcpy(&nOtherMarkers, ptr, 4); ptr += 4;
        printf("Unidentified Marker Count : %d\n", nOtherMarkers);
        for(int j=0; j < nOtherMarkers; j++)
        {
            float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
            float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
            float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;
            printf("\tMarker %d : pos = [%3.2f,%3.2f,%3.2f]\n",j,x,y,z);
        }
        
        // rigid bodies
        int nRigidBodies = 0;
        memcpy(&nRigidBodies, ptr, 4); ptr += 4;
        printf("Rigid Body Count : %d\n", nRigidBodies);
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
            printf("ID : %d\n", ID);
            printf("pos: [%3.2f,%3.2f,%3.2f]\n", x,y,z);
            printf("ori: [%3.2f,%3.2f,%3.2f,%3.2f]\n", qx,qy,qz,qw);
            
            // associated marker positions
            int nRigidMarkers = 0;  memcpy(&nRigidMarkers, ptr, 4); ptr += 4;
            printf("Marker Count: %d\n", nRigidMarkers);
            int nBytes = nRigidMarkers*3*sizeof(float);
            float* markerData = (float*)malloc(nBytes);
            memcpy(markerData, ptr, nBytes);
            ptr += nBytes;
            
            if(major >= 2)
            {
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
                    printf("\tMarker %d: id=%d\tsize=%3.1f\tpos=[%3.2f,%3.2f,%3.2f]\n", k, markerIDs[k], markerSizes[k], markerData[k*3], markerData[k*3+1],markerData[k*3+2]);
                }
                
                if(markerIDs)
                    free(markerIDs);
                if(markerSizes)
                    free(markerSizes);
                
            }
            else
            {
                for(int k=0; k < nRigidMarkers; k++)
                {
                    printf("\tMarker %d: pos = [%3.2f,%3.2f,%3.2f]\n", k, markerData[k*3], markerData[k*3+1],markerData[k*3+2]);
                }
            }
            if(markerData)
                free(markerData);
            
            if(major >= 2)
            {
                // Mean marker error
                float fError = 0.0f; memcpy(&fError, ptr, 4); ptr += 4;
                printf("Mean marker error: %3.2f\n", fError);
            }
            
            
        } // next rigid body
        
        
        // skeletons (version 2.1 and later)
        if( ((major == 2)&&(minor>0)) || (major>2))
        {
            int nSkeletons = 0;
            memcpy(&nSkeletons, ptr, 4); ptr += 4;
            printf("Skeleton Count : %d\n", nSkeletons);
            for (int j=0; j < nSkeletons; j++)
            {
                // skeleton id
                int skeletonID = 0;
                memcpy(&skeletonID, ptr, 4); ptr += 4;
                // # of rigid bodies (bones) in skeleton
                int nRigidBodies = 0;
                memcpy(&nRigidBodies, ptr, 4); ptr += 4;
                printf("Rigid Body Count : %d\n", nRigidBodies);
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
                    printf("ID : %d\n", ID);
                    printf("pos: [%3.2f,%3.2f,%3.2f]\n", x,y,z);
                    printf("ori: [%3.2f,%3.2f,%3.2f,%3.2f]\n", qx,qy,qz,qw);
                    
                    // associated marker positions
                    int nRigidMarkers = 0;  memcpy(&nRigidMarkers, ptr, 4); ptr += 4;
                    printf("Marker Count: %d\n", nRigidMarkers);
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
                        printf("\tMarker %d: id=%d\tsize=%3.1f\tpos=[%3.2f,%3.2f,%3.2f]\n", k, markerIDs[k], markerSizes[k], markerData[k*3], markerData[k*3+1],markerData[k*3+2]);
                    }
                    
                    // Mean marker error
                    float fError = 0.0f; memcpy(&fError, ptr, 4); ptr += 4;
                    printf("Mean marker error: %3.2f\n", fError);
                    
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
            printf("Labeled Marker Count : %d\n", nLabeledMarkers);
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
                
                printf("ID  : %d\n", ID);
                printf("pos : [%3.2f,%3.2f,%3.2f]\n", x,y,z);
                printf("size: [%3.2f]\n", size);
            }
        }
        
        // latency
        float latency = 0.0f; memcpy(&latency, ptr, 4);	ptr += 4;
        printf("latency : %3.3f\n", latency);
        
        // timecode
        unsigned int timecode = 0; 	memcpy(&timecode, ptr, 4);	ptr += 4;
        unsigned int timecodeSub = 0; memcpy(&timecodeSub, ptr, 4); ptr += 4;
        char szTimecode[128] = "";
        TimecodeStringify(timecode, timecodeSub, szTimecode, 128);
        
        // end of data tag
        int eod = 0; memcpy(&eod, ptr, 4); ptr += 4;
        printf("End Packet\n-------------\n");
        
    }
    else if(MessageID == 5) // Data Descriptions
    {
        // number of datasets
        int nDatasets = 0; memcpy(&nDatasets, ptr, 4); ptr += 4;
        printf("Dataset Count : %d\n", nDatasets);
        
        for(int i=0; i < nDatasets; i++)
        {
            printf("Dataset %d\n", i);
            
            int type = 0; memcpy(&type, ptr, 4); ptr += 4;
            printf("Type : %d\n", type);
            
            if(type == 0)   // markerset
            {
                // name
                char szName[256];
                strlcpy(szName, ptr, sizeof(szName));
                int nDataBytes = (int) strlen(szName) + 1;
                ptr += nDataBytes;
                printf("Markerset Name: %s\n", szName);
                
                // marker data
                int nMarkers = 0; memcpy(&nMarkers, ptr, 4); ptr += 4;
                printf("Marker Count : %d\n", nMarkers);
                
                for(int j=0; j < nMarkers; j++)
                {
                    char szName[256];
                    strlcpy(szName, ptr, sizeof(szName));
                    int nDataBytes = (int) strlen(szName) + 1;
                    ptr += nDataBytes;
                    printf("Marker Name: %s\n", szName);
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
                    printf("Name: %s\n", szName);
                }
                
                int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                printf("ID : %d\n", ID);
                
                int parentID = 0; memcpy(&parentID, ptr, 4); ptr +=4;
                printf("Parent ID : %d\n", parentID);
                
                float xoffset = 0; memcpy(&xoffset, ptr, 4); ptr +=4;
                printf("X Offset : %3.2f\n", xoffset);
                
                float yoffset = 0; memcpy(&yoffset, ptr, 4); ptr +=4;
                printf("Y Offset : %3.2f\n", yoffset);
                
                float zoffset = 0; memcpy(&zoffset, ptr, 4); ptr +=4;
                printf("Z Offset : %3.2f\n", zoffset);
                
            }
            else if(type ==2)   // skeleton
            {
                char szName[MAX_NAMELENGTH];
                strcpy(szName, ptr);
                ptr += strlen(ptr) + 1;
                printf("Name: %s\n", szName);
                
                int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                printf("ID : %d\n", ID);
                
                int nRigidBodies = 0; memcpy(&nRigidBodies, ptr, 4); ptr +=4;
                printf("RigidBody (Bone) Count : %d\n", nRigidBodies);
                
                for(int i=0; i< nRigidBodies; i++)
                {
                    if(major >= 2)
                    {
                        // RB name
                        char szName[MAX_NAMELENGTH];
                        strcpy(szName, ptr);
                        ptr += strlen(ptr) + 1;
                        printf("Rigid Body Name: %s\n", szName);
                    }
                    
                    int ID = 0; memcpy(&ID, ptr, 4); ptr +=4;
                    printf("RigidBody ID : %d\n", ID);
                    
                    int parentID = 0; memcpy(&parentID, ptr, 4); ptr +=4;
                    printf("Parent ID : %d\n", parentID);
                    
                    float xoffset = 0; memcpy(&xoffset, ptr, 4); ptr +=4;
                    printf("X Offset : %3.2f\n", xoffset);
                    
                    float yoffset = 0; memcpy(&yoffset, ptr, 4); ptr +=4;
                    printf("Y Offset : %3.2f\n", yoffset);
                    
                    float zoffset = 0; memcpy(&zoffset, ptr, 4); ptr +=4;
                    printf("Z Offset : %3.2f\n", zoffset);
                }
            }
            
        }   // next dataset
        
        printf("End Packet\n-------------\n");
        
    }
    else
    {
        printf("Unrecognized Packet Type.\n");
    }
    
    return frame;
}


sFrameOfMocapData *CMNatNetPacketParser::nextFrameBuffer() {
    sFrameOfMocapData *frame = &m_frame[m_iFrame];
    m_iFrame=(m_iFrame+1)&1;
    return frame;
}
