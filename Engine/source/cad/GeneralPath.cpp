#include "cad/GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		GeneralPath::GeneralPath(int npoints , int flag)
		{
			this->npoints = npoints;
			this->flag = flag;
			points = new float[3*npoints];
			directions = new float[3*npoints];
			scaleFactors = new float[3*npoints];
			for(int i=0; i<3*npoints ;i++)
				scaleFactors[i]=1; 
		}

		void GeneralPath::setPoints(float* points)
		{
			for(int i=0; i<3*npoints; i++)
				this->points[i] = points[i];
		}

		int GeneralPath::getNumPoints()
		{
			return npoints;
		}  

		int GeneralPath::getFlag()
		{
			return flag;
		}

		float* GeneralPath::getPoints()
		{
			return points;
		}

		float* GeneralPath::getDirections()
		{
			return directions;
		}

		float* GeneralPath::getPoint(int index)
		{
			point[0] = points[3*index];
			point[1]= points[3*index + 1];
			point[2]= points[3*index + 2];
			return point;
		}

		float* GeneralPath::getDirection(int index)
		{
			direction[0] = directions[3*index];
			direction[1]= directions[3*index + 1];
			direction[2]= directions[3*index + 2];
			return direction;
		}

		float GeneralPath::getSegmentLength(int nseg)
		{
			float x0 , x1 , x2 , x3 , x4 , x5 , x , y , z ;
			x5 = points[3*nseg+5] ;x4 = points[3*nseg+4];x3 = points[3*nseg+3];
			x2 = points[3*nseg+2] ;x1 = points[3*nseg+1];x0 = points[3*nseg]; 	
			x = x3 - x0; y = x4 - x1; z = x5 - x2;
			return (float)(sqrt(x*x + y*y + z*z));
		}

		float GeneralPath::getLength()
		{
			float len = 0.0f;
			for(int i=0 ; i<npoints-1 ; i++)
				len += getSegmentLength(i);
			length = len;
			return length;
		}


		float* GeneralPath::getScaleFactor(int index)
		{
			scale[0] = scaleFactors[3*index];
			scale[1] = scaleFactors[3*index+1];
			scale[2] = scaleFactors[3*index+2];
			return scale;
		}

		void GeneralPath::setScaleFactors(ScaleModulator& modulator)
		{
			scaleFactors = modulator.getScales();
		}

		void GeneralPath::setCoord(float* coord)
		{
			for(int i=0; i<3*npoints; i++)
				points[i] = coord[i];
		};

	}
}


