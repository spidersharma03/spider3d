#ifndef _KF_VERSION_CHUNK_H

#define _KF_VERSION_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * Read the KeyFrame Version from the 3ds and MDD file.
 ******************************************************************************/
class KFVersionChunk : public Chunk
{

public:

    KFVersionChunk() {
    }

    /***************************************************************************
     * Read the KeyFrame Version from the 3ds and MDD file.
     * @param ChunkChopper Reference
     **************************************************************************/
    void loadData(ChunkChopper* chopper)
    {
		
        short version = (short) chopper->getShort();
        string verName = chopper->getString();
        int frameCount = chopper->getInt();

        //MaxDataStructure.kfVersion = (int) version;
        //chopper.getMaxModelDS().setVersionName(verName);*/
    }
};


#endif