#ifndef _FACES_MATERIAL_CHUNK_H

#define _FACES_MATERIAL_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
 * FacesMaterialsChunk contains the materials information 
 * from the 3ds file. It contains information pertaining to
 * which faces of a mesh have materials on them and the
 * texture coordinates for texture mapping.
 * Right now, its just putting the name of the material
 * that needs to be applied to the mesh under construction.
 ******************************************************************************/
class FacesMaterialChunk : public Chunk
{
    /***************************************************************************
     * Loads the texture coordinates for a mesh, 
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
    {
		
        string materialName = chopper->getString();
        
		if ( chopper->getChangeMatNameMap().find(materialName) != chopper->getChangeMatNameMap().end() )
		{		 
			materialName =  ( chopper->getChangeMatNameMap().find(materialName))->second;
        }

        
		//char* CurMeshMatName = chopper->getCurNameOfObject() + "<" + materialName;
        string CurMeshMatName;// = chopper->getCurNameOfObject() + "<" + materialName;
        CurMeshMatName += chopper->getCurNameOfObject();
		CurMeshMatName += "<";
		CurMeshMatName += materialName;

        int numFaces = chopper->getUnsignedShort();


        vector<unsigned>* curMatFacesIndices = new vector<unsigned>(numFaces);
		//curMatFacesIndices->set_used( numFaces );

        if (numFaces > 0)
        {
            for (int j = 0; j < numFaces; j++)
            {
                int index = chopper->getUnsignedShort();
                (*curMatFacesIndices)[j] = index*3;
            }
        }

		if ( chopper->getMaterialFaceIndicesMap().find(CurMeshMatName) ==  chopper->getMaterialFaceIndicesMap().end() )
        {
            chopper->getMaterialFaceIndicesMap().insert( pair<string,vector<unsigned int>*>(CurMeshMatName , curMatFacesIndices));
            chopper->pushData(chopper->getID(), &materialName);
        }
    }

};

#endif