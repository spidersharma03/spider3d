#ifndef _PERCENTAGE_CHUNK_H

#define _PERCENTAGE_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Loads percentage values from binary data representing them.
 ******************************************************************************/
class PercentageChunk :public Chunk {


public :
    /** Represents an int percentage */
    const static int INT = 0x30;
    /** Represents a float percentage */
    const static int FLOAT = 0x31;

    /***************************************************************************
     * Gets tye type of percentage, reads it
     * and sets the value on the chopper using
     * the id of the current chunk as the key.
     **************************************************************************/

    void loadData(ChunkChopper* chopper) 
	{
		float* percentage = new float;
        int percentageType = chopper->getUnsignedShort();
        int percentageLength = chopper->getUnsignedInt();
        if (percentageType == INT)
		{
            *percentage = chopper->getUnsignedShort() / 100.0f;
            chopper->pushData(chopper->getID(), percentage );
        }
		else if (percentageType == FLOAT)
		{
            *percentage = chopper->getFloat() / 100.0f;
            chopper->pushData(chopper->getID(), percentage );
        } 
    }
};

#endif