#ifndef TOROIDALSPIRALPATH_H
#define TOROIDALSPIRALPATH_H
#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class ToroidalSpiralPath: public GeneralPath
		{

		private:
			float radius;
			float a , b;
			int p , q;

		public:
			ToroidalSpiralPath(float , float, int , int , int, int);

		protected:
			void generateCoord(); 
		};

	}
}

#endif