#ifndef _PIVOT_CHUNK_H

#define _PIVOT_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Loads the pivot for a mesh. 
 * {@see KeyFramerInfoChunk} for more information about using
 * animations from a 3ds file and MDD file
 ******************************************************************************/
class PivotChunk :public Chunk {

private:
	Vector3 pivot;

    /***************************************************************************
     * Gets the pivot and associates it with the current mes.
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
public :
    void loadData(ChunkChopper* chopper) 
	{
        pivot = chopper->getPoint();
        string meshName = chopper->getObjectName();
        //chopper.getMaxModelDS().addMeshPivot(meshName, new Vector3f(pivot));
        chopper->getKeyFramer()->setPivot( pivot );
        //pivot = null;*/
    }
};

#endif