#ifndef _MESHNODE_H

#define _MESHNODE_H

#include "Node.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "PickResult.h"

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
		class Octree;
		class OctreeNode;
	}
}

using namespace spider3d::appearance;
using namespace spider3d::picking;

namespace spider3d
{
	namespace scene
	{

		class MeshNode:public Node
		{
		public:

			virtual void render();

			virtual void OnRegister();

			virtual void print();

			AABB& getLocalAABB()  ;

			void setMesh(Mesh*, sbool bOptimizePicking = false);

			void addMesh(Mesh*, sbool bOptimizePicking = false);

			sbool removeMesh(Mesh*);

			void removeAllMeshes();

			void setDebugDraw(sbool);

			Mesh* getMesh();

			Mesh* getMesh(si32 index);

			vector<Mesh*>& getAllMeshes();

			si32 getNumMeshes();

			sbool isAppearanceOverridden();

			void setAppearanceOverrideEnable(sbool);

			void setAppearance(Appearance* appearance);

			Appearance* getAppearance();

			MeshNode(Nodeptr, SceneManagerptr);

			MeshNode(SceneManagerptr);

			friend class Mesh;
		
			Point3 pickPoint;
			Point3 *pickPrimitive;
			Vector3 pickNormal;
		protected:
			PickResult* intersectRay(Ray& ray);
			Octree *m_Octree;
			sbool m_bOptimizePicking;
			Mesh* mesh;
			vector<Mesh*> m_vecMeshes;
			si32 m_NumMeshes;
			Appearance* m_pAppearance;
			sbool m_bAppearanceOverride;
			sbool DebugDraw;
			PickResult m_pickResult;
		};


	}

}

#endif 