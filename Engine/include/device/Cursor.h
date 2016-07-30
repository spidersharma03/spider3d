#ifndef _CURSOR_H
#define _CURSOR_H

#include "spiderDataTypes.h"

namespace spider3d
{

	class Cursor
	{
	public:
		Cursor():x(0),y(0)
		{
		}

		virtual void setVisible(sbool bVisible) = 0;

		inline sbool getVisible()
		{
			return bVisible;
		}

		virtual void setPosition(si32 x , si32 y ) = 0;

		virtual void setRelativePosition(sf32 x , sf32 y ) = 0;

		virtual void getPosition(si32* x, si32* y) = 0;

		virtual void getRelativePosition(sf32* x, sf32* y) = 0;

	protected:
		sbool bVisible;
		sf32 x, y;
	};

}
#endif