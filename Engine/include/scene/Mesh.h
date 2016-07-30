#ifndef _MESH_H

#define _MESH_H

#include <vector>
#include "spiderDataTypes.h"
#include "AABB.h"

namespace spider3d
{
	namespace core
	{
		class Ray;
	}
}

namespace spider3d
{
	namespace picking
	{
		class OctreeNode;
	}
}

namespace spider3d
{
	namespace appearance
	{
		class Appearance;
	}
}

namespace spider3d
{
	namespace picking
	{
		class PickResult;
		class Octree;
	}
}

using namespace std;
using namespace spider3d::core;
using namespace spider3d::picking;
using namespace spider3d::appearance;

namespace spider3d
{
	namespace scene
	{
		class MeshBuffer;
		class IndexedMeshBuffer;
		class MeshNode;

		class Mesh
		{
		public:
			
			void constructOctree();

			void addMeshBuffer(MeshBuffer*);

			MeshBuffer& getMeshBuffer(su32);

			su32 getNumMeshBuffers();

			AABB& getAABB();

			void setAppearance(Appearance*);

			Appearance* getAppearance();

			void setAABB(AABB&);

			void reCalculateAABB();

			MeshNode* getMeshNode();
			
			sbool detach();

			Mesh();

			Mesh(const Mesh& other);

			Mesh& operator= (const Mesh& other);

			bool intersect(Ray& ray);

			virtual ~Mesh();

			friend class MeshNode;

		private:
			
			MeshNode* m_pMeshNode;
			Appearance *m_pAppearance;
			vector<OctreeNode*> *m_vecOctreeNodes;
			vector<MeshBuffer*> meshBuffers;
			Octree *m_Octree;
			AABB BoundingBox;

			bool pickTriangles(Ray ray, MeshBuffer* meshBuffer);
			bool pickIndexedTriangles(Ray ray, IndexedMeshBuffer* meshBuffer);
			bool pickQuads(Ray ray, MeshBuffer* indexBuffer);
			bool pickIndexedQuads(Ray ray, IndexedMeshBuffer* indexBuffer);
		};

	}

}
#endif