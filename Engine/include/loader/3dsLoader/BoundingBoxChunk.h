#ifndef _BOUNDING_BOX_CHUNK

#define _BOUNDING_BOX_CHUNK

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Loads the bounding box for keyframer of mesh. The pivot is relative to it.
 * {@see KeyFramerInfoChunk} for more information about using
 * animations from a 3ds file
 ******************************************************************************/
class BoundingBoxChunk: public Chunk {
private:
	Vector3 min ;
    Vector3 max ;

    /***************************************************************************
     * Gets the bounding box and associates it with the current mes.
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
    void loadData(ChunkChopper* chopper) {
        min = chopper->getPoint();
        max = chopper->getPoint();
    }
};

#endif
