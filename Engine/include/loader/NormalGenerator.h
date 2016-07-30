#ifndef _NORMAL_GENERATOR_H

#define _NORMAL_GENERATOR_H

#include "IndexedMeshBuffer.h"

namespace spider3d
{
	namespace scene
	{
		class Mesh;
	}
}

using namespace spider3d::scene;

class NormalGenerator
{
public:
   
	void generateNormals( MeshBuffer* meshBuffer);

	void generateNormals( Mesh* mesh);

};

#endif