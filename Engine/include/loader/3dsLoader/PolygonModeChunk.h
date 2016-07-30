#ifndef POLYGON_MODE_CHUNK_H

#define POLYGON_MODE_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 *Read the polygon Mode Data
 ******************************************************************************/
class PolygonModeChunk :public Chunk
{

    public:

	/**
     * Default Constructor
     */
    PolygonModeChunk()
    {
    }

    /***************************************************************************
    *Read the polygon Mode Data
    ***************************************************************************/

    void loadData(ChunkChopper* chopper)
    {
        int mode = chopper->getUnsignedByte();
        chopper->pushData(chopper->getID(), &mode);
    }
};

#endif 