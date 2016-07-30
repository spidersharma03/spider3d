#include "animator/OrbitalCameraAnimator.h"
#include "scene/sceneNodes/CameraNode.h"


using namespace spider3d::core;

namespace spider3d
{

	namespace scene
	{

		OrbitalCameraAnimator::OrbitalCameraAnimator(CameraNode* cameraNode):cameraNode(cameraNode)
		{
			rotX = rotY = 0.0f;
			Vector3 v(cameraNode->target.x - cameraNode->eye.x,
				      cameraNode->target.y - cameraNode->eye.y,
					  cameraNode->target.z - cameraNode->eye.z);
			v.normalize();
			initTarget = v;
		}

		void OrbitalCameraAnimator::animate(sf32 time)
		{
			Event& inputEvent = cameraNode->inputEvent;

			if ( inputEvent.eventType == SKEY_EVENT )
				return;

			Event::MouseInput& mouseInput = inputEvent.mouseInput;

			if ( mouseInput.Event == LEFT_MOUSE_PRESSED_DOWN 
				|| mouseInput.Event == MIDDLE_MOUSE_PRESSED_DOWN 
				|| mouseInput.Event == RIGHT_MOUSE_PRESSED_DOWN)
			{
				mouseX = mouseInput.X;
				mouseY = mouseInput.Y;
			}

			if( !cameraNode->m_bEnable )
				return;

			Point3& target  = cameraNode->target;
			Point3& eye     = cameraNode->eye;
			Point3& initeye = cameraNode->initialEye;
			Vector3& up     = cameraNode->up;
			Vector3& initUp = cameraNode->initUp;
			Vector3& right  = cameraNode->rightBasis;
			Matrix4f& lookAtMatrix = cameraNode->lookAtMatrix;


			if ( cameraNode->mouseEvent[1] ) // Translate
			{
				float dx = mouseInput.X - mouseX;
				float dy = mouseInput.Y - mouseY;
				// move the target and eye position in the direction of camera's right vector and up vector
				target.x -= right.x * dx * cameraNode->rotX_Factor;
				target.y -= right.y * dx * cameraNode->rotX_Factor;
				target.z -= right.z * dx * cameraNode->rotX_Factor;
				//		
				target.x += up.x * dy * cameraNode->rotY_Factor;
				target.y += up.y * dy * cameraNode->rotY_Factor;
				target.z += up.z * dy * cameraNode->rotY_Factor;

				initeye.x -= right.x * dx * cameraNode->rotX_Factor;
				initeye.y -= right.y * dx * cameraNode->rotX_Factor;
				initeye.z -= right.z * dx * cameraNode->rotX_Factor;
				//	
				initeye.x -= up.x * -dy * cameraNode->rotY_Factor;
				initeye.y -= up.y * -dy * cameraNode->rotY_Factor;
				initeye.z -= up.z * -dy * cameraNode->rotY_Factor;
			}	

			if ( cameraNode->mouseEvent[2] )
			{
				float dy = mouseInput.Y - mouseY;

				initeye.x -= initTarget.x * dy * cameraNode->zoomFactor;
				initeye.y -= initTarget.y * dy * cameraNode->zoomFactor;
				initeye.z -= initTarget.z * dy * cameraNode->zoomFactor;
			}

			/*if ( mouseInput.Event == SMOUSE_WHEEL )
			{
				initeye.z -= mouseInput.Wheel*cameraNode->zoomFactor;
			}*/

			if ( cameraNode->mouseEvent[0] )
			{
				rotY += (mouseY - mouseInput.Y )*cameraNode->rotY_Factor;
				rotX += (mouseX - mouseInput.X )*cameraNode->rotX_Factor;
			}

			float csy = cos(rotY); float sny = sin(rotY);
			float csx = cos(rotX); float snx = sin(rotX); 

			eye.y = target.y -  (target.y - initeye.y ) * csy  +  (target.z - initeye.z ) * sny;
			eye.z = target.z -  (target.y - initeye.y ) * -sny +  (target.z - initeye.z ) * csy;

			eye.x =  target.x - (target.x - initeye.x ) * csx  + (target.z - eye.z ) * snx;
			eye.z =  target.z - (target.x - initeye.x ) * -snx + (target.z - eye.z ) * csx;

			up.y = initUp.y * csy + initUp.z * -sny;
			up.z = initUp.y * sny + initUp.z * csy;
			up.x = initUp.x * csx + up.z * snx;	
			up.z = initUp.x * -snx + up.z * csx;

			
			cameraNode->lookAt( eye , target , up);

			right.x = lookAtMatrix.m00;
			right.y = lookAtMatrix.m01;
			right.z = lookAtMatrix.m02;

			mouseX = mouseInput.X;
			mouseY = mouseInput.Y;

			cameraNode->setTransformChanged(true);
		}

		CameraNode* OrbitalCameraAnimator::getCameraNode()
		{
			return cameraNode;
		}

	}
}