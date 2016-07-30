#ifndef CIRCULARPATH_H
#define CIRCULARPATH_H

#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class CircularPath: public GeneralPath
		{

		private:
			int nloop;
			float radius , arcangle;

		public:
			CircularPath(float , float ,  int , int );

		protected:

			virtual void generateCoord();

		};

	}
}

#endif