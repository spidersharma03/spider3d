#ifndef _SPOT_LIGHT_CHUNK

#define _SPOT_LIGHT_CHUNK

#include "Chunk.h"
#include "ChunkChopper.h"



/*******************************************************************************
 * SpotLights to be placed in a scene.
 * All the default parameters other than 
 * position and direction are used and
 * not loaded from the 3ds file and MDD file
 ******************************************************************************/
class SpotLightChunk :public Chunk {

    /***************************************************************************
     * This is called by the chunk chopper before any of the chunk's 
     * subchunks  are loaded.  Any data loaded that may need to be 
     * used later by superchunks should be stored in
     * the chunk chopper via {@link ChunkChopper#popData}
     * @param chopper the ChunkChopper that will have the light placed in it.  
     **************************************************************************/
public:
    void loadData(ChunkChopper* chopper)
	{
        Vector3 target = chopper->getPoint();
        float beam = chopper->getFloat();
        float falloff = chopper->getFloat();
        Node* transformGroup = (Node*)chopper->getGroup();

		//vector<Node*>& vecChildren = transformGroup->getChildren();
		MeshNode* meshGroup = (MeshNode*)transformGroup;

        Mesh* object3d = new Mesh();
        //object3d.setDummyFlag();
        //object3d.setLightCamFlag();
        //object3d.setObjectName(chopper.getObjectName());
		if ( meshGroup )
		meshGroup->addMesh(object3d);
		
		vector<Mesh*>* obj3dVec = new vector<Mesh*>();
		obj3dVec->resize(1);
		(*obj3dVec)[0] = object3d;
        //chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(chopper->getObjectName(), obj3dVec));
        chopper->getShape3DList()[chopper->getObjectName()] = obj3dVec;

    }
};

#endif