#ifndef GENERALSHAPE2D_H
#define GENERALSHAPE2D_H

#include "coreMath.h"
using namespace spider3d::core;

namespace spider3d
{
	namespace cad
	{

		class GeneralShape2D
		{
		public:
			enum {OPEN = 0  , CLOSED = 1};
		protected:
			int npoint , flag;
			float* coord;
			float* transcoord;

		private:
			float *matrix , *v4 , *v5, *v , *zaxis;
			Matrix4f transform;
			//Vector3 v4, v5, v, zaxis;

		public:
			GeneralShape2D(int , int);
			void applyTransform(float* , float* , float* );
			float* getTransCoord();
			int getNumPoints();
			int getFlag();
			void setCoord(float* coord);

		protected:
			void createCoord();

		private:
			void getTransform(float* , float* , float* );
			float angle( float*  , float* );
			float len(float* );
		};

	}
}

#endif