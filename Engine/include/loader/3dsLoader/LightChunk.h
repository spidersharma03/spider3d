#ifndef _LIGHT_CHUNK_H

#define _LIGHT_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * Lights to be placed in a scene.
 * Only point lights and target spot lights are supported.
 * All the default parameters are used for lights as well.
 * Only position is specified.
 ******************************************************************************/
class LightChunk :public Chunk {

private:
	Vector3 currentPosition;

    /***************************************************************************
     * This is called by the chunk chopper before any of the chunk's 
     * subchunks  are loaded.  Any data loaded that may need to be 
     * used later by superchunks should be stored in
     * the chunk chopper via {@link ChunkChopper#pushData}
     * @param chopper used to store the position of the light. 
     **************************************************************************/
public:

	LightChunk()
	{
	}

    void loadData(ChunkChopper* chopper) 
	{
        currentPosition = chopper->getVector();
        //currentPosition = null;*/
    }

    /***************************************************************************
     * Gets the data put into the chopper by the subchunks
     * and creates a light, adding it to the scene as a named object.
     * @param chopper the ChunkChopper containing sub chunk data.
     **************************************************************************/
    void initialize(ChunkChopper* chopper) 
	{
    }
};

#endif