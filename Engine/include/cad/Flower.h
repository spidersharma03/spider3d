#ifndef FLOWER_H
#define FLOWER_H

#include "GeneralShape2D.h"

namespace spider3d
{
	namespace cad
	{

		class Flower:public GeneralShape2D{

		private:
			float radius;
			int nleaves;
			float absolute(float);
		public:

			Flower(float , int , int , int );

		protected:
			void createCoord();

		};

	}
}
#endif