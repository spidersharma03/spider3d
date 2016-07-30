#ifndef _CAMERA_CHUNK

#define _CAMERA_CHUNK

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * Loads percentage values from binary data representing them.
 ******************************************************************************/
class CameraChunk :public Chunk {

private:
	Vector3 position ;
    Vector3 target;

    /***************************************************************************
     * Reads the position and target vectors and subtracts them to get an axis of
     * rotation. Translate a transform to position and rotate on the axis of
     * rotation to point at the target. The angle between the z axis and the axis
     * of rotation is the angle used to rotate. The translated and rotated vector
     * is stored it the chopper as a named object since camera chunks are named.
     **************************************************************************/
    void loadData(ChunkChopper* chopper) {

        position    = chopper->getPoint();
        target      = chopper->getPoint();
        float bank  = chopper->getFloat();
        float lens  = chopper->getFloat();
        
        //position = null; //no use
        //target   = null; //no use

		Node* trg = chopper->getGroup();
		Mesh* object3d = new Mesh();
        /*TransformGroup trg = chopper.getGroup();
        Object3D object3d = new Object3D();
        object3d.setDummyFlag();
        object3d.setLightCamFlag();
        object3d.setObjectName(chopper.getObjectName());
        trg.addChild(object3d);*/
		vector<Mesh*>* obj3dVec = new vector<Mesh*>();
        obj3dVec->push_back(object3d);
        chopper->getShape3DList().insert( pair<string,vector<Mesh*>*>(chopper->getObjectName(), obj3dVec));
    }
};

#endif