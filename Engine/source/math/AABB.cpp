#include "AABB.h"

namespace spider3d
{
	namespace core
	{

		void AABB::set( AABB& aabb )
		{
			MaxEdge.set(aabb.MaxEdge.x , aabb.MaxEdge.y , aabb.MaxEdge.z);
			MinEdge.set(aabb.MinEdge.x , aabb.MinEdge.y , aabb.MinEdge.z);
		}
		
		Point3& AABB::getEdge(su8 index) 
		{
			return edges[index];
		}

		Point3& AABB::getMaxEdge() 
		{
			return MaxEdge;
		}

		Point3& AABB::getMinEdge() 
		{
			return MinEdge;
		}

		sbool AABB::isPointInside(Tuple3& point)
		{
			return ( point.x >= MinEdge.x && point.x <=  MaxEdge.x
				     && point.y >= MinEdge.y && point.y <=  MaxEdge.y
					 && point.z >= MinEdge.z && point.z <=  MaxEdge.z );
		}

		void AABB::addPoint(Tuple3& point)
		{
			addPoint(point.x , point.y , point.z );
		}

		void AABB::addPoint(sf32 x , sf32 y , sf32 z)
		{
			if (x>MaxEdge.x) MaxEdge.x = x;
			if (y>MaxEdge.y) MaxEdge.y = y;
			if (z>MaxEdge.z) MaxEdge.z = z;

			if (x<MinEdge.x) MinEdge.x = x;
			if (y<MinEdge.y) MinEdge.y = y;
			if (z<MinEdge.z) MinEdge.z = z;
		}

		void AABB::addAABB(AABB& aabb)
		{
			addPoint(aabb.MaxEdge);
			addPoint(aabb.MinEdge);
		}

		const Point3& AABB::getCentre()
		{
			return Centre;
		}
 
		void AABB::reCalculateAABB()
		{
			setAABB();
		}

		void AABB::transform(Matrix4f& matrix)
		{
			matrix.transform(MaxEdge);
			matrix.transform(MinEdge);
			setAABB();
		}

		void AABB::setAABB()
		{
			Vector3 diag;
			Vector3 middle;
           
			middle.add(MaxEdge , MinEdge );
			middle.scale(0.5f);
			Centre.set(middle);
			diag.sub( middle , MaxEdge );
			/*
			Edges are stored in this way:
                  /3--------/7
                 /  |      / |
                /   |     /  |
                1---------5  |
                |   2- - -| -6
                |  /      |  /
                |/        | /
                0---------4/
			*/

			edges[0].set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[1].set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[2].set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[3].set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
			edges[4].set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[5].set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[6].set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[7].set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
		}
	}

}