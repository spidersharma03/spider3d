#include "cad/DirectionGenerator.h"

namespace spider3d
{
	namespace cad
	{

		DirectionGenerator::DirectionGenerator(GeneralPath* path):path(path)
		{
		}

		void DirectionGenerator::createDirections()
		{
			int i=0;
			float x ,y ,z ,d;
			npoints = path->getNumPoints();
			directions = path->getDirections();
			points = path->getPoints();

			for(i=0 ; i<3*npoints-3 ; i+=3){
				directions[i] = points[i+3] - points[i];
				directions[i+1] = points[i+4] - points[i+1];
				directions[i+2] = -(points[i+5] - points[i+2]);
			}
			directions[i] = directions[i-3] ;
			directions[i+1] = directions[i-2];
			directions[i+2] = directions[i-1];

			for(i=0 ; i<3*npoints ; i+=3){
				x = directions[i];
				y = directions[i+1];
				z = directions[i+2];
				d = (float) (sqrt(x*x + y*y + z*z) );
				directions[i] = x/d;
				directions[i+1] = y/d;
				directions[i+2] = z/d;	
			}

		};

		void DirectionGenerator::setPath(GeneralPath* path)
		{
			this->path = path;
		};

	}
}
