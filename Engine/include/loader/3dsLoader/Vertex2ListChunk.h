#ifndef _VERTEX2LIST_CHUNK_H

#define _VERTEX2LIST_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Reads a list of x,y points that will be used
 * later for texture mapping
 ******************************************************************************/
class Vertex2ListChunk :public Chunk {
    /***************************************************************************
     * Reads all the point data from the chopper and stores
     * teh points in the chopper.
     * @param chopper the chopper that will parse and store
     * the data using this chunks id as the key. 
     **************************************************************************/
public:

	void loadData(ChunkChopper* chopper) 
	{
        int numVertices = chopper->getUnsignedShort();
        
		vector<Vector2*>* points = new vector<Vector2*>();
		points->resize(numVertices);
		for ( int i = 0 ; i < numVertices ; i ++ )
			(*points)[i] = new Vector2();

        for (int i = 0; i < numVertices; i++)
		{
            float point0 = chopper->getFloat();
            float point1 = chopper->getFloat();
			(*points)[i]->x = point0;
			(*points)[i]->y = point1;
        }
        chopper->pushData(chopper->getID(), points);
    }
};


#endif