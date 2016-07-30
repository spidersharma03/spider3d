#ifndef STRAIGHTPATH_H
#define STRAIGHTPATH_H

#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class StraightPath:public GeneralPath{

		private:
			float length;	

		public:
			StraightPath(float , int , int );

		protected:
			void generateCoord();
		};

	}
}

#endif