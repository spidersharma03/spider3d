#include "cad/SpiralPath.h"

namespace spider3d
{
	namespace cad
	{

		SpiralPath::SpiralPath(int npoints , int nloop , float pitch  , int flag)
			:GeneralPath(npoints , flag),nloop(nloop) , pitch(pitch){
				generateCoord();
		}	

		void SpiralPath::generateCoord(){
			float angle=0.0f , r;
			float theta=(float)( (nloop+1)*M_PI/npoints);
			points = new float[3*npoints];
			directions = new float[3*npoints];

			for(int i=0 ; i<3*npoints ; i+=3){
				r = pitch*angle;
				points[i] = r*(float)(cos(angle));
				points[i+1] = r*(float)(sin(angle));
				points[i+2] = 0.0f;
				angle += theta;
			}
		}	

	}
}