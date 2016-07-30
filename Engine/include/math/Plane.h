#ifndef _PLANE_H

#define _PLANE_H

#include "coreMath.h"

namespace spider3d
{

	namespace core
	{

		enum POINT_RELATION { FRONT = 0 , BACK , PLANAR };
		class Plane
		{
		public:

			Plane()
			{
				this->Normal.set(0.0f,1.0f,0.0f);
				this->D = 1.0f;
			};

			Plane(Plane& otherPlane)
			{

			};

			void set(Tuple3& , sf32 );

			void set(Tuple3& , Tuple3& );

			void set(Vector3& , Vector3& , Vector3& );

			sf32 calculateDistanceFromOrigin(Tuple3&);

			sf32 shortestDistance(Tuple3& );

			Point3 getPointOnPlane() ;

			POINT_RELATION getPointRelation( Tuple3& );

			void transform(Matrix4f&);
				// normal to the plane
				Vector3 Normal;
			// distance from the origin ( shortest distance )
			sf32 D;

		};

	}
}
#endif