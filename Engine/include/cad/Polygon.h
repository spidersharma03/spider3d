#ifndef POLYGON_H
#define POLYGON_H

#include "GeneralShape2D.h"

namespace spider3d
{
	namespace cad
	{

		class Polygon:public GeneralShape2D
		{

		private:
			float radius;

		public:
			Polygon(float , int , int );

		protected:
			void createCoord();
		};

	}
}

#endif

