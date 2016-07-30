#include "cad/Flower.h"
#include <math.h>

namespace spider3d
{
	namespace cad
	{

		Flower::Flower(float radius , int nleaves , int npoint , int flag)
			:GeneralShape2D(npoint , flag) , radius(radius) , nleaves(nleaves){
				if(npoint < 4)this->npoint = 4;
				createCoord();
		}

		void Flower::createCoord(){
			float x=0.0f , z=0.0f , r=0.0f;
			float theta=0.0f;
			float angle = (float) (2*M_PI/npoint );
			int k=0;
			float PSI = 0.0f;

			if( nleaves % 2 == 0 )PSI = (float)M_PI/2;
			if( nleaves % 2 != 0 )PSI = 0.0f;

			for(int i=0 ; i<3*npoint ; i+=3){
				r = absolute( ( radius* (float) ( cos(nleaves*theta + PSI) ) ) );
				coord[i] = (float) (r*cos(theta) );
				coord[i+1] = 0.0f;
				coord[i+2] = (float) (r*sin(theta) );
				theta += angle;	
			}
		} 	

		float Flower::absolute(float a){
			return a>0 ? a:-a;
		}

	}
}