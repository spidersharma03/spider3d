#ifndef _PICK_INTERSECTION_H
#define _PICK_INTERSECTION_H

#include "math/coreMath.h"

using namespace spider3d::core;

namespace spider3d
{
	namespace scene
	{
		class Mesh;
	}
}

using namespace spider3d::scene;

namespace spider3d
{
	namespace picking
	{
		class PickIntersection
		{
		public:
			PickIntersection()
			{
				m_Indices[0] = m_Indices[1] = m_Indices[2] = m_Indices[3] = 0;
				m_PrimitiveType = spider3d::EPT_POINTS;
			}

			inline Point3 getIntersectionPointLocal()
			{
				return m_pickPointLocal;
			}

			inline Point3 getTextureCoordinates()
			{
				return m_TextureCoordinate;
			}

			inline Vector3 getIntersectionNormalLocal()
			{
				return m_NormalLocal;
			}

			inline Vector3 getIntersectionNormalGlobal()
			{
				return m_NormalGlobal;
			}

			inline Point3 getIntersectionPointWorld()
			{
				return m_pickPointGlobal;
			}
			
			inline Point3* getIntersectedPrimitiveCoordinatesLocal()
			{
				return m_PrimitiveCoordinatesLocal;
			}

			inline Point3* getIntersectedPrimitiveCoordinatesWorld()
			{
				return m_PrimitiveCoordinatesWorld;
			}

			inline void getIndices(unsigned short* indices)
			{

			}

			inline spider3d::PRIMITIVE_TYPE getPrimitiveType()
			{
				return m_PrimitiveType;
			}

			friend class spider3d::scene::Mesh;

		protected:

			Point3 m_pickPointLocal;
			Point3 m_pickPointGlobal;

			Point3 m_TextureCoordinate;

			Vector3 m_NormalLocal;
			Vector3 m_NormalGlobal;

			unsigned short m_Indices[4];
			Point3 m_PrimitiveCoordinatesLocal[4];
			Point3 m_PrimitiveCoordinatesWorld[4];

			spider3d::PRIMITIVE_TYPE m_PrimitiveType;
		};
	}
}

#endif