#ifndef _FRAMES_DESCRIPTION_CHUNK_H

#define _FRAMES_DESCRIPTION_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * This chunk contains the name of the object
 * the frames belong to and the parameters and 
 * hierarchy information for it.
 ******************************************************************************/
class FramesDescriptionChunk: public Chunk
{
    ////////////////////////////////////////////////////////////////////////////
    // Private Member Variables ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:
	Mesh* shape;
    vector<Mesh*>* object3dVec ;

    /***************************************************************************
     * reads the name of the object for these frames
     * and stores it in the chopper.
     * @param chopper the chopper used to store the transient data
     * for this chunk. 
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
    {
		
        KeyFramer* keyFramer = chopper->getKeyFramer();
        string objectName = chopper->getString();
        chopper->setObjectName(objectName);
        chopper->getUnsignedShort();
        chopper->getUnsignedShort();
		ChunkChopper::fatherID = chopper->getShort();

        shape = 0;
        object3dVec = 0;

        // For mesh
		if ( chopper->getShape3DList().find(objectName) != chopper->getShape3DList().end() ) 
        object3dVec = chopper->getShape3DList().find(objectName)->second;

		if ( object3dVec )
			shape = (*object3dVec)[0];
        
        //CMeshSceneNode* transformGroup = (CMeshSceneNode*)chopper->getNamedTransformGroup(objectName);
        MeshNode* transformGroup = (MeshNode*)chopper->getNamedTransformGroup(objectName);

        if(shape != 0)
        {
            /*if( ! shape->getObject3dName().equals("$$$DUMMY"))
            {
                if(shape.hierarchyID == -1)
                {
                    shape.setfatherID(ChunkChopper.fatherID);
                    if(object3dVec != null)
                        for(int i =0; i< object3dVec.size();i++)
                            ((Object3D)(object3dVec.get(i))).setHierarchyId(ChunkChopper.hierarchyID);
                    else
                        shape.setHierarchyId(ChunkChopper.hierarchyID);                  
                }
            }*/
        }//End

        //For Light and Camera
        if( !(objectName == "$$$DUMMY") && (shape == 0) && (transformGroup != 0) )
        {
            Mesh* omniObject3d = new Mesh();
			vector<Mesh*>* obj3dVec = new vector<Mesh*>();
            obj3dVec->push_back(omniObject3d);
            //omniObject3d.setfatherID(ChunkChopper.fatherID);
            //omniObject3d.setHierarchyId(ChunkChopper.hierarchyID);
            //omniObject3d.setObjectName(objectName);
            //chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(objectName, obj3dVec));
            chopper->getShape3DList()[objectName] = obj3dVec;
			//omniObject3d.setDummyFlag();
            //omniObject3d.setLightCamFlag();
			
			vector<Node*>& vecChildren = transformGroup->getChildren();
			MeshNode* meshGroup = (MeshNode*)transformGroup;
			meshGroup->addMesh(omniObject3d);
        }

        //For Dummy
        if(transformGroup == 0)//its a dummy transformGroup.
        {
			transformGroup = new MeshNode( 0 ,chopper->getSceneManager());
			keyFramer->setDummyObject(transformGroup);

			if(ChunkChopper::hierarchyID > -1)
			{
            Mesh* dummyObj3D = new Mesh();
            //dummyObj3D.setfatherID(ChunkChopper.fatherID);
            //dummyObj3D.setHierarchyId(ChunkChopper.hierarchyID);           
            //dummyObj3D.setObjectName(objectName);
            vector<Mesh*>* obj3dVec = new vector<Mesh*>();
			obj3dVec->push_back(dummyObj3D);
			//chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(objectName, obj3dVec));
 			chopper->getShape3DList()[objectName] = obj3dVec;
           
			//dummyObj3D.setDummyFlag();
			transformGroup->addMesh(dummyObj3D);
            }
        }//End

		keyFramer->addFather(ChunkChopper::fatherID, transformGroup);
    }

	FramesDescriptionChunk()
	{
	}
};


#endif