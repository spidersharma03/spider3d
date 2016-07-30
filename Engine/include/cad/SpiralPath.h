#ifndef SPIRALPATH_H
#define SPIRALPATH_H

#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class SpiralPath:public GeneralPath{

		private:
			int nloop;
			float pitch;

		public:  
			SpiralPath(int , int , float ,  int );

		protected:
			void generateCoord();
		};

	}
}

#endif