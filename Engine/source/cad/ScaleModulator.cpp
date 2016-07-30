#include "cad/ScaleModulator.h"
#include <stdlib.h>

namespace spider3d
{
	namespace cad
	{

		ScaleModulator::ScaleModulator(int npoints):npoints(npoints)
		{
			scales = new float[3*npoints];
			for(int i=0; i<3*npoints; i++)
				scales[i] = 1.0;
		}

		int ScaleModulator::getNumPoints()
		{
			return npoints;
		}

		float* ScaleModulator::getScales()
		{
			return scales;
		}	

		void ScaleModulator::setScales(float* scales)
		{
			for(int i=0; i<3*npoints; i++)
				this->scales[i] = scales[i];
		}	

		void ScaleModulator::setLinearScale(float start , float end)
		{
			float d = (end - start)/npoints;
			for(int i=0 ; i<3*npoints ; i+=3){
				scales[i] = start + i*d/3;
				scales[i+1] = start + i*d/3; 
				scales[i+2] = start + i*d/3;}
		}

		void ScaleModulator::setHarmonicScale(float mean , float delta ,  int period )
		{
			float angle = 0.0;
			float Theta = (float)(period*2*M_PI/npoints);
			for(int i=0 ; i<3*npoints ; i+=3){
				float d = mean + (float)(delta*sin(angle)/2);
				scales[i] = d;
				scales[i+1] = d; 
				scales[i+2] = d;
				angle += Theta;
			}

		}

		void ScaleModulator::setRandomScale(float mean , float perturb)
		{
			//if(perturb>mean)perturb = mean; 
			for(int i=0 ; i<3*npoints ; i+=3){
				float d = mean + (float)((rand()/RAND_MAX - 0.5)*perturb);
				scales[i] = d;
				scales[i+1] = d; 
				scales[i+2] = d;
				//angle += Theta;
			}
		}

	}
}


