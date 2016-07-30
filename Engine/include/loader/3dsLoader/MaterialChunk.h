#ifndef _MATERIAL_CHUNK_H

#define _MATERIAL_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "Chunk_Types.h"

/*******************************************************************************
 * Loads material chunks with ambient, diffuse and specular colors,
 * shininess, transparency, two sidedness and texture.
 ******************************************************************************/
class MaterialChunk :public Chunk {

private:
	Appearance*  appearance;
    //private Material    material        = null;
    /***************************************************************************
     * This will set the ambient, diffuse and specular
     * colors as well as the textures, two sidedness
     * and transparency of the material.
     * @param chopper the chopper containing the data
     * needed to set the attributes.
     **************************************************************************/
public:
     void initialize(ChunkChopper* chopper)
    {
		appearance = new Appearance();
        
        //material = new Material();
        string objname = chopper->getObjectName();
        string* name    = (string*)chopper->popData(MATERIAL_NAME);

        Vector3* ambientColor = (Vector3*)chopper->popData(AMBIENT_COLOR);
        if (ambientColor != 0) 
		{
			appearance->getMaterial().ambient.set( ambientColor->x , ambientColor->y, ambientColor->z,1.0f );
        }
		

        Vector3* color = (Vector3*) chopper->popData(DIFFUSE_COLOR);
        if (color != 0) 
		{
			appearance->getMaterial().diffuse.set( color->x , color->y , color->z , 1.0f);
        }

        color = (Vector3*) chopper->popData(SPECULAR_COLOR);
        if (color != 0) 
		{
            // Changes By Tarun on 18 June 09 // Shininess Strength Fix
            float* shininessStrength = (float*) chopper->popData(SHININESS_STRENGTH);
            if (shininessStrength != 0)
			{
                *color *= *shininessStrength;
            }
            // End //
            appearance->getMaterial().specular.set( color->x ,color->y, color->z, 1.0f );
        }

        bool* twoSided = (bool*) chopper->popData(TWO_SIDED);
        if (twoSided != 0) //Just being there is equivalent to a boolean true.
        {
           
            //appearance->setFlag(EMF_BACK_FACE_CULLING , false);
			//appearance->setFlag(EMF_FRONT_FACE_CULLING , false);
        }

        float* transparency = (float*) chopper->popData(TRANSPARENCY);

        if (transparency != 0) 
		{
            if ( *transparency > 0.01f) 
			{
				appearance->getTransparencyAttributes().transparency = *transparency;
				appearance->getTransparencyAttributes().mode = ETM_BLEND;
				appearance->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
				appearance->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE_MINUS_SRC_ALPHA;
            }
        }

        float* shininess = (float*) chopper->popData(SHININESS);
        if (shininess != 0)
		{
			//appearance->Shininess = *shininess * 128;
			appearance->getMaterial().shininess = *shininess * 128;
        }

        //appearance.setMaterial(material);
        chopper->setAppearanceMap(*name, appearance);
    }
};

#endif