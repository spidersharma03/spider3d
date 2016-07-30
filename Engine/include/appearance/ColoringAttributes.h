#ifndef _COLORING_ATTRIBUTES_H

#define _COLORING_ATTRIBUTES_H

#include "common/spiderDataTypes.h"

namespace spider3d
{
	enum SHADE_MODEL { ESM_FLAT = 0 , ESFN_GOURAUD };

	namespace appearance
	{

		struct ColoringAttributes
		{

			sbool ColorRedMask;

			sbool ColorGreenMask;

			sbool ColorBlueMask;

			sbool ColorAlphaMask;
            
			SHADE_MODEL ShadeModel;

			ColoringAttributes()
			{
				ColorRedMask = true;
				ColorGreenMask = true;
				ColorBlueMask = true;
				ColorAlphaMask = true;
				ShadeModel  = ESFN_GOURAUD;
			}

			ColoringAttributes(ColoringAttributes& other)
			{
				ColorRedMask = other.ColorRedMask;
				ColorGreenMask = other.ColorGreenMask;
				ColorBlueMask = other.ColorBlueMask;
				ColorAlphaMask = other.ColorAlphaMask;
				ShadeModel     = other.ShadeModel;
			}

			ColoringAttributes& operator= (const ColoringAttributes& other)
			{
				ColorRedMask = other.ColorRedMask;
				ColorGreenMask = other.ColorGreenMask;
				ColorBlueMask = other.ColorBlueMask;
				ColorAlphaMask = other.ColorAlphaMask;
				ShadeModel     = other.ShadeModel;
				return *this;
			}

			sbool operator!= (ColoringAttributes& other)
			{
				return ( (ColorRedMask != other.ColorRedMask) || 
					( ColorGreenMask != other.ColorGreenMask) ||
					( ColorBlueMask  != other.ColorBlueMask) ||
					( ColorAlphaMask != other.ColorAlphaMask) ||
					( ShadeModel     != other.ShadeModel) );
			}

			sbool operator== (ColoringAttributes& other)
			{
				return ( (ColorRedMask == other.ColorRedMask) && 
					(ColorGreenMask == other.ColorGreenMask) &&
					(ColorBlueMask  == other.ColorBlueMask) &&
					(ColorAlphaMask == other.ColorAlphaMask) &&
					( ShadeModel    == other.ShadeModel) );
			}

		};

	}

}
#endif