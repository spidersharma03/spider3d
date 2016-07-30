#ifndef _DEPTH_ATTRIBUTES_H

#define _DEPTH_ATTRIBUTES_H

#include "common/spiderDataTypes.h"
#include "common/e_comparisionfunction.h"

namespace spider3d
{
	namespace appearance
	{

		struct DepthAttributes
		{
			sf32 MinRange;

			sf32 MaxRange;

			COMPARISION_FUNCTION CompareFunc;

			sbool DepthTest;

			sbool DepthMask;


			DepthAttributes()
			{
				MinRange = 0.001f;
				MaxRange = 100.0f;
				CompareFunc = ECFN_LESS_OR_EQUAL;
				DepthTest  = true;
				DepthMask  = true;
			}

			DepthAttributes(const DepthAttributes& other )
			{
				MinRange = other.MinRange;
				MaxRange = other.MaxRange;
				CompareFunc = other.CompareFunc;
				DepthTest  = other.DepthTest;
				DepthMask  = other.DepthMask;
			}

			DepthAttributes& operator= (const DepthAttributes& other )
			{
				MinRange = other.MinRange;
				MaxRange = other.MaxRange;
				CompareFunc = other.CompareFunc;
				DepthTest  = other.DepthTest;
				DepthMask  = other.DepthMask;
				return *this;
			}

			sbool operator== (DepthAttributes& other )
			{
				return ( MinRange == other.MinRange &&
					MaxRange == other.MaxRange &&
					CompareFunc == other.CompareFunc &&
					DepthTest == other.DepthTest &&
					DepthMask == other.DepthMask );
			}

			sbool operator!= (DepthAttributes& other )
			{
				return ( MinRange != other.MinRange ||
					MaxRange != other.MaxRange ||
					CompareFunc != other.CompareFunc ||
					DepthTest != other.DepthTest ||
					DepthMask != other.DepthMask );
			}


		};

	}
}
#endif