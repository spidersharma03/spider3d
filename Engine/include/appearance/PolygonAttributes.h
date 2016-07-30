#ifndef _POLYGON_ATTRIBUTES_H

#define _POLYGON_ATTRIBUTES_H

#include "common/spiderDataTypes.h"

namespace spider3d
{
	enum POLYGON_MODE { EPM_FILL = 0 , EPM_WIRE , EPM_POINT };

	enum CULL_FACE    { ECF_NONE = 0 , ECF_FRONT , ECF_BACK };

	namespace appearance
	{
		
		struct PolygonAttributes
		{
			POLYGON_MODE polygonMode;
             
			CULL_FACE    cullFace;

			sbool        normalFlip;

			sf32         polygonOffset;

            PolygonAttributes(const PolygonAttributes& other)
			{
				polygonMode = other.polygonMode;
				cullFace    = other.cullFace;
				normalFlip  = other.normalFlip;
			}

			PolygonAttributes& operator= (const PolygonAttributes& other)
			{
				polygonMode = other.polygonMode;
				cullFace    = other.cullFace;
				normalFlip  = other.normalFlip;
				return *this;
			}

			PolygonAttributes()
			{
                polygonMode = EPM_FILL;
				cullFace    = ECF_BACK;
				normalFlip  = false;  
			}

			sbool operator!= (PolygonAttributes& other)
			{
				return !( polygonMode==other.polygonMode && cullFace==other.cullFace
					&& normalFlip==other.normalFlip);
			}

			sbool operator== (PolygonAttributes& other)
			{
				return ( polygonMode==other.polygonMode && cullFace==other.cullFace
					&& normalFlip==other.normalFlip);
			}

		};

	}
}
#endif