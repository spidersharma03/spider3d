#include "animator/FpsCameraAnimator.h"
#include "scene/sceneNodes/CameraNode.h"


using namespace spider3d::core;

namespace spider3d
{

	namespace scene
	{

		FpsCameraAnimator::FpsCameraAnimator(CameraNode* cameraNode, Cursor* cursor):cameraNode(cameraNode),cursor(cursor)
		{
			rotX = rotY = 0.0f;
			cursorPositionX = cursorPositionY = 0.5f;
			cursorCentrePositionX = cursorCentrePositionY = 0.0f;
			//cameraNode->move_Front_Factor = 2.0f;
			//cameraNode->move_Side_Factor = 2.0f;
		}

		void FpsCameraAnimator::animate(sf32 time)
		{
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

			float move_Front_Factor = cameraNode->move_Front_Factor;
			float move_Side_Factor  = cameraNode->move_Side_Factor;

			if( cameraNode->keyCodes[KEY_KEY_W] )
			{
			// move the target and eye position in the direction of TARGET vector
				cameraNode->initialTarget.x += forward.x*move_Front_Factor;
				cameraNode->initialTarget.y += forward.y*move_Front_Factor;
				cameraNode->initialTarget.z += forward.z*move_Front_Factor;
		
				cameraNode->eye.x += forward.x*move_Front_Factor;
				cameraNode->eye.y += forward.y*move_Front_Factor;
				cameraNode->eye.z += forward.z*move_Front_Factor;
			}
			
			if( cameraNode->keyCodes[KEY_KEY_S] )
			{
			// move the target and eye position in the direction of TARGET vector
				cameraNode->initialTarget.x -= forward.x*move_Front_Factor;
				cameraNode->initialTarget.y -= forward.y*move_Front_Factor;
				cameraNode->initialTarget.z -= forward.z*move_Front_Factor;
		
				cameraNode->eye.x -= forward.x*move_Front_Factor;
				cameraNode->eye.y -= forward.y*move_Front_Factor;
				cameraNode->eye.z -= forward.z*move_Front_Factor;
			}

			if ( cameraNode->keyCodes[KEY_KEY_A] )
			{
			// move the target and eye position in the direction of TARGET vector
				cameraNode->initialTarget.x += right.x*move_Side_Factor;
				cameraNode->initialTarget.y += right.y*move_Side_Factor;
				cameraNode->initialTarget.z += right.z*move_Side_Factor;
		
				cameraNode->eye.x += right.x*move_Side_Factor;
				cameraNode->eye.y += right.y*move_Side_Factor;
				cameraNode->eye.z += right.z*move_Side_Factor;
			}

			if ( cameraNode->keyCodes[KEY_KEY_D] )
			{
			// move the target and eye position in the direction of TARGET vector
				cameraNode->initialTarget.x -= right.x*move_Side_Factor;
				cameraNode->initialTarget.y -= right.y*move_Side_Factor;
				cameraNode->initialTarget.z -= right.z*move_Side_Factor;
		
				cameraNode->eye.x -= right.x*move_Side_Factor;
				cameraNode->eye.y -= right.y*move_Side_Factor;
				cameraNode->eye.z -= right.z*move_Side_Factor;
			}

			if ( mouseInput.Event == SMOUSE_MOVED )
			{
				//rotY += (mouseY - mouseInput.Y )*cameraNode->rotY_Factor;
				//rotX += (mouseX - mouseInput.X )*cameraNode->rotX_Factor;
				if ( cursor )
				{
					cursor->getRelativePosition(&cursorPositionX, &cursorPositionY);

					//if (cursorPositionX != cursorCentrePositionX && cursorPositionY != cursorCentrePositionY) 
					{
						rotY += (0.5f - cursorPositionY )*cameraNode->rotY_Factor*1000;
						rotX += (0.5f - cursorPositionX )*cameraNode->rotX_Factor*1000;

						cursor->setRelativePosition( 0.5f, 0.5f);
						cursor->getRelativePosition(&cursorCentrePositionX, &cursorCentrePositionY);

						cursorPositionX = cursorCentrePositionX ;
						cursorPositionY = cursorCentrePositionY;
					}
				}
			}

			float csy = cos(rotY); float sny = sin(rotY);
			float csx = cos(rotX); float snx = sin(rotX); 

			target.y = eye.y -  (eye.y - init_target.y ) * csy  +  (eye.z - init_target.z ) * sny;
			target.z = eye.z -  (eye.y - init_target.y ) * -sny +  (eye.z - init_target.z ) * csy;
 	
			target.x =  eye.x - (eye.x - init_target.x ) * csx  +  (eye.z - target.z ) * snx;
			target.z =  eye.z - (eye.x - init_target.x ) * -snx +  (eye.z - target.z ) * csx;
	
			up.y = initUp.y * csy + initUp.z * -sny;
			up.z = initUp.y * sny + initUp.z * csy;
			up.x = initUp.x * csx + up.z * snx;	
			up.z = initUp.x * -snx + up.z * csx;

			
			cameraNode->lookAt( eye , target , up);

			mouseX = mouseInput.X;
			mouseY = mouseInput.Y;

			cameraNode->setTransformChanged(true);
		}

		CameraNode* FpsCameraAnimator::getCameraNode()
		{
			return cameraNode;
		}

	}
}