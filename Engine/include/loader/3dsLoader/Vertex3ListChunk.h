#ifndef _VERTEX3LIST_CHUNK_H

#define _VERTEX3LIST_CHUNK_H


#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Reads and store x,y,z vertex coordinates.
 * The coordinates will be accessed with indexes to construct 
 * the mesh out of triangles.
 ******************************************************************************/
class Vertex3ListChunk :public Chunk {
    /***************************************************************************
     * Reads all the point data from the chopper
     * and stores it using this chunk's id as the key.
     * @param chopper the chopper that will read and
     * store the data. 
     **************************************************************************/
public:
    void loadData(ChunkChopper* chopper) 
	{
        int numVertices = chopper->getUnsignedShort();
        //vector3df* points = new vector3df[numVertices];
		
		vector<Vector3*>* points = new vector<Vector3*>();
		points->resize(numVertices);
		
        for ( int i = 0 ; i < numVertices ; i ++ )
           (*points)[i] = new Vector3();

		for (int i = 0; i < numVertices; i++) 
		{
			Vector3 p = chopper->getPoint();
			(*points)[i]->x = p.x;
			(*points)[i]->y = p.y;
			(*points)[i]->z = p.z;
        }

        chopper->pushData(chopper->getID(), points);
        //chopper->setMeshVertices(chopper->getObjectName(), points);
    }
};

#endif