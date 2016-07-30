#ifndef _MATERIAL_H

#define _MATERIAL_H

#include "math/coreMath.h"

using namespace spider3d::core;

namespace spider3d
{
	enum COLOR_TARGET 
	{ 
		ECT_NONE = 0,
		ECT_DIFFUSE = 0x1201 ,
		ECT_AMBIENT = 0x1200 ,
		ECT_DIFFUSE_AND_AMBIENT = 0x1602,
		ECT_SPECULAR = 0x1202,
		ECT_EMMISSIVE = 0x1600
	};

	namespace appearance
	{
		class Material
		{
		public:
			Material()
			{
				ambient.set(0.1f,0.1f,0.1f,1.0f);
				diffuse.set(0.9f,0.9f,0.9f,1.0f);
				specular.set(0.7f,0.7f,0.7f,1.0f);
				emissive.set(0.0f,0.0f,0.0f,1.0f);
				shininess = 50.0f;
                _mLightingEnabled = true;
				colorTarget    = ECT_DIFFUSE;
				useColorMaterial = false;
			}

			Material(const Material& other)
			{
				ambient = other.ambient;
				diffuse = other.diffuse;
				specular = other.specular;
				emissive = other.emissive;
				shininess = other.shininess;
				_mLightingEnabled = other._mLightingEnabled;
				colorTarget = other.colorTarget;
				useColorMaterial = other.useColorMaterial;
			}

			Material& operator= (const Material& other)
			{
				ambient = other.ambient;
				diffuse = other.diffuse;
				specular = other.specular;
				emissive = other.emissive;
				shininess = other.shininess;
				_mLightingEnabled = other._mLightingEnabled;
				colorTarget = other.colorTarget;
				useColorMaterial = other.useColorMaterial;
				return *this;
			}

			sbool operator!= ( Material& other)
			{
				return !( (ambient == other.ambient) && (diffuse == other.diffuse)
					    &&(specular == other.specular) && (emissive == other.emissive)  
						&& (shininess == other.shininess) 
						&& (_mLightingEnabled == other._mLightingEnabled)
						&& (colorTarget == other.colorTarget)
						&& (useColorMaterial == other.useColorMaterial) );
			}

			sbool operator== ( Material& other)
			{
				return ( (ambient == other.ambient) && (diffuse == other.diffuse)
					    &&(specular == other.specular) && (emissive == other.emissive)  
						&& (shininess == other.shininess) 
						&& (_mLightingEnabled == other._mLightingEnabled)
						&& (colorTarget == other.colorTarget)
						&& (useColorMaterial == other.useColorMaterial) );
			}

			virtual ~Material()
			{
			}

			Color4 ambient;
			Color4 diffuse;
			Color4 specular;
			Color4 emissive;
			sf32 shininess;
			COLOR_TARGET colorTarget;
			sbool _mLightingEnabled;
            sbool useColorMaterial; 
		};
	}
}
#endif