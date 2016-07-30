#include "animator/BillBoard.h"
#include "scene/sceneNodes/MeshNode.h"
#include "scene/sceneNodes/CameraNode.h"

namespace spider3d
{

	namespace scene
	{
		BillBoard::BillBoard(CameraNode* cameraNode, MeshNode* target, BILLBOARD_TYPE type):m_CameraNode(cameraNode),m_Target(target),m_Type(type)
		{
		}

		void BillBoard::animate(sf32 time)
		{
			//Matrix4f& transform = m_Target->getGlobalTransform();
			Matrix4f billBoardTransform;
			// Vector from Centre of the BillBoard to Eye
			Point3 eyePosition = m_CameraNode->getEyePosition();
			const Point3& centre = m_Target->getMesh()->getAABB().getCentre();
			Point3 p( centre.x, centre.y, centre.z );
			//transform.transform(p);
			Vector3 eyeToCentre(p.x-eyePosition.x, p.y-eyePosition.y, p.z-eyePosition.z);
			if(m_Type == EBT_CYLINDRICAL)
			eyeToCentre.y = 0.0f;
			eyeToCentre.normalize();

			Vector3 up(0.0f , 1.0f ,0.0f);
            if (  m_Type == EBT_SPHERICAL )
			{
				Matrix4f& camView = m_CameraNode->getViewMatrix();
				up.x = camView.m10;
				up.y = camView.m11;
				up.z = camView.m12;
			}
			Vector3 right;
			right = Vector3::cross( up , eyeToCentre);

			if( m_Type == EBT_SPHERICAL )
				up = eyeToCentre.cross(right);
			

			billBoardTransform.m00 = right.x; billBoardTransform.m01 = up.x; billBoardTransform.m02 = eyeToCentre.x;
			billBoardTransform.m10 = right.y; billBoardTransform.m11 = up.y; billBoardTransform.m12 = eyeToCentre.y;
			billBoardTransform.m20 = right.z; billBoardTransform.m21 = up.z; billBoardTransform.m22 = eyeToCentre.z;

			//transform.set(billBoardTransform);
			m_Target->setTransform(billBoardTransform);
		}
	}
}