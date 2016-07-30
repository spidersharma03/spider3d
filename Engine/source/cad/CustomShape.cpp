#include "cad/CustomShape.h"
#include <math.h>

namespace spider3d
{
	namespace cad
	{
		CustomShape::CustomShape(float stepHeight, float stepWidth, int numSteps, int flags)
			:GeneralShape2D((2*numSteps+1) , flags),m_stepHeight(stepHeight),m_stepWidth(stepWidth),m_numSteps(numSteps)
		{
				if(npoint < 3)this->npoint = 3;
				createCoord();
		}

		void CustomShape::createCoord()
		{
			/*
			    stepWidth
			   2---------3
			   |
			   | <--stepHeight
			   |
			  1	
			*/
			// 1st point
			coord[0] = 0.0f; coord[1] = 0.0f; coord[2] = 0.0f;
			// 2nd point
			coord[3] = 0.0f; coord[4] = 0.0f; coord[5] = m_stepHeight;
			// 4th point
			coord[6] = m_stepWidth; coord[7] = 0.0f; coord[8] = m_stepHeight;

			for( int i=9; i<3*npoint; i+=3 )
			{
				coord[i] = m_stepWidth + coord[i-9];
				coord[i+1] = 0.0f;
				coord[i+2] = m_stepHeight + coord[i+2-9];
			}
		} 	

	}
}
