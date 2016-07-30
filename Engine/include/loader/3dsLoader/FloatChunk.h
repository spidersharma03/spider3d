#ifndef _FLOAT_CHUNK_H

#define _FLOAT_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * These have no subchunks. Only data.
 * Any objects that have a float chunk
 * as a sub chunk may retrieve the float
 * value from {@link ChunkChopper.popData()}
 * using the current chunk id as an argument.
 ******************************************************************************/
class FloatChunk : public Chunk 
{

    /***************************************************************************
     * Loads a Float value into the chopper
     * for use later when parent chunks are 
     * initializing
     *
     * @param chopper the chopper in which the float
     * chunk will be stored by the id of this chunk. 
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
	{
		
        float* floatValue = new float;
		*floatValue = chopper->getFloat();
        chopper->pushData(chopper->getID(), floatValue);

        if (chopper->getID() == 2) {
            //chopper.getMaxModelDS().setMaxVersion(floatValue);           
        }
        if (chopper->getID() == 0x100) {
            //chopper.getMaxModelDS().setMasterScale(floatValue);
        }
        if (chopper->getID() == -20471) {
            //chopper.getMaxModelDS().setKfTime(floatValue);
        }

    }

	void initialize(ChunkChopper* chopper)
	{
	}
};

#endif