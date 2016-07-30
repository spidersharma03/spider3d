#ifndef GENERALPATH_H
#define GENERALPATH_H

#include "ScaleModulator.h"

namespace spider3d
{
	namespace cad
	{

		class GeneralPath
		{

		public:
			enum { CAP_START = 0 , CAP_END = 1 , CAP_START_END = 2 , CAP_NONE = 3};

		protected:
			int npoints , flag;
			float* points;
			float* directions;
			float* scaleFactors;
			float length;
			float point[3] , direction[3] , scale[3];

		public:
			GeneralPath(int , int );
			void setPoints(float* );
			int getNumPoints();
			int getFlag();
			float* getPoints();
			float* getDirections();
			float* getPoint(int);
			float* getDirection(int);
			void calculateDirections();
			float* getScaleFactor(int);
			float getSegmentLength(int);
			float getLength();
			void setScaleFactors(ScaleModulator &);
			void setCoord(float* coord);

		protected:

			void normalize();

			virtual void generateCoord() = 0;
		};

	}
}
#endif