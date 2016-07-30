#ifndef _RECTANGLE_H

#define _RECTANGLE_H

#include "spiderDataTypes.h"

namespace spider3d
{

	namespace core
	{

		class Rectangle
		{
		public:

			Rectangle():x(0),y(0),width(0),height(0)
			{};

			Rectangle(sf32 x , sf32 y , sf32 width , sf32 height):x(x),y(y),width(width),height(height)
			{};

			Rectangle(Rectangle& rect):x(rect.x),y(rect.y),width(rect.width),height(rect.height)
			{};

			void set(sf32 x , sf32 y , sf32 width , sf32 height)
			{
				this->x  = x;
				this->y   = y;
				this->width = width;
				this->height = height;
			};

			void set(Rectangle& rect)
			{
				x  = rect.x;
				y  = rect.y;
				width = rect.width;
				height = rect.height;
			};

			sf32 x , y;
			sf32 width , height;

		};

	}
}

#endif