#include "NormalGenerator.h"
#include "Mesh.h"

void NormalGenerator::generateNormals(MeshBuffer* meshBuffer)
{
	unsigned short index0 = 0;
	unsigned short index1 = 0;
	unsigned short index2 = 0;
	Point3 p0, p1, p2;
	Vector3 v0;
    Vector3 v1;
	Vector3 normal;

	float* normals = meshBuffer->getNormals();
	float* coords = meshBuffer->getCoordinates();

	for ( int i = 0 ; i < meshBuffer->getVertexCount()*3; i += 9 )
	{
         index0 = i;
		 index1 = i+1;
		 index2 = i+2;
		 
		 p0.set(coords[i], coords[i+1], coords[i+2]);
		 p1.set(coords[i+3], coords[i+4], coords[i+5]);
		 p2.set(coords[i+6], coords[i+7], coords[i+8]);

		 v0.sub(p1,p0);
		 v1.sub(p2,p0);

		 normal = v0.cross(v1);
		 normal.normalize();

		 normals[i] = normal.x; normals[i+1] = normal.y; normals[i+2] = normal.z;
		 normals[i+3] = normal.x; normals[i+4] = normal.y; normals[i+5] = normal.z;
		 normals[i+6] = normal.x; normals[i+7] = normal.y; normals[i+8] = normal.z;
	}
}

void NormalGenerator::generateNormals(Mesh* mesh)
{
	for ( unsigned i = 0 ; i < mesh->getNumMeshBuffers() ; i++ )
	{
		MeshBuffer& meshBuffer = mesh->getMeshBuffer(i);
		generateNormals( &meshBuffer );
	}
}