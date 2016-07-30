#ifndef _PICK_RESULT_H
#define _PICK_RESULT_H

#include "PickIntersection.h"

namespace spider3d
{
	namespace scene
	{
		class Node;
		class Mesh;
		class MeshBuffer;
	}
}

using namespace spider3d::scene;

namespace spider3d
{
	namespace picking
	{
		class PickResult
		{
		public:
			
			PickResult()
			{
				m_pNode = 0;
				m_pMesh = 0;
				m_PickDistanceSquared = 1000.0f;
				m_NumIntersections = 0;
			}

			inline PickIntersection& getIntersection()
			{
				return m_pickIntersection;
			}

			inline PickIntersection& getIntersection(int index)
			{
				return m_pickIntersection;
			}

			inline Node* getNode()
			{
				return m_pNode;
			}

			inline Mesh* getMesh()
			{
				return m_pMesh;
			}

			inline float getPickDistanceSquared()
			{
				return m_PickDistanceSquared;
			}

			inline int getNumIntersections()
			{
				return m_NumIntersections;
			}

			MeshBuffer* getMeshBuffer(si32);

			si32 getNumMeshBuffers();
			
			friend class spider3d::scene::Mesh;

		protected:
			PickIntersection m_pickIntersection;
			Node* m_pNode;
			Mesh* m_pMesh;
			int m_NumIntersections;
			float m_PickDistanceSquared;
		};
	}
}
#endif