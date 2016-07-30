#ifndef _LINE_ATTRIBUTES_H

#define _LINE_ATTRIBUTES_H

#include "common/spiderDataTypes.h"

namespace spider3d
{

	namespace appearance
	{

		struct LineAttributes
		{
            
			LineAttributes()
			{
               width = 1.0;
			   antiAliased = false;
			}

            LineAttributes(LineAttributes& other)
			{
				width = other.width;
				antiAliased = other.antiAliased;
			}

			LineAttributes& operator= (const LineAttributes& other)
			{
				width = other.width;
				antiAliased = other.antiAliased;
				return *this;
			}

			sbool operator!= ( LineAttributes& other )
			{
   				return !( width == other.width && antiAliased == other.antiAliased );
			}

			sbool operator== ( LineAttributes& other )
			{
   				return ( width == other.width && antiAliased == other.antiAliased );
			}

			sf32 width;

			sbool antiAliased;
		};

	}

}
#endif