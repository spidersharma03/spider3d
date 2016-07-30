#include "animator/FpsPhysicalCameraAnimator.h"
#include "scene/sceneNodes/CameraNode.h"
#include "scene/SceneManager.h"
#include "util/STimer.h"
#include "picking/PickResult.h"

using namespace spider3d::picking;

namespace spider3d
{
	namespace scene
	{

		FpsPhysicalCameraAnimator::FpsPhysicalCameraAnimator(CameraNode* cameraNode, Cursor* cursor)
			:FpsCameraAnimator(cameraNode, cursor)
		{
			m_Gravity.set(0.0f,-10.0f,0.0f);
			m_EyeHeight = 2.0f;
			cameraNode->move_Front_Factor = 4.0f;
			cameraNode->move_Side_Factor  = 4.0f;
			m_bFalling = true;
			m_JumpSpeed = 4.0f;
			m_LimitAngle = M_PI/2.3f;
			m_HeightModulation = 0.006f;
			m_HeightModulationSpeed = 0.015f;
			m_ErrorCorrectionFactor = 10.0f;

			initialForward.set(cameraNode->initialTarget.x - cameraNode->initialEye.x,
				0.0f , cameraNode->initialTarget.z - cameraNode->initialEye.z);
			initialForward.normalize();
		}

		void FpsPhysicalCameraAnimator::animate(sf32 time)
		{
			m_CurrentTime = cameraNode->sceneManager->getTimer()->getCurrentTime();
			sf32 snd = 0.0f;

			static sbool bFirst = true;
			sf32 deltaT = (m_CurrentTime - m_PrevTime)/1000.0f;
			if(bFirst)
			{
				bFirst = false;
				m_PrevTime = m_CurrentTime;
				return;
			}
			
			//float timeFactor = 1.0f/deltaT/300.0f;
			//printf("DltaT=%f\n", m_CurrentTime);
			if (!m_bFalling) 
			{
				snd = m_HeightModulation*sin(m_CurrentTime*m_HeightModulationSpeed);
			}

			Event& inputEvent = cameraNode->inputEvent;

			Event::MouseInput& mouseInput = inputEvent.mouseInput;
			Event::KeyInput& keyInput = inputEvent.keyInput;

			static sbool flag = true;

			if ( flag && mouseInput.Event == SMOUSE_MOVED )
			{
				if ( cursor )
				{
					cursor->setRelativePosition( 0.5f, 0.5f);
				}
				mouseX = mouseInput.X;
				mouseY = mouseInput.Y;
				flag = false;
			}

			if( !cameraNode->m_bEnable )
				return;

			Point3& target  = cameraNode->target;
			Point3& init_target  = cameraNode->initialTarget;
			Point3& eye     = cameraNode->eye;
			Point3& initeye = cameraNode->initialEye;
			Vector3& up     = cameraNode->up;
			Vector3& initUp = cameraNode->initUp;
			Vector3& right  = cameraNode->rightBasis;
			Vector3 forward;
			Matrix4f& lookAtMatrix = cameraNode->lookAtMatrix;
			forward.set(-lookAtMatrix.m20,-lookAtMatrix.m21,-lookAtMatrix.m22);
			right.set( -lookAtMatrix.m00, -lookAtMatrix.m01, -lookAtMatrix.m02);

			Vector3& cameraVelocity = cameraNode->velocity;
			Vector3& velocity = cameraNode->velocity;

			float move_Front_Factor = cameraNode->move_Front_Factor;
			float move_Side_Factor  = cameraNode->move_Side_Factor;
			if( !m_bFalling && (cameraNode->keyCodes[KEY_KEY_W]
			|| cameraNode->keyCodes[KEY_KEY_A]
			|| cameraNode->keyCodes[KEY_KEY_S]
			|| cameraNode->keyCodes[KEY_KEY_D] ) ) 
			{
				//cameraNode->initialTarget.y += snd;
				//cameraNode->eye.y += snd;
			}
			else
			{
				snd = 0.0f;
			}


			if( !cameraNode->keyCodes[KEY_KEY_W]
			&& !cameraNode->keyCodes[KEY_KEY_A]
			&& !cameraNode->keyCodes[KEY_KEY_S]
			&& !cameraNode->keyCodes[KEY_KEY_D] ) 
			{
				cameraVelocity.x = cameraVelocity.z = 0.0f;
			}

			Vector3 v;

			if( cameraNode->keyCodes[KEY_KEY_W] )
			{
				v.x = forward.x;
				v.z = forward.z;
			}

			if( cameraNode->keyCodes[KEY_KEY_S] )
			{
				v.x += -forward.x;
				v.z += -forward.z;
			}

			if ( cameraNode->keyCodes[KEY_KEY_A] )
			{
				v.x += right.x;
				v.z += right.z;
			}

			if ( cameraNode->keyCodes[KEY_KEY_D] )
			{
				v.x += -right.x;
				v.z += -right.z;
			}

			v.normalize();
			v.scale(cameraNode->move_Side_Factor);
			cameraVelocity.x = v.x;
			cameraVelocity.z = v.z;

			if ( cameraNode->keyCodes[KEY_SPACE] && !m_bFalling )
			{
				cameraVelocity.y = m_JumpSpeed;
				m_bFalling = true;
			}

			if ( mouseInput.Event == SMOUSE_MOVED )
			{
				if ( cursor )
				{
					cursor->getRelativePosition(&cursorPositionX, &cursorPositionY);

					rotY += (0.5f - cursorPositionY )*cameraNode->rotY_Factor*1000;
					rotX += (0.5f - cursorPositionX )*cameraNode->rotX_Factor*1000;

					cursor->setRelativePosition( 0.5f, 0.5f);
					cursor->getRelativePosition(&cursorCentrePositionX, &cursorCentrePositionY);

					cursorPositionX = cursorCentrePositionX ;
					cursorPositionY = cursorCentrePositionY;
				}
			}
			
			if( rotY > m_LimitAngle )
				rotY = m_LimitAngle;

			if( rotY < -m_LimitAngle )
				rotY = -m_LimitAngle;

			float csy = cos(rotY); float sny = sin(rotY);
			float csx = cos(rotX); float snx = sin(rotX); 

			cameraVelocity.x += m_Gravity.x * deltaT;
			cameraVelocity.y += m_Gravity.y * deltaT;
			cameraVelocity.z += m_Gravity.z * deltaT;

			eye.x += cameraVelocity.x * deltaT;
			eye.y += cameraVelocity.y * deltaT + snd;
			eye.z += cameraVelocity.z * deltaT;

			init_target.x += cameraVelocity.x * deltaT;
			init_target.y += cameraVelocity.y * deltaT + snd;
			init_target.z += cameraVelocity.z * deltaT;

			// Ray Cast to avoid falling below
			Ray ray;
			ray.origin.set(cameraNode->eye.x,cameraNode->eye.y,cameraNode->eye.z);
			ray.direction.set(0.0f,-1.0f,0.0f);

			PickResult* pickClosestResult = cameraNode->sceneManager->pickClosest(ray);

			sf32 distSq = 1000.0f;
			Point3 intersectionPoint;
			sf32 distSqMoveDirection = 1000.0f;
			Point3 intersectionPointMoveDirection;
			Vector3 hitNormal(0.0f,1.0f,0.0f);

			if( pickClosestResult )
			{
				distSq = pickClosestResult->getPickDistanceSquared();
				intersectionPoint = pickClosestResult->getIntersection().getIntersectionPointWorld();
				//hitNormal = pickClosestResult->getIntersection().getIntersectionNormalGlobal();
			}
			//sf32 cs = ray.direction.dot(hitNormal);
			//distSq *= cs*cs;
			if( distSq < m_EyeHeight * m_EyeHeight )
			{
				sf32 delta = m_EyeHeight - sqrt(distSq);
				cameraVelocity.y = delta * m_ErrorCorrectionFactor;
				m_bFalling = false;
			}
			
			// Ray Cast to Avoid penetrations into objects
			/*Ray moveDirRay;
			moveDirRay.origin.set(cameraNode->eye.x,cameraNode->eye.y,cameraNode->eye.z);
			Vector3 rayDirection(cameraVelocity);
			rayDirection.y = 0.0f;
			moveDirRay.direction.set(rayDirection);
			PickResult* pickClosestResultMoveDirection = cameraNode->sceneManager->pickClosest(moveDirRay);

			// Push the eye back in the direction of hit normal
			sf32 D = 0.2f;
			if( pickClosestResultMoveDirection )
			{
				distSqMoveDirection = pickClosestResultMoveDirection->getPickDistanceSquared();
				intersectionPointMoveDirection = pickClosestResultMoveDirection->getIntersection().getIntersectionPointWorld();
				hitNormal = pickClosestResultMoveDirection->getIntersection().getIntersectionNormalGlobal();
				sf32 cs = rayDirection.dot(hitNormal);
				distSqMoveDirection *= cs*cs;
				if( distSqMoveDirection < D * D )
				{
					sf32 delta = D - sqrt(distSqMoveDirection);
					
					eye.x += delta*hitNormal.x;
					eye.z += delta*hitNormal.z;

					init_target.x = eye.x + initialForward.x * 4.0f;
					init_target.z = eye.z + initialForward.z * 4.0f;
				}
			}*/

			
			target.y = eye.y -  (eye.y - init_target.y ) * csy  +  (eye.z - init_target.z ) * sny;
			target.z = eye.z -  (eye.y - init_target.y ) * -sny +  (eye.z - init_target.z ) * csy;

			target.x =  eye.x - (eye.x - init_target.x ) * csx  +  (eye.z - target.z ) * snx;
			target.z =  eye.z - (eye.x - init_target.x ) * -snx +  (eye.z - target.z ) * csx;

			up.y = initUp.y * csy + initUp.z * -sny;
			up.z = initUp.y * sny + initUp.z * csy;
			up.x = initUp.x * csx + up.z * snx;	
			up.z = initUp.x * -snx + up.z * csx;
			
			//cameraNode->lookAt( eye , target , up);

			mouseX = mouseInput.X;
			mouseY = mouseInput.Y;

			cameraNode->setTransformChanged(true);

			m_PrevTime = m_CurrentTime;
		}

		void FpsPhysicalCameraAnimator::setGravity(Vector3 gravity)
		{
			m_Gravity = gravity;
		}

	}
}