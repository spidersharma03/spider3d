#ifndef _RENDERING_ATTRIBUTES_H

#define _RENDERING_ATTRIBUTES_H

#include "common/spiderDataTypes.h"
#include "common/e_comparisionfunction.h"

namespace spider3d
{

	namespace appearance
	{

		struct RenderingAttributes
		{
			sbool m_bAlphaTestEnable;

			COMPARISION_FUNCTION alphaTestFunc;
			
			sf32 alphaRefValue;

			RenderingAttributes()
			{
				m_bAlphaTestEnable = false;
				alphaTestFunc      = ECFN_NEVER;
				alphaRefValue	   = 0.0f;
			}

			RenderingAttributes(const RenderingAttributes& other)
			{
				m_bAlphaTestEnable = other.m_bAlphaTestEnable;
				alphaTestFunc      = other.alphaTestFunc;
				alphaRefValue	   = other.alphaRefValue;
			}
		};
	}
}

#endif