#ifndef _NAMED_OBJECT_CHUNK_H

#define _NAMED_OBJECT_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * Loads information about a named object: Cameras, meshes and lights
 ******************************************************************************/
class NamedObjectChunk :public Chunk
{

    /***************************************************************************
     * Adds a TransformGroup the the chopper's branch
     * group to which meshes will be added.
     * @param chopper The chopper containing the state of parsing.  
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
    {
        string name = chopper->getString();
		//Node* transformGroup = new Node(0,chopper->getSceneManager());
		MeshNode* meshGroup = new MeshNode(0,  chopper->getSceneManager());
		//transformGroup->addChild(meshGroup);
		chopper->setCurNameOfObject(name);
        chopper->addObject(name, meshGroup);
        //chopper.getMaxModelDS().addMeshName(name);*/
    }

};

#endif