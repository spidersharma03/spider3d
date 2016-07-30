#ifndef _AABB_H

#define _AABB_H

#include "coreMath.h"

namespace spider3d
{
	namespace core
	{
		class AABB
		{
		public:

			AABB():MaxEdge(-10000.0f,-10000.0f,-10000.0f),MinEdge(10000.0f,10000.0f,10000.0f)
			{
              setAABB();
			};

			AABB(Point3& maxEdge , Point3& minEdge):MaxEdge(maxEdge),MinEdge(minEdge)
			{
			  setAABB();
			}

			AABB(sf32 maxX , sf32 maxY , sf32 maxZ , sf32 minX , sf32 minY , sf32 minZ ):MaxEdge(maxX,maxY,maxZ),MinEdge(minX,minY,minZ)
			{
		      setAABB();
			}

			AABB( AABB& aabb):MaxEdge(aabb.MaxEdge),MinEdge(aabb.MinEdge)
			{
				Centre = aabb.getCentre();
				setAABB();
			};

			void set ( AABB& );

            Point3& getEdge(su8) ;

			Point3& getMaxEdge() ;

			Point3& getMinEdge() ;

			const Point3& getCentre();

			sbool isPointInside(Tuple3&);

			void addPoint(Tuple3&);

			void addPoint(sf32 , sf32 , sf32);

			void addAABB(AABB& );

			void reCalculateAABB();

			void transform(Matrix4f& matrix);

			Point3 MaxEdge;
			Point3 MinEdge;

		private:

			Point3 Centre;
			Point3 edges[8];

			void setAABB();
		};

	}

}
#endif