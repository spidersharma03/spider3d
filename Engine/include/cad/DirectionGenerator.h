#ifndef DIRECTIONGENERATOR_H
#define DIRECTIONGENERATOR_H

#include "GeneralPath.h"

namespace spider3d
{
	namespace cad
	{

		class DirectionGenerator
		{
		private:
			GeneralPath* path;
			float* directions;
			float* points;
			int npoints;
			void normalize();
		public:
			DirectionGenerator(GeneralPath*);
			void setPath(GeneralPath*);
			void createDirections();

		};

	}
}

#endif