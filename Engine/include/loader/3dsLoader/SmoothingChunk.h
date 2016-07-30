#ifndef _SMOOTHING_CHUNK_H

#define _SMOOTHING_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "Chunk_Types.h"

/*******************************************************************************
 * This chunk is used to generate normals for a mesh according
 * to the data in the smoothing groups chunk.
 * Vertices that share coordinates will all use the same, averaged
 * normal if they also belong to the same smoothing groups.
 ******************************************************************************/
class SmoothingChunk :public Chunk {


private:

	unsigned* smoothGroups ;

    /***************************************************************************
     * Loads the vertices smoothing groups for 
     * a mesh and stores it in chopper
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
public:
    void loadData(ChunkChopper* chopper) 
	{

		int numFaces = ChunkChopper::NumVertices/3;
        vector<Vector3*>* vertices = (vector<Vector3*>*) chopper->popData(FACES_DESCRIPTION);
        smoothGroups = new unsigned[vertices->size()];
        for (int i = 0; i < numFaces; i++) 
		{
            int groupMask = chopper->getInt();
            smoothGroups[i * 3] = (unsigned)groupMask;
            smoothGroups[(i * 3) + 1] = (unsigned)groupMask;
            smoothGroups[(i * 3) + 2] = (unsigned)groupMask;
        }
		
        chopper->pushData(chopper->getID(), smoothGroups);

    }
};

#endif