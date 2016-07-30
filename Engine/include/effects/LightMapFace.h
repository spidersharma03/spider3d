#ifndef _LIGHT_MAP_FACE_H_
#define _LIGHT_MAP_FACE_H_

#include "coreMath.h"
#include "Image.h"

using namespace spider3d::appearance;
using namespace spider3d::core;

#define MAX_LIGHTMAP_VERTICES 4
#define MAX_RESOLUTION 32
#define MIN_RESOLUTION 4

namespace spider3d
{
	namespace effects
	{
		class LightMapFace
		{
		public:
			static const enum LIGHTMAP_PRIMITIVE_TYPE { ELMPT_TRIANGLE, ELMPT_QUAD};
		public:
			LightMapFace(LIGHTMAP_PRIMITIVE_TYPE);
			Point3 m_lmVertices[MAX_LIGHTMAP_VERTICES];
			Point3 m_lmTexCoords[MAX_LIGHTMAP_VERTICES];
			Matrix4f TangentBasis;
			Image* m_LightMapImage;
			sf32 m_Width, m_Height;
			su16 m_LightMapWidth, m_LightMapHeight; 
			Point3 m_EdgeCoefficients[2];
			LIGHTMAP_PRIMITIVE_TYPE m_lmPrimitiveType;

			~LightMapFace();
		private:
			void createTangentBasis();
			void createLightMap();
			Color3 calculateColor();
			sbool isPointInside(int x, int y);
		};
	}
}

#endif