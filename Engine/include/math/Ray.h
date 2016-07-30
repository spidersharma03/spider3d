#ifndef _RAY_H
#define _RAY_H

#include "coreMath.h"
#include "AABB.h"

#define RAYAABB_EPSILON 0.00001f

namespace spider3d
{
	namespace core
	{

		class Ray
		{
		public: 
			Point3 origin;
			Vector3 direction;
		public:
			Ray();
			Ray(const Ray& otherRay );
			Ray(const Point3& origin , const Vector3& direction );
			Ray( float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz ); 
			void set(const Ray& otherRay );
			void set(const Point3& origin , const Vector3& direction  );
			void set(float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz  );
			void transform( Matrix4f& mat );
			bool intersectTriangle(Point3& v0, Point3& v1, Point3& v2, Point3& out);
			bool intersectTriangle(Point3& v0, Point3& v1, Point3& v2, float* u, float* v, float* t);
			bool intersectAABB(AABB& aabb);
		};


		typedef struct IntersectionInformation
		{
			//vector<Point3*> hitPoints;
			Point3 hitPoints[8];
			Vector3 Normal;
			float T;
			bool isHit;
			unsigned numHits;

			IntersectionInformation()
			{
				T = 1e20f;
				isHit = false;
				numHits = 0;
			}

		}Intersect_Info;

	}
}

#endif