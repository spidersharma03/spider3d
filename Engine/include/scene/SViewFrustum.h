#ifndef _SVIEWFRUSTUM_H

#define _SVIEWFRUSTUM_H

#include "coreMath.h"
#include <math.h>
#include "Plane.h"

using namespace spider3d::core;


namespace spider3d
{

	enum FRUSTUM_PLANE { FP_NEAR = 0 , FP_FAR , FP_LEFT , FP_RIGHT , FP_BOTTOM , FP_TOP };

	namespace scene
	{	

		class SViewFrustum
		{
		public:

			inline void set(Matrix4f&);

			inline void transform(Matrix4f&);

			inline sbool isPointInside(Point3& point);

			inline float sphereFractionInside(Point3& centre, float radius);

			SViewFrustum()
			{
			};

			SViewFrustum(SViewFrustum& other)
			{
              for ( su32 i = 0 ; i < 6 ; i++ )
			  {
				  planes[i].Normal.x = other.planes[i].Normal.x;
				  planes[i].Normal.y = other.planes[i].Normal.y;
				  planes[i].Normal.z = other.planes[i].Normal.z;
				  planes[i].D        = other.planes[i].D;
			  }
			  eyePosition.x = other.eyePosition.x;
			  eyePosition.y = other.eyePosition.y;
			  eyePosition.z = other.eyePosition.z;
                
			};


			SViewFrustum& operator= (SViewFrustum& other)
			{
              for ( su32 i = 0 ; i < 6 ; i++ )
			  {
				  planes[i] = other.planes[i];
			  }
			  eyePosition.x = other.eyePosition.x;
			  eyePosition.y = other.eyePosition.y;
			  eyePosition.z = other.eyePosition.z;

			  return *this;
                
			};

			SViewFrustum(Matrix4f& matrix)
			{
               set(matrix);
			};


    		Plane planes[6];

			Point3 eyePosition;

		};


		inline void SViewFrustum::set(Matrix4f &matrix)
		    {
            // left clipping plane
			planes[FP_LEFT].Normal.x = (matrix.m30 + matrix.m00);
			planes[FP_LEFT].Normal.y = (matrix.m31 + matrix.m01);
			planes[FP_LEFT].Normal.z = (matrix.m32 + matrix.m02);
			planes[FP_LEFT].D =        (matrix.m33 + matrix.m03);

			// FP_RIGHT clipping plane
			planes[FP_RIGHT].Normal.x = (matrix.m30 - matrix.m00);
			planes[FP_RIGHT].Normal.y = (matrix.m31 - matrix.m01);
			planes[FP_RIGHT].Normal.z = (matrix.m32 - matrix.m02);
			planes[FP_RIGHT].D =        (matrix.m33 - matrix.m03);

			// FP_TOP clipping plane
			planes[FP_TOP].Normal.x = (matrix.m30 - matrix.m10 );
			planes[FP_TOP].Normal.y = (matrix.m31 - matrix.m11);
			planes[FP_TOP].Normal.z = (matrix.m32 - matrix.m12);
			planes[FP_TOP].D =        (matrix.m33 - matrix.m13);

			// FP_BOTTOM clipping plane
			planes[FP_BOTTOM].Normal.x = (matrix.m30 + matrix.m10);
			planes[FP_BOTTOM].Normal.y = (matrix.m31 + matrix.m11);
			planes[FP_BOTTOM].Normal.z = (matrix.m32 + matrix.m12);
			planes[FP_BOTTOM].D =        (matrix.m33 + matrix.m13);

			// far clipping plane
			planes[FP_FAR].Normal.x = (matrix.m30 - matrix.m20);
			planes[FP_FAR].Normal.y = (matrix.m31 - matrix.m21);
			planes[FP_FAR].Normal.z = (matrix.m32 - matrix.m22);
			planes[FP_FAR].D =        (matrix.m33 - matrix.m23);

			// FP_NEAR clipping plane
			planes[FP_NEAR].Normal.x = (matrix.m20 + matrix.m30);
			planes[FP_NEAR].Normal.y = (matrix.m21 + matrix.m31);
			planes[FP_NEAR].Normal.z = (matrix.m22 + matrix.m32);
			planes[FP_NEAR].D =        (matrix.m23 + matrix.m33);

			// normalize normals
			su32 i;
			for ( i=0; i < 6; i++)
			{
				sf32 len = -planes[i].Normal.length();
				len = 1/len ;
				planes[i].Normal *= len;
				planes[i].D *= len;
			}
		}


		inline sbool SViewFrustum::isPointInside(Point3& point)
		{
			for (su32 i=0; i<6; i++)
			{
				if ( planes[i].getPointRelation( point ) ==  spider3d::core::FRONT )
				{
					return false;
				}
			}
			return true;
		}
		
		inline float SViewFrustum::sphereFractionInside(Point3& centre, float radius)
		{
			float minFraction = 1000.0f;
			float fraction;
			for (su32 i=0; i<6; i++)
			{
				float dist = planes[i].shortestDistance(centre);
				// Outside the ViewFrustrum
				if ( dist >  radius )
				{
					return 0.0f;
				}
				fraction = fabs(dist)/radius;
				minFraction = fraction < minFraction ? fraction : minFraction;
				minFraction = dist < 0.0f ? minFraction : -minFraction;
			}
			return minFraction + 1;
		}

		inline void SViewFrustum::transform(Matrix4f& matrix)
		{
			for ( su32 i = 0 ; i < 6 ; i++ )
			{
				planes[i].transform(matrix);
			}
			matrix.transform(eyePosition);
		}

		
}


}
#endif 