#ifndef HELICALPATH_H
#define HELICALPATH_H

#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class HelicalPath:public GeneralPath{

		private:
			int nloop;
			float radius;
			float pitch;

		public:
			HelicalPath(float , float , int , int , int );

		protected:
			void generateCoord();
		};

	}
}

#endif