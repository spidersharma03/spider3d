#ifndef _TEXCOORD_GEN_H
#define _TEXCOORD_GEN_H

#include "math/coreMath.h"

using namespace spider3d::core;

namespace spider3d
{
	enum TEXTURE_COMPONENT_FORMAT { ETCF_2, ETCF_3, ETCF_4};
	enum TEX_GEN_MODE { ETGM_OBJ_LINEAR, ETGM_EYE_LINEAR, ETGM_SPHERE_MAP, ETGM_NORMAL_MAP, ETGM_REFLECTION_MAP };
	namespace appearance
	{
		struct TexCoordGeneration
		{
			TEXTURE_COMPONENT_FORMAT texComponentFormat;
			TEX_GEN_MODE texGenMode;
			bool bEnable;
			//Vector4 planeS, planeT, planeR, planeQ;
			float planeS[4];
			float planeT[4];
			float planeR[4];
			float planeQ[4];

			TexCoordGeneration()
			{
				texComponentFormat = ETCF_2;
				texGenMode = ETGM_OBJ_LINEAR;
				bEnable    = false;
				planeS[0] = 1.0f; planeS[1] = 0.0f; planeS[2] = 0.0f; planeS[3] = 0.0f;
				planeT[0] = 0.0f; planeT[1] = 1.0f; planeT[2] = 0.0f; planeT[3] = 0.0f;
				planeR[0] = 0.0f; planeR[1] = 0.0f; planeR[2] = 1.0f; planeR[3] = 0.0f;
				planeQ[0] = 0.0f; planeQ[1] = 0.0f; planeQ[2] = 0.0f; planeQ[3] = 1.0f;
			}

			TexCoordGeneration(TEXTURE_COMPONENT_FORMAT format):texComponentFormat(texComponentFormat)
			{
				texGenMode = ETGM_OBJ_LINEAR;
				bEnable    = false;
				planeS[0] = 1.0f; planeS[1] = 0.0f; planeS[2] = 0.0f; planeS[3] = 0.0f;
				planeT[0] = 0.0f; planeT[1] = 1.0f; planeT[2] = 0.0f; planeT[3] = 0.0f;
				planeR[0] = 0.0f; planeR[1] = 0.0f; planeR[2] = 1.0f; planeR[3] = 0.0f;
				planeQ[0] = 0.0f; planeQ[1] = 0.0f; planeQ[2] = 0.0f; planeQ[3] = 1.0f;
			}

			TexCoordGeneration(TEXTURE_COMPONENT_FORMAT texComponentFormat, TEX_GEN_MODE texGenMode):texComponentFormat(texComponentFormat),texGenMode(texGenMode)
			{
				bEnable    = false;
				planeS[0] = 1.0f; planeS[1] = 0.0f; planeS[2] = 0.0f; planeS[3] = 0.0f;
				planeT[0] = 0.0f; planeT[1] = 1.0f; planeT[2] = 0.0f; planeT[3] = 0.0f;
				planeR[0] = 0.0f; planeR[1] = 0.0f; planeR[2] = 1.0f; planeR[3] = 0.0f;
				planeQ[0] = 0.0f; planeQ[1] = 0.0f; planeQ[2] = 0.0f; planeQ[3] = 1.0f;
			}

			TexCoordGeneration(TEXTURE_COMPONENT_FORMAT format, TEX_GEN_MODE texGenMode
				, float* planeS , float* planeT , float* planeR , float* planeQ	):texComponentFormat(texComponentFormat),texGenMode(texGenMode)
			{
				bEnable    = false;
				this->planeS[0] = planeS[0]; this->planeS[1] = planeS[1]; this->planeS[2] = planeS[2]; this->planeS[3] = planeS[3];
				this->planeT[0] = planeT[0]; this->planeT[1] = planeT[1]; this->planeT[2] = planeT[2]; this->planeT[3] = planeT[3];
				this->planeR[0] = planeR[0]; this->planeR[1] = planeR[1]; this->planeR[2] = planeR[2]; this->planeR[3] = planeR[3];
				this->planeQ[0] = planeQ[0]; this->planeQ[1] = planeQ[1]; this->planeQ[2] = planeQ[2]; this->planeQ[3] = planeQ[3];
			}

			bool operator != ( TexCoordGeneration& other)
			{
				return !( texGenMode == other.texGenMode && texComponentFormat == other.texComponentFormat &&
						bEnable   == other.bEnable   &&
						planeS[0] == other.planeS[0] &&
					    planeS[1] == other.planeS[1] && planeS[2] == other.planeS[2] &&
						planeS[3] == other.planeS[3] && planeT[0] == other.planeT[0] &&
						planeT[1] == other.planeT[1] && planeT[2] == other.planeT[2] &&
						planeR[0] == other.planeR[0] && planeR[1] == other.planeR[1] &&
						planeR[2] == other.planeR[2] && planeR[3] == other.planeR[3] &&
						planeT[0] == other.planeT[0] && planeT[1] == other.planeT[1] &&
						planeT[2] == other.planeT[2] && planeT[3] == other.planeT[3] );
			}

			void setEnable(bool bEnable)
			{
				this->bEnable = bEnable;
			}
		};
	}
}
#endif