#ifndef _BLUR_MESH_H
#define _BLUR_MESH_H

#include "coreMath.h"
#include "SIndexedMeshBuffer.h"
#include "Mesh.h"
#include <math.h>

using namespace spider3d::core;

Mesh* createBlurMesh()
{
	int n = 20;
	Mesh *mesh = new Mesh();
	sf32* coordinates = new sf32[4 * 3 * n];
	sf32* texcoords   = new sf32[4 * 2 * n];
	sf32* colors      = new sf32[4 * 4 * n];
    float size = 1.0f;

	texcoords[0] =  0.0f; 	texcoords[1] = 0.0f; 
	texcoords[2] =  1.0f; 	texcoords[3] = 0.0f; 
	texcoords[4] =  1.0f; 	texcoords[5] = 1.0f; 
	texcoords[6] =  0.0f; 	texcoords[7] = 1.0f; 

	float alpha = 0.4f;

	//colors[0]  = 1.0f; colors[1] = 1.0f; colors[2] = 1.0f; colors[3] = alpha;
	//colors[4]  = 1.0f; colors[5] = 1.0f; colors[6] = 1.0f; colors[7] = alpha;
	//colors[8]  = 1.0f; colors[9] = 1.0f; colors[10] = 1.0f; colors[11] = alpha;
	//colors[12] = 1.0f; colors[13] = 1.0f; colors[14] = 1.0f; colors[15] = alpha;
    
	float alphainc = alpha/n;
    float texcoordOffset = 0.0f;
	float texcoordOffsetInc = 0.02f;

	coordinates[0] = -size; coordinates[1] = 0.0f; coordinates[2] = -size;
	coordinates[3] =  size; coordinates[4] = 0.0f; coordinates[5] = -size;
	coordinates[6] =  size; coordinates[7] =  0.0f; coordinates[8] = size;
	coordinates[9] = -size; coordinates[10] = 0.0f; coordinates[11] = size;

	float z = 0.0f;
	for( int i=0; i<n*12; i+=12 )
	{
		coordinates[i] = -size; coordinates[i+1] = z; coordinates[i+2] = -size;
		coordinates[i+3] =  size; coordinates[i+4] = z; coordinates[i+5] = -size;
		coordinates[i+6] =  size; coordinates[i+7] =  z; coordinates[i+8] = size;
		coordinates[i+9] = -size; coordinates[i+10] = z; coordinates[i+11] = size;
		z += 0.001f;
	}

	for( int i=0; i<n*8; i+=8 )
	{
		texcoords[i+0] =  0.0f + texcoordOffset; 	texcoords[i+1] = 0.0f + texcoordOffset; 
		texcoords[i+2] =  1.0f - texcoordOffset; 	texcoords[i+3] = 0.0f + texcoordOffset; 
		texcoords[i+4] =  1.0f - texcoordOffset; 	texcoords[i+5] = 1.0f - texcoordOffset; 
		texcoords[i+6] =  0.0f + texcoordOffset; 	texcoords[i+7] = 1.0f - texcoordOffset; 
		texcoordOffset += texcoordOffsetInc;
	}

	for( int i=0; i<n*16; i+=16 )
	{
		colors[i+0]  = 1.0f; colors[i+1] = 1.0f; colors[i+2] = 1.0f; colors[i+3] = alpha;
		colors[i+4]  = 1.0f; colors[i+5] = 1.0f; colors[i+6] = 1.0f; colors[i+7] = alpha;
		colors[i+8]  = 1.0f; colors[i+9] = 1.0f; colors[i+10] = 1.0f; colors[i+11] = alpha;
		colors[i+12] = 1.0f; colors[i+13] = 1.0f; colors[i+14] = 1.0f; colors[i+15] = alpha;
		alpha -= alphainc;
	}

	for( int i=0; i<1; i++ )
	{
		SMeshBuffer* buffer = new SMeshBuffer( n*4 , COORDINATES | COLOR_4 | TEXTURE_COORDINATE_2 , 1);
		buffer->setCoordinates(coordinates);
		buffer->setColors(colors);
		buffer->setTexCoords(0,texcoords);
		buffer->setPrimitiveType(QUADS);
		buffer->reCalculateAABB();
		mesh->addMeshBuffer(buffer);
		
		/*texcoords[0] =  0.0f + texcoordOffset; 	texcoords[1] = 0.0f + texcoordOffset; 
		texcoords[2] =  1.0f - texcoordOffset; 	texcoords[3] = 0.0f + texcoordOffset; 
		texcoords[4] =  1.0f - texcoordOffset; 	texcoords[5] = 1.0f - texcoordOffset; 
		texcoords[6] =  0.0f + texcoordOffset; 	texcoords[7] = 1.0f - texcoordOffset; 

		coordinates[1] = coordinates[4] = coordinates[7] = coordinates[10] = z;
	
		colors[3] = colors[7] = colors[11] = colors[15] = alpha;
		alpha -= alphainc;
		texcoordOffset += texcoordOffsetInc;
		z += 0.005f;*/
	}

	delete[] coordinates;
	delete[] colors;
	delete[] texcoords;

	return mesh;
}

#endif