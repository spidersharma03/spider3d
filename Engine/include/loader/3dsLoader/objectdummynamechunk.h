#ifndef _OBJECT_DUMMY_NAME_CHUNK_H

#define _OBJECT_DUMMY_NAME_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Loads the position of a mesh as defined in the 3ds file.
 * This position may need to be converted to another coordinate
 * system by KeyFramerInfoChunk.
 *  * {@see KeyFramerInfoChunk} for more information about using
 * animations from a 3ds file and Mdd File
 ******************************************************************************/
class ObjectDummyNameChunk :public Chunk {

    ////////////////////////////////////////////////////////////////////////////
    // Private Member Variable /////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:
	vector<Mesh*>* object3dVec ;
    Mesh* shape ;

    /***************************************************************************
     * Loads the position for a shape and KeyFramerInfoChunk
     * @param chopper the ChunkChopper containing the state of the parser.
     **************************************************************************/
public:

	void loadData(ChunkChopper* chopper) 
	{
      string ObjDummyName = chopper->getString();

       if( !( chopper->getObjectName() == "$$$DUMMY") )
        {
             object3dVec = chopper->getShape3DList().find( chopper->getObjectName() )->second;
             shape = (*object3dVec)[0];
        }
	    else
        {
            object3dVec = chopper->getShape3DList().find( chopper->getObjectName() )->second;
            shape = (*object3dVec)[0];
        }

        if ( ! (chopper->getObjectName() == "$$$DUMMY" ) ) 
		{
            //for pivot ..
            //Vector3f pivot = (Vector3f) chopper.getMaxModelDS().getMeshPivot(chopper.getObjectName());
            //chopper.getMaxModelDS().addMeshPivot(ObjDummyName, pivot);

            Matrix4f* t3d =  chopper->namedObjectCoordinateSystems.find(chopper->getObjectName())->second;
            //chopper->namedObjectCoordinateSystems.insert(pair<string,Matrix4f*>(ObjDummyName, t3d));
            chopper->namedObjectCoordinateSystems[ObjDummyName] = t3d;

            if(object3dVec == 0)
            {
				vector<Mesh*>* obj3dVec = new vector<Mesh*>();
				obj3dVec->resize(1);
                (*obj3dVec)[0] = shape; 
                //chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(ObjDummyName, obj3dVec));
                chopper->getShape3DList()[ObjDummyName] = obj3dVec;
				//shape.setLocalCoordinateT3D(t3d);
            }
            else
            {
                //chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(ObjDummyName, object3dVec));
                chopper->getShape3DList()[ObjDummyName] = object3dVec;
				//for(int p = 0 ; p < object3dVec.size() ; p++)
                //{
                //    object3dVec.get(p).setLocalCoordinateT3D(t3d);
                //}
            }

            if (shape != 0) 
			{
				MeshNode* meshGroup = new MeshNode(0, chopper->getSceneManager());
                Mesh* shapecopy = new Mesh();
				shapecopy = shape;
                //shapecopy.setObjectName(ObjDummyName);
                //shapecopy.setMeshOriginalName(chopper.getObjectName());

                //shapecopy.setfatherID(ChunkChopper.fatherID);
                //shapecopy.setHierarchyId(ChunkChopper.hierarchyID);

				meshGroup->addMesh(shapecopy);

                chopper->addObject(ObjDummyName, meshGroup);
				chopper->getKeyFramer()->addFather(ChunkChopper::fatherID, meshGroup);
            }
            chopper->setObjectName(ObjDummyName);
        } 
		else 
		{
            //shape.setObjectName(ObjDummyName);
			vector<Mesh*>* object3dVec = new vector<Mesh*>();
            object3dVec->resize(1);
            (*object3dVec)[0] =  shape;
            //chopper->getShape3DList().insert(pair<string,vector<Mesh*>*>(ObjDummyName, object3dVec));
            chopper->getShape3DList()[ObjDummyName] = object3dVec;
			if ( chopper->getShape3DList().find ( "$$$DUMMY" ) != chopper->getShape3DList().end() )
			{
				chopper->getShape3DList().erase("$$$DUMMY");
			}
			
            /// oct 15
            //Vector3f pivot = (Vector3f) chopper.getMaxModelDS().getMeshPivot("$$$DUMMY");
            //chopper.getMaxModelDS().addMeshPivot(ObjDummyName, pivot);
            chopper->setObjectName(ObjDummyName);
        }

    }

};
 
#endif