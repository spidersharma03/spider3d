#include "math/Plane.h"

namespace spider3d
{

	namespace core
	{

		void Plane::set(Tuple3& Normal , sf32 D)
		{
			this->Normal.set(Normal.x , Normal.y , Normal.z);
			this->D = D;
		}

		void Plane::set(Tuple3& Normal , Tuple3& point)
		{
			this->Normal.set(Normal.x , Normal.y , Normal.z);
			this->D = calculateDistanceFromOrigin(point);
		}

		void Plane::set(Vector3& p1 , Vector3& p2 , Vector3& p3)
		{
			Normal = (p3 - p1).cross( p2 - p1);
			Normal.normalize();

			this->D = calculateDistanceFromOrigin(p1);
		}

		sf32 Plane::shortestDistance(Tuple3& point)
		{
			return Normal.dot(point) + D;
		}

		sf32 Plane::calculateDistanceFromOrigin(Tuple3& point)
		{
			D = -Normal.dot(point) ;
			return  D;
		}

		Point3 Plane::getPointOnPlane() 
		{
			Point3 point;
			point.set(Normal);
			return (point * -D);
		}

		POINT_RELATION Plane::getPointRelation( Tuple3& point)
		{

			sf32 d = Normal.dot(point) + D;

			if (d < -1e-16)
				return BACK;

			if (d > 1e-16)
				return FRONT;

			return PLANAR;
		}


		void Plane::transform(Matrix4f& matrix)
		{
			Point3 p;
			p.set(Normal);
			p.x *= -D;p.y *= -D;p.z *= -D;
			matrix.transform( p );

			Matrix4f inverseTranspose;
			inverseTranspose.set ( matrix.getInverseTranspose() );

			inverseTranspose.transform(Normal);

			Normal.normalize();
			calculateDistanceFromOrigin( p );
		}

	}
}