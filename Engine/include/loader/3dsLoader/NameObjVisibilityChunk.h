#ifndef _NAME_OBJ_VISIBILITY_CHUNK_H

#define _NAME_OBJ_VISIBILITY_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Read the Mesh Visibility Flag From file
 ******************************************************************************/
class NameObjVisibilityChunk :public Chunk{

public:
    /**
     * Default Constructor
     */
    NameObjVisibilityChunk()
    {
    }

    /***************************************************************************
     * Read visibility of mesh
     **************************************************************************/
    void loadData(ChunkChopper* chopper)
    {
        int mode = chopper->getUnsignedByte();
        bool visibility = true;
        if(mode < 1)
            visibility = false;
        chopper->pushData(chopper->getID(), &visibility);
    }
};

#endif