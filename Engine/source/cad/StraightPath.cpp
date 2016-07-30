#include "cad/StraightPath.h"

namespace spider3d
{
	namespace cad
	{

		StraightPath::StraightPath(float length , int npoints , int flag)
			:GeneralPath(npoints , flag){
				this->length = length;
				generateCoord();
		}	

		void StraightPath::generateCoord(){
			float x=0.0f;
			float delta=(float)(length/npoints);

			for(int i=0 ; i<3*npoints ; i+=3){
				points[i] = 0.0f;
				points[i+1] = x;
				points[i+2] = 0.0f;
				x += delta;
			}

		}	

	}
}
