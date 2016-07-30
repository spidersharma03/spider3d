#ifndef SCALEMODULATOR_H
#define SCALEMODULATOR_H
#include "coreMath.h"
#include <math.h>

using namespace spider3d::core;

namespace spider3d
{
	namespace cad
	{

		class ScaleModulator
		{

		private:
			float* scales;
			int npoints;

		public:
			ScaleModulator(int);

			int getNumPoints();

			float* getScales();

			void setScales(float*);

			void setLinearScale(float , float);

			void setHarmonicScale(float , float ,  int );

			void setRandomScale(float , float);
		};

	}
}

#endif