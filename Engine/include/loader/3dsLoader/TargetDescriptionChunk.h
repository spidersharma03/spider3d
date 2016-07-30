#ifndef _TARGET_DESCRIPTION_CHUNK_H

#define _TARGET_DESCRIPTION_CHUNK_H

#include "Chunk.h"
#include "KeyFramer.h"

/*******************************************************************************
 * TargetDescription chunk for omnilight and camera.
 *******************************************************************************/
class TargetDescriptionChunk :public Chunk {

public:

    void loadData(ChunkChopper* chopper)
	{
        KeyFramer* keyFramer = chopper->getKeyFramer();
        string objectName = chopper->getString();
        chopper->getUnsignedShort();
        chopper->getUnsignedShort();
		ChunkChopper::fatherID = chopper->getShort();


        string s2 ;
        for (unsigned j = 0; j < objectName.size(); j++) {
            char c = objectName[j];
            if (c == '.') {
                objectName = s2;
                break;
            }
			string character;
			character = c;
			s2.append(character);

        }
        //s2 = null;

        
        Node* transformGroup = (Node*)chopper->getNamedTransformGroup(objectName);

        if( transformGroup == 0 )
        {
			transformGroup = new Node(0,chopper->getSceneManager());
			MeshNode* meshGroup = new MeshNode(0,chopper->getSceneManager());
			transformGroup->addChild(meshGroup);
			chopper->addObject(objectName, transformGroup);
        }

        Mesh* shape = new Mesh() ;
        //shape.setDummyFlag();
        //shape.setLightCamFlag();
        //shape.setfatherID(ChunkChopper.fatherID);
        //shape.setHierarchyId(ChunkChopper.hierarchyID);
        //shape.setObjectName(objectName + ".target");
		//vector<Node*>& vecChildren = transformGroup->getChildren();
		MeshNode* meshGroup = dynamic_cast<MeshNode*>(transformGroup);
        chopper->setObjectName(objectName + ".target");
        if( meshGroup )
			meshGroup->addMesh(shape);
    }
};

#endif