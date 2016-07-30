#ifndef _LIGHTDATA_H

#define _LIGHTDATA_H

#include "math/coreMath.h"

using namespace spider3d::core;

namespace spider3d
{
	enum LIGHT_TYPE { POINTLIGHT = 0 , SPOTLIGHT , DIRECTIONALLIGHT };

	namespace appearance
	{

		class LightData
		{
		public:

			void set(LightData& lightData)
			{

			};

			LightData()
			{
				ambient.x = 0.1f ; ambient.y = 0.1f;ambient.z = 0.1f; ambient.w = 1.0f;
				diffuse.x = 0.7f ; diffuse.y = 0.7f ; diffuse.z = 0.7f; diffuse.w = 1.0f;
				specular.x = 0.7f ; specular.y = 0.7f ; specular.z = 0.7f; specular.w = 1.0f;
				type = POINTLIGHT;
				position.x = 1.0f;position.y = 5.0f;position.z = -2.0f;
				direction.x = 1.0;direction.y = 1.0;direction.z = 0.0;
				spotAngle = 45;
				concentration = 6.0f;
				attenuation.x = 1.0f;attenuation.y = 0.1f;attenuation.z = 0.1f;
			}

			Color4 ambient;

			Color4 diffuse;

			Color4 specular;

			Point3 position;

			Vector3 direction;

			Vector3 attenuation;

			sf32 spotAngle;

			sf32 concentration;

			LIGHT_TYPE type;

		};


	}

}
#endif