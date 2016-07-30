#include "animator/TpsCameraAnimator.h"
#include "scene/sceneNodes/CameraNode.h"
#include "scene/SceneManager.h"
#include "util/STimer.h"

namespace spider3d
{
	namespace scene
	{

		TpsCameraAnimator::TpsCameraAnimator(CameraNode* cameraNode, Cursor* cursor):cameraNode(cameraNode),cursor(cursor)
		{
			m_TargetNode = 0;
			m_OrbitRadius = 5.0f;
			m_OffsetVector.set(0.0f,0.7f,1.0f);
			m_OffsetVector.normalize();
			m_OffsetVector.scale(m_OrbitRadius);

			m_SpringDamping = 10.0f;
			m_SpringStiffness = 40.0f;
		}

		void TpsCameraAnimator::animate(sf32 time)
		{
			if( !m_TargetNode )
				return;

			m_CurrentTime = cameraNode->getSceneManager()->getTimer()->getCurrentTime();

			static sbool bFirst = true;
			sf32 deltaT = (m_CurrentTime - m_PrevTime)/1000.0f;
			if(bFirst)
			{
				bFirst = false;
				m_PrevTime = m_CurrentTime;
				return;
			}

			Point3& target  = cameraNode->target;
			Point3& init_target  = cameraNode->initialTarget;
			Point3& eye     = cameraNode->eye;
			Point3& initeye = cameraNode->initialEye;
			Vector3& up     = cameraNode->up;
			Vector3& initUp = cameraNode->initUp;
			Vector3& right  = cameraNode->rightBasis;
			Vector3 forward;

			Matrix4f targetTransform = m_TargetNode->getGlobalTransform();
			Point3 targetPosition(targetTransform.m03, targetTransform.m13, targetTransform.m23);

			init_target = target = targetPosition;

			//
			/*sf32 snlat = sin(m_Latitude);
			sf32 snlon = sin(m_Longitude);
			sf32 cslon = cos(m_Longitude);
			sf32 x = m_OrbitRadius * snlat * snlon;
			sf32 z = m_OrbitRadius * snlat * cslon;
			sf32 y = m_OrbitRadius * cos(m_Latitude);*/

			targetPosition.x += m_OffsetVector.x;
			targetPosition.y += m_OffsetVector.y;
			targetPosition.z += m_OffsetVector.z;

			Vector3 d(m_CameraPosition.x - target.x, m_CameraPosition.y - target.y, m_CameraPosition.z - target.z);
			//Vector3 d(m_CameraPosition.x - targetPosition.x, m_CameraPosition.y - targetPosition.y, m_CameraPosition.z - targetPosition.z);
			sf32 delta = ( d.length() - m_OrbitRadius );
			//printf("Delta=%f\n", deltaT);
			d.normalize();

			Vector3 cameraAccleration;
			sf32 k = -m_SpringStiffness * delta; 
			d.scale(k);
			cameraAccleration.add(d);
			Vector3 velocity = m_CameraVelocity;
			velocity.scale(-m_SpringDamping);
			cameraAccleration.add(velocity);

			m_CameraVelocity.x += cameraAccleration.x * deltaT;
			m_CameraVelocity.y += cameraAccleration.y * deltaT;
			m_CameraVelocity.z += cameraAccleration.z * deltaT;

			m_CameraPosition.x += m_CameraVelocity.x * deltaT;
			m_CameraPosition.y += m_CameraVelocity.y * deltaT;
			m_CameraPosition.z += m_CameraVelocity.z * deltaT;
			
			//printf("m_CameraPosition=%f \t %f \t %f\n", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);

			initeye = eye = targetPosition;
			up  = Vector3(0.0f,1.0f,0.0f);

			cameraNode->lookAt( eye , target , up);
			cameraNode->setTransformChanged(true);

			m_PrevTime = m_CurrentTime;
		}

		void TpsCameraAnimator::setTarget(Node* target)
		{
			m_TargetNode = target;
			Matrix4f targetTransform = m_TargetNode->getGlobalTransform();
			Point3 targetPosition(targetTransform.m03, targetTransform.m13, targetTransform.m23);

			cameraNode->target = targetPosition;

			targetPosition.x += m_OffsetVector.x;
			targetPosition.y += m_OffsetVector.y;
			targetPosition.z += m_OffsetVector.z;

			m_CameraPosition = cameraNode->eye = targetPosition;

			Vector3 d(m_CameraPosition.x - cameraNode->target.x, m_CameraPosition.y - cameraNode->target.y, m_CameraPosition.z - cameraNode->target.z);
			sf32 delta = fabs ( d.length() );

		}

		Node* TpsCameraAnimator::getTarget()
		{
			return m_TargetNode;
		}

		CameraNode* TpsCameraAnimator::getCameraNode()
		{
			return cameraNode;
		}

	}
}