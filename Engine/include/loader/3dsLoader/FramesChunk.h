#ifndef _FRAMES_CHUNK_H

#define _FRAMES_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * This chunk specifies the beginning and end frames.
 ******************************************************************************/
class FramesChunk :public Chunk
{
    /***************************************************************************
     * Reads two ints. Start frame and stop frame. 
     * @param chopper  the chopper that may be used to store the starting and 
     * stopping frames.
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
    {
		
        long start = chopper->getUnsignedInt();
        long stop = chopper->getUnsignedInt();
		ChunkChopper::FrameCnt = stop - start + 1;
    }
};


#endif