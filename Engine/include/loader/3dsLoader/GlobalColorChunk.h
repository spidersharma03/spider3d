#ifndef _GLOBAL_COLOR_CHUNK_H

#define _GLOBAL_COLOR_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * Loads colors from binary data representing them.
 * Its a global in the manner that it reads its own
 * header information.
 ******************************************************************************/
class GlobalColorChunk :public ColorChunk 
{

    /***************************************************************************
     * Gets the color type for this chunk.
     * @param chopper with the information the
     * chunk may use to determine color type
     * @return the color type for the chunk.
     **************************************************************************/
protected:

    virtual int getColorType(ChunkChopper* chopper)
	{
        int type = chopper->getUnsignedShort();
        int colorLength = chopper->getUnsignedInt();
        return type;
    }
};


#endif