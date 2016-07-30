#include "Ray.h"

namespace spider3d
{
#define EPSILON 0.0000000000001

	namespace core
	{


		Ray::Ray()
		{
			origin.x = 0.0f;origin.y = 0.0f;origin.z = -1.0f;
			direction.x = 0.0f;direction.y = 0.0f;direction.z = 1.0f;
		}

		Ray::Ray(const Ray& otherRay )
		{
			this->origin    = otherRay.origin;
			this->direction = otherRay.direction;
		}

		Ray::Ray(const Point3& origin , const Vector3& direction )
		{
			this->origin    = origin;
			this->direction = direction;
		}

		Ray::Ray( float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz )
		{
			origin.x = eyex ; origin.y = eyey ; origin.z = eyez ;
			direction.x = dirx; direction.y = diry; direction.z = dirz;
		}

		void Ray::set(const Point3& origin , const Vector3& direction  )
		{
			this->origin    = origin;
			this->direction = direction;
		}

		void Ray::set( const Ray& otherRay )
		{
			this->origin    = otherRay.origin;
			this->direction = otherRay.direction;
		}

		void Ray::set(float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz  )
		{
			origin.x = eyex ; origin.y = eyey ; origin.z = eyez ;
			direction.x = dirx; direction.y = diry; direction.z = dirz;
		}

		void Ray::transform( Matrix4f& mat )
		{
			mat.transform(origin);
			mat.transform(direction);
		}

		// Moller Algorithm for Ray-Triangle Intersection
		bool Ray::intersectTriangle(Point3& v0, Point3& v1, Point3& v2, Point3& out)
		{
			Vector3 edge1, edge2, tvec, pvec, qvec;
			float det, inv_det;
			edge1.sub(v2,v0);
			edge2.sub(v1,v0);	
			pvec = Vector3::cross(direction, edge2);
			det = edge1.dot(pvec);
			//if( det < EPSILON && det > -EPSILON )
			//	return false;
			if( det < EPSILON )
				return false;
			inv_det = 1.0f/det;
			tvec.sub( origin, v0);
			float u = tvec.dot(pvec) * inv_det;
			if ( u < 0.0f || u > 1.0f )
				return false;
			qvec = Vector3::cross(tvec, edge1);
			float v = direction.dot(qvec) * inv_det;
			if ( v < 0.0f || ( u + v ) > 1.0f ) 
				return false;
			float t = edge2.dot(qvec) * inv_det;
			out.x = origin.x + t*direction.x;
			out.y = origin.x + t*direction.y;
			out.z = origin.z + t*direction.z;
			return true;
		}

		bool Ray::intersectTriangle(Point3& v0, Point3& v1, Point3& v2, float* u, float* v, float* t)
		{
			Vector3 edge1, edge2, tvec, pvec, qvec;
			float det, inv_det;
			edge1.sub(v2,v0);
			edge2.sub(v1,v0);	
			pvec = Vector3::cross(direction, edge2);
			det = edge1.dot(pvec);
			//if( det < EPSILON && det > -EPSILON )
			//	return false;
			if( det < EPSILON )
				return false;
			inv_det = 1.0f/det;
			tvec.sub( origin, v0);
			*u = tvec.dot(pvec) * inv_det;
			if ( *u < 0.0f || *u > 1.0f )
				return false;
			qvec = Vector3::cross(tvec, edge1);
			*v = direction.dot(qvec) * inv_det;
			if ( *v < 0.0f || ( *u + *v ) > 1.0f ) 
				return false;
			*t = edge2.dot(qvec) * inv_det;
			return true;
		}

		bool Ray::intersectAABB(AABB& aabb)
		{
			bool Inside = true;
			Point3 MinB = aabb.MinEdge;
			Point3 MaxB = aabb.MaxEdge;
			Point3 MaxT;
			MaxT.x=MaxT.y=MaxT.z=-1.0f;
			
			// X DIRECTION
			// Find candidate planes.
			if(origin.x < MinB.x)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.x)	MaxT.x = (MinB.x - origin.x) / direction.x;
			}
			else if(origin.x > MaxB.x)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.x)	MaxT.x = (MaxB.x - origin.x) / direction.x;
			}
			
			// Y DIRECTION
			if(origin.y < MinB.y)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.y)	MaxT.y = (MinB.y - origin.y) / direction.y;
			}
			else if(origin.y > MaxB.y)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.y)	MaxT.y = (MaxB.y - origin.y) / direction.y;
			}
			
			// Z DIRECTION
			if(origin.z < MinB.z)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.z)	MaxT.z = (MinB.z - origin.z) / direction.z;
			}
			else if(origin.z > MaxB.z)
			{
				Inside		= false;
				// Calculate T distances to candidate planes
				if(direction.z)	MaxT.z = (MaxB.z - origin.z) / direction.z;
			}
			
			// Ray origin inside bounding box
			if(Inside)
			{
				return true;
			}
			
			// Get largest of the maxT's for final choice of intersection
			//si32 WhichPlane = 0;
			sf32 Largest = MaxT.x;
			if(MaxT.y > Largest)	Largest = MaxT.y;
			if(MaxT.z > Largest)	Largest = MaxT.z;
			
			// Check final candidate actually inside box
			//if(Largest & 0x80000000) return false;
			
			float outx = origin.x + Largest * direction.x;
			if( outx < MinB.x - RAYAABB_EPSILON || outx > MaxB.x + RAYAABB_EPSILON )	return false;
			
			float outy = origin.y + Largest * direction.y;
			if( outy < MinB.y - RAYAABB_EPSILON || outy > MaxB.y + RAYAABB_EPSILON )	return false;
			
			float outz = origin.z + Largest * direction.z;
			if( outz < MinB.z - RAYAABB_EPSILON || outz > MaxB.z + RAYAABB_EPSILON )	return false;
			
			//				//for(i=0;i<3;i++)
			//				{
			//					if(i!=WhichPlane)
			//					{
			//						coord.m[i] = origin.m[i] + MaxT.m[WhichPlane] * dir.m[i];
			//#ifdef RAYAABB_EPSILON
			//						if(coord.m[i] < MinB.m[i] - RAYAABB_EPSILON || coord.m[i] > MaxB.m[i] + RAYAABB_EPSILON)	return false;
			//#else
			//						if(coord.m[i] < MinB.m[i] || coord.m[i] > MaxB.m[i])	return false;
			//#endif
			//					}
			//				}
			return true;	// ray hits box
			
			//return false;
		}

	}
}