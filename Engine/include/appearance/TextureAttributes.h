#ifndef _TEXTURE_ATTRIBUTES_H

#define _TEXTURE_ATTRIBUTES_H

#include "math/coreMath.h"

using namespace spider3d::core;

namespace spider3d
{

	enum TEXTURE_MODE { ETEXM_MODULATE = 0 , ETEXM_BLEND , ETEXM_DECAL , ETEXM_REPLACE, ETEXM_COMBINE };

	enum TEXTURE_COMBINE_MODE { ETCM_MODULATE, ETCM_REPLACE, ETCM_ADD, ETCM_ADD_SIGNED, ETCM_SUBTRACT, ETCM_INTERPOLATE, ETCM_DOT3 };

	enum TEXTURE_COMBINE_SOURCE { ETCS_COMBINE_OBJECT_COLOR, ETCS_COMBINE_TEXTURE_COLOR, ETCS_COMBINE_CONSTANT_COLOR, ETCS_COMBINE_PREVIOUS_TEXTURE_UNIT_STATE };

	enum TEXTURE_COMBINE_RGB_OPERAND {ETC_RGBO_COMBINE_SRC_COLOR, ETC_RGBO_COMBINE_ONE_MINUS_SRC_COLOR, ETC_RGBO_COMBINE_SRC_ALPHA, ETC_RGBO_COMBINE_ONE_MINUS_SRC_ALPHA };

	enum TEXTURE_COMBINE_ALPHA_OPERAND {ETC_ALPHAO_COMBINE_SRC_ALPHA, ETC_ALPHAO_COMBINE_ONE_MINUS_SRC_ALPHA};

	namespace appearance
	{
		struct TextureAttributes
		{
            TEXTURE_MODE texture_mode;
			TEXTURE_COMBINE_MODE texture_Combine_RGB_Mode;
			TEXTURE_COMBINE_MODE texture_Combine_Alpha_Mode;
			
			TEXTURE_COMBINE_SOURCE texture_Combine_RGB_Source[3];
			TEXTURE_COMBINE_SOURCE texture_Combine_Alpha_Source[3];
			TEXTURE_COMBINE_RGB_OPERAND texture_Combine_RGB_Operand[3];
			TEXTURE_COMBINE_ALPHA_OPERAND texture_Combine_Alpha_Operand[3];
			
			int texture_Combine_RGB_Scale;
			int texture_Combine_Alpha_Scale;

			float texture_Constant_Color[4];

			Matrix4f texture_transform;

			TextureAttributes()
			{
			   texture_mode = ETEXM_REPLACE;
			   texture_Combine_RGB_Mode	  = ETCM_ADD;
			   texture_Combine_Alpha_Mode = ETCM_ADD;
			   for( int i = 0 ; i < 3; i++ )
			   {
					texture_Combine_RGB_Source[i] = ETCS_COMBINE_TEXTURE_COLOR;
					texture_Combine_Alpha_Source[i] = ETCS_COMBINE_OBJECT_COLOR;
					texture_Combine_RGB_Operand[i] = ETC_RGBO_COMBINE_SRC_COLOR;
					texture_Combine_Alpha_Operand[i] = ETC_ALPHAO_COMBINE_SRC_ALPHA;
			   }
			   texture_Combine_RGB_Source[1] = ETCS_COMBINE_PREVIOUS_TEXTURE_UNIT_STATE;
			}

			TextureAttributes(const TextureAttributes& other)
			{
				texture_mode    = other.texture_mode;
				texture_Combine_RGB_Mode = other.texture_Combine_RGB_Mode;
				texture_Combine_Alpha_Mode = other.texture_Combine_Alpha_Mode;
			}

			TextureAttributes& operator= (const TextureAttributes& other)
			{
				texture_mode    = other.texture_mode;
				texture_Combine_RGB_Mode = other.texture_Combine_RGB_Mode;
				texture_Combine_Alpha_Mode = other.texture_Combine_Alpha_Mode;
				return *this;
			}

			sbool operator!= ( TextureAttributes& other )
			{
   				return !( texture_mode == other.texture_mode);
			}
			
			sbool operator== ( TextureAttributes& other )
			{
   				return ( texture_mode == other.texture_mode);
			}
		};

	}
}
#endif