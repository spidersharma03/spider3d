#include "cad/HelicalPath.h"

namespace spider3d
{
	namespace cad
	{

		HelicalPath::HelicalPath(float radius , float pitch , int npoints , int nloop , int flag)
			:GeneralPath(npoints , flag) , radius(radius) , pitch(pitch) , nloop(nloop){
				generateCoord();
		}	

		void HelicalPath::generateCoord(){
			float angle=0.0f , r = 0.0;
			float theta=(float)( (nloop+1)*M_PI/npoints);

			for(int i=0 ; i<3*npoints ; i+=3){
				points[i] = radius*(float)(cos(angle));
				points[i+1] = radius*(float)(sin(angle));
				points[i+2] = pitch*angle;
				angle += theta;
			}
		}	

	}
}