#ifndef _LENS_FLARE_NODE_H
#define _LENS_FLARE_NODE_H

#include "Node.h"

namespace spider3d
{
	namespace appearance
	{
		class Appearance;
	}
}

using namespace spider3d::appearance;

namespace spider3d
{
	namespace scene
	{
		class LensFlareNode: public Node
		{
		public:

			LensFlareNode( Nodeptr , SceneManagerptr, Point3 lightPosition);

			virtual void render();  	

			virtual void OnRegister();

			virtual void print();

			void setLightPosition( Point3& lightPosition )
			{
				m_LightPosition = lightPosition;
			}

		private:

			void createGeometry();

			void computeBillBoard(Point3& p,Matrix4f& billBoardTransform);

			void renderFlare(Matrix4f& billBoardTransform, Matrix4f& viewMat, Point3 position, vector<sf32>& vecCoords, vector<sf32>& vecTexCoords, vector<su8>& vecColors, su8);

			Point3 m_LightPosition, m_PointOfIntersection;
			Vector3 m_LightToPointOfIntersection;
			sf32 m_distLightSq;
			sf32 m_LightToPointOfIntersectionLength;
			sf32 m_Fraction;

			vector<sf32> m_vecCoords1;
			vector<su8> m_vecColors1;
			vector<sf32> m_vecTexCoords1;

			vector<sf32> m_vecCoords2;
			vector<su8> m_vecColors2;
			
			vector<sf32> m_vecCoords3;
			vector<su8> m_vecColors3;
			
			vector<sf32> m_vecCoords4,m_vecCoords5,m_vecCoords6,m_vecCoords7,m_vecCoords8,m_vecCoords9;
			vector<su8> m_vecColors4,m_vecColors5,m_vecColors6,m_vecColors7,m_vecColors8,m_vecColors9;

			vector<sf32> m_vecCoords10, m_vecCoords11, m_vecCoords12, m_vecCoords13, m_vecCoords14, m_vecCoords15, m_vecCoords16, m_vecCoords17;
			vector<su8> m_vecColors10,m_vecColors11,m_vecColors12,m_vecColors13,m_vecColors14,m_vecColors15,m_vecColors16,m_vecColors17;

			Appearance *m_App1;
			Appearance *m_App2;
			Appearance *m_App3;
			Appearance *m_App4;
			Appearance *m_App5;
		};
	}
}
#endif