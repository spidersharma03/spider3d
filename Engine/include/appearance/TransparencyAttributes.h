#ifndef _TRANSPARENCY_ATTRIBUTES_H

#define _TRANSPARENCY_ATTRIBUTES_H

#include "common/spiderDataTypes.h"

namespace spider3d
{
	enum TRANSPARENCY_MODE { ETM_BLEND = 0 , ETM_SCREEN_DOOR , ETM_NONE };
    
	enum BLEND_FACTOR { EBF_BLEND_ZERO = 0 , EBF_BLEND_ONE , EBF_BLEND_SRC_ALPHA = 0x0302 ,
		                EBF_BLEND_ONE_MINUS_SRC_ALPHA = 0x0303, EBF_BLEND_DST_COLOR = 0x0306,
						EBF_BLEND_ONE_MINUS_DST_COLOR = 0x0307, EBF_BLEND_SRC_COLOR = 0x0300,
						EBF_BLEND_ONE_MINUS_SRC_COLOR = 0x0301, EBF_BLEND_DST_ALPHA = 0x0304,
						EBF_BLEND_ONE_MINUS_DST_ALPHA = 0x0305, EBF_BLEND_CONST_COLOR = 0x8001,
                        EBF_BLEND_ONE_MINUS_CONST_COLOR = 0x8002,  
						EBF_BLEND_CONST_ALPHA = 0x8003 , EBF_BLEND_ONE_MINUS_CONST_ALPHA = 0x8004,
	};

	namespace appearance
	{

		struct TransparencyAttributes
		{
			sf32 transparency;
            
			sbool m_doublePass;

            TRANSPARENCY_MODE mode;

			BLEND_FACTOR srcBlendFactor;
			
			BLEND_FACTOR dstBlendFactor;

			TransparencyAttributes()
			{
               transparency = 0.0f;
			   m_doublePass = false;
			   mode = ETM_NONE;
			   srcBlendFactor = EBF_BLEND_ONE;
			   dstBlendFactor = EBF_BLEND_ZERO;
			}

			TransparencyAttributes(const TransparencyAttributes& other)
			{
				transparency    = other.transparency;
				m_doublePass    = other.m_doublePass;
				mode            = other.mode;
				srcBlendFactor  = other.srcBlendFactor;
				dstBlendFactor  = other.dstBlendFactor;
			}

			TransparencyAttributes& operator= (const TransparencyAttributes& other)
			{
				transparency    = other.transparency;
				m_doublePass    = other.m_doublePass;
				mode            = other.mode;
				srcBlendFactor  = other.srcBlendFactor;
				dstBlendFactor  = other.dstBlendFactor;
				return *this;
			}

			sbool operator!= ( TransparencyAttributes& other )
			{
   				return !( transparency == other.transparency && mode == other.mode 
					    && srcBlendFactor == other.srcBlendFactor && dstBlendFactor == other.dstBlendFactor);
			}
			
			sbool operator== ( TransparencyAttributes& other )
			{
   				return ( transparency == other.transparency && mode == other.mode 
					    && srcBlendFactor == other.srcBlendFactor && dstBlendFactor == other.dstBlendFactor);
			}
		};

	}
}
#endif