#include "MeshBufferUpdater.h"
#include "MeshBuffer.h"

using namespace spider3d::scene;

#define divisionZ 64
#define divisionX 64
#define delta (12.0/64)

typedef struct Neighbour
{
	int EAST, WEST, NORTH, SOUTH;
}Neighbour;

Neighbour *neighbours;

class CustomMeshUpdater:public MeshBufferUpdater
{
public:
	CustomMeshUpdater(MeshBuffer* meshBufer)
	{
		float* coordinates = meshBufer->getCoordinates();
		float* texCoords   = meshBufer->getTexCoords(0);
		originalCoordsT			  = new float[meshBufer->getVertexCount()*3];
		originalCoordsT_Minus_One = new float[meshBufer->getVertexCount()*3];
		for( unsigned i = 0 ; i < meshBufer->getVertexCount()*3; i++ )
		{
			originalCoordsT[i]			 = coordinates[i];
			originalCoordsT_Minus_One[i] = coordinates[i];
			//originalTexCoords[i]		 = texCoords[i];
		}
		
		neighbours = new Neighbour[(divisionX+1)*(divisionZ+1)];

		// Assign Neighbours
		int l = 0;
		for (int i1 = 0; i1 < divisionZ+1 ; i1++) 
		{
			for (int j = 0 ; j < divisionX+1; j++)
			{
				//if( i != 0 && j != 0 && i != divisionZ-1 && j != divisionX-1 )
				{
					neighbours[l].EAST  = l+1;
					neighbours[l].WEST  = l-1;
					neighbours[l].NORTH = l + divisionX + 1;
					neighbours[l].SOUTH = l - divisionX - 1;
				}
				if ( i1 == 0 )
				{
					neighbours[l].SOUTH  = -1;
				}
				if ( j == 0 )
				{
					neighbours[l].WEST  = -1;
				}
				if ( i1 == divisionX )
				{
					neighbours[l].NORTH  = -1;
				}
				if ( j == divisionZ )
				{
					neighbours[l].EAST  = -1;
				}
				l++;
			}
		}
	}

	void updateMeshBuffer(MeshBuffer* meshBufer)
	{
		float* coordinates = meshBufer->getCoordinates();
		float* texCoords   = meshBufer->getTexCoords(0);
		float* normals     = meshBufer->getNormals();

		static float dt = 0.0f;

		float centreX = 0.0f;
		float centreY = 0.0f;

		for( int i=0; i<meshBufer->getVertexCount()*3; i+=3 )
		{
			float dx = originalCoordsT[i] - centreX;
			float dy = originalCoordsT[i+2] - centreY;
			float r = sqrt( dx*dx + dy*dy);

            //float dis = omega * localt / k;

			//if (r < dis) 
			{
				float perturb = (float) (0.003f * (sin(35.01f * r - 2.0f * dt)));
				//perturb *= Math.exp(-damping * localt);
				coordinates[i] = originalCoordsT[i] + perturb;
				coordinates[i+2] = originalCoordsT[i+2] + perturb;
			}

		}

		dt += 0.04f;

	}
private:
	float* originalCoordsT;
	float* originalCoordsT_Minus_One;
	float* originalTexCoords;
};
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*

#include "MeshBufferUpdater.h"
#include "MeshBuffer.h"

using namespace spider3d::scene;

#define divisionZ 64
#define divisionX 64
#define delta (12.0/64)

typedef struct Neighbour
{
	int EAST, WEST, NORTH, SOUTH;
}Neighbour;

Neighbour *neighbours;

class CustomMeshUpdater:public MeshBufferUpdater
{
public:
	CustomMeshUpdater(MeshBuffer* meshBufer)
	{
		float* coordinates = meshBufer->getCoordinates();
		float* texCoords   = meshBufer->getTexCoords(0);
		originalCoordsT			  = new float[meshBufer->getVertexCount()*3];
		originalCoordsT_Minus_One = new float[meshBufer->getVertexCount()*3];
		for( unsigned i = 0 ; i < meshBufer->getVertexCount()*3; i++ )
		{
			originalCoordsT[i]			 = coordinates[i];
			originalCoordsT_Minus_One[i] = coordinates[i];
			//originalTexCoords[i]		 = texCoords[i];
		}
		
		neighbours = new Neighbour[(divisionX+1)*(divisionZ+1)];

		// Assign Neighbours
		int l = 0;
		for (int i1 = 0; i1 < divisionZ+1 ; i1++) 
		{
			for (int j = 0 ; j < divisionX+1; j++)
			{
				//if( i != 0 && j != 0 && i != divisionZ-1 && j != divisionX-1 )
				{
					neighbours[l].EAST  = l+1;
					neighbours[l].WEST  = l-1;
					neighbours[l].NORTH = l + divisionX + 1;
					neighbours[l].SOUTH = l - divisionX - 1;
				}
				if ( i1 == 0 )
				{
					neighbours[l].SOUTH  = -1;
				}
				if ( j == 0 )
				{
					neighbours[l].WEST  = -1;
				}
				if ( i1 == divisionX )
				{
					neighbours[l].NORTH  = -1;
				}
				if ( j == divisionZ )
				{
					neighbours[l].EAST  = -1;
				}
				l++;
			}
		}
	}

	void updateMeshBuffer(MeshBuffer* meshBufer)
	{
		return;
		float* coordinates = meshBufer->getCoordinates();
		float* texCoords   = meshBufer->getTexCoords(0);
		float* normals     = meshBufer->getNormals();

		int l = 0;
		static float dt = 0.0f;
		float deltaT = 0.5f;

		int n = 0;
		float objectSpeed = 0.2f;
		float objectX = 15.0f;
		float objectZ = 0.0f;
		float lambda = 0.0f;
		float disturb = -1.0f;// * sin(dt) * exp(-lambda*dt);
		float damping = 0.999f;
		//objectX += 10*sin(objectSpeed*dt);
		float objX = objectX * cos(objectSpeed * dt);
		float objZ = objectX * sin(objectSpeed * dt);

		for (int i = 0; i < (divisionZ+1)*(divisionX+1) ; i++) 
		{	
			float LaplaceOperator = 0.0f;
			Neighbour *pNbr = &neighbours[n];
			int nbrs[] = { pNbr->EAST, pNbr->WEST, pNbr->NORTH, pNbr->SOUTH };

			// ReCalculate Normals
			int nbrIndex1 = nbrs[1];
			int nbrIndex2 = nbrs[2];
			if ( nbrIndex1 > 0 && nbrIndex2 > 0 )
			{
			float dx = coordinates[3*nbrIndex1] - coordinates[l];
			float dy = coordinates[3*nbrIndex1+1] - coordinates[l+1];
			float dz = coordinates[3*nbrIndex1+2] - coordinates[l+2];
			float d = sqrt(dx*dx+dy*dy+dz*dz);
			dx /= d; dy /= d; dz /= d;

			float dx1 = coordinates[3*nbrIndex2] - coordinates[l];
			float dy1 = coordinates[3*nbrIndex2+1] - coordinates[l+1];
			float dz1 = coordinates[3*nbrIndex2+2] - coordinates[l+2];
			float d1 = sqrt(dx1*dx1+dy1*dy1+dz1*dz1);
			dx1 /= d1; dy1 /= d1; dz1 /= d1;

			float nx = dy * dz1 - dy1 * dz;
			float ny = dx1 * dz - dz1 * dx;
			float nz = dx * dy1 - dy1 * dx;
			float d3 = sqrt(nx*nx+ny*ny+nz*nz);

			nx /= d3; ny /= d3; nz /= d3;
			float factor = 7.0f;
			texCoords[l]   = originalCoordsT[l] + nx * factor ;
			texCoords[l+1] = originalCoordsT[l+1] + nz * factor  ;
			//texCoords[l+2] = originalCoordsT[l+2] + -ny * factor;
			//normals[l]   = nx;
			//normals[l+1] = -ny;
			//normals[l+2] = nz;
			}

			float dx = objX - originalCoordsT[l]; float dz = objZ - originalCoordsT[l+2];
			float r = sqrt(dx*dx + dz*dz);
			if( r < 0.5f)
			{
				originalCoordsT[l+1] = disturb ;
			}

			for ( int k=0; k< 4;k++ )
			{
				int index = nbrs[k];
				if ( index > 0 )
					LaplaceOperator += originalCoordsT[3*index+1];
			}

			LaplaceOperator -= 4*originalCoordsT[l+1];
			LaplaceOperator /= (delta*delta);
			coordinates[l+1] = (1 + damping) * originalCoordsT[l+1] - damping * originalCoordsT_Minus_One[l+1] + 0.00009f*deltaT*deltaT*LaplaceOperator;

			l += 3;
			n++;
		}
		l = 0;
		for (int i2 = 0; i2 < (divisionZ+1)*(divisionX+1) ; i2++) 
		{
			originalCoordsT_Minus_One[l+1] = originalCoordsT[l+1];
			originalCoordsT[l+1]		   = coordinates[l+1];
			l += 3;
		}

		dt += 0.04f;

	}
private:
	float* originalCoordsT;
	float* originalCoordsT_Minus_One;
	float* originalTexCoords;
};
*/