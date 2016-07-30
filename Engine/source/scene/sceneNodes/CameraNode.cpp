#include "CameraNode.h"
#include "FpsCameraAnimator.h"
#include "TpsCameraAnimator.h"
#include "FpsPhysicalCameraAnimator.h"
#include "OrbitalCameraAnimator.h"
#include "SceneManager.h"
#include "Rectangle.h"
#include "SpiderDisplay.h"


namespace spider3d
{
	namespace scene
	{

		CameraNode::CameraNode(Nodeptr parent ,SceneManagerptr sceneManager):Node(parent,sceneManager)
		{
			initialEye.x = eye.x = 0.0f;
			initialEye.y = eye.y = 1.0f;
			initialEye.z = eye.z = 4.0f;
			target.x = 0.0f;target.y = 0.0f;target.z = 0.0f;
			up.x = 0.0f ; up.y = 1.0f ; up.z = 0.0f;
			zNear = 1.00f;
			zFar = 100.0f;
			fov  = (sf32)M_PI_4;
			aspectRatio = 1.0f;
			lookAtMatrix.lookAt(eye , target , up);
			rightBasis.x = lookAtMatrix.m00;
			rightBasis.y = lookAtMatrix.m01; 
			rightBasis.z = lookAtMatrix.m02; 
			up.x = lookAtMatrix.m10 ; up.y = lookAtMatrix.m11 ; up.z = lookAtMatrix.m12;
			policy = PERSPECTIVE;
			cameraType = ECT_NORMAL;
			rotX_Factor = 0.0005f; rotY_Factor = 0.0005f;
			move_Side_Factor  = 0.05f;
			move_Front_Factor = 0.05f;
			zoomFactor  = 0.005f;
			mouseEvent[0] = mouseEvent[1] = mouseEvent[2] = false;
			m_bEnable = true;
			m_bIsActive = false;
			Renderer* renderer = sceneManager->getRenderer();
			for( unsigned i = 0; i<256; i++ ) 
				keyCodes[i] = false;
			if ( renderer )
			{
				projectionMatrix.perspective(fov , renderer->getViewPort().width/renderer->getViewPort().height, zNear , zFar);
				aspectRatio = renderer->getViewPort().width/renderer->getViewPort().height;
			}
			else
			{
				projectionMatrix.perspective(fov , aspectRatio , zNear , zFar);
			}
		}

		CameraNode::CameraNode(Nodeptr parent ,SceneManagerptr sceneManager, CamereType cameraType):Node(parent,sceneManager)
		{
			initialEye.x = eye.x = 0.0f;
			initialEye.y = eye.y = 1.0f;
			initialEye.z = eye.z = 4.0f;
			target.x = 0.0f;target.y = 0.0f;target.z = 0.0f;
			up.x = 0.0f ; up.y = 1.0f ; up.z = 0.0f;
			zNear = 1.0f;
			zFar = 100.0f;
			fov  = (sf32)M_PI_4;
			aspectRatio = 1.0f;
			lookAtMatrix.lookAt(eye , target , up);
			rightBasis.x = lookAtMatrix.m00;
			rightBasis.y = lookAtMatrix.m01; 
			rightBasis.z = lookAtMatrix.m02; 
			initUp.x = up.x = lookAtMatrix.m10 ;
			initUp.y = up.y = lookAtMatrix.m11 ;
			initUp.z = up.z = lookAtMatrix.m12;
			policy = PERSPECTIVE;
			this->cameraType = cameraType;
			rotX_Factor = 0.001f; rotY_Factor = 0.001f;
			move_Side_Factor  = 0.05f;
			move_Front_Factor = 0.05f;
			zoomFactor  = 0.005f;
			mouseEvent[0] = mouseEvent[1] = mouseEvent[2] = false;
			Renderer* renderer = sceneManager->getRenderer();
			m_bEnable = true;
			m_bIsActive = false;
			for( unsigned i = 0; i<256; i++ ) 
				keyCodes[i] = false;

			if ( renderer )
			{
				projectionMatrix.perspective(fov , renderer->getViewPort().width/renderer->getViewPort().height, zNear , zFar);
			}
			else
			{
				projectionMatrix.perspective(fov , aspectRatio , zNear , zFar);
			}
			aspectRatio = renderer->getViewPort().width/renderer->getViewPort().height;
			if ( cameraType == ECT_ORBITAL )
			{
				OrbitalCameraAnimator* animator = new OrbitalCameraAnimator(this);
				addAnimator(animator);
				m_Animator = animator;
			}
			if ( cameraType == ECT_FPS )
			{
				FpsCameraAnimator* animator = new FpsCameraAnimator(this,sceneManager->getDisplay()->getCursor());
				addAnimator(animator);
				m_Animator = animator;
			}
			if ( cameraType == ECT_TPS )
			{
				TpsCameraAnimator* animator = new TpsCameraAnimator(this,sceneManager->getDisplay()->getCursor());
				addAnimator(animator);
				m_Animator = animator;
			}
			if ( cameraType == ECT_FPS_PHYSICAL )
			{
				FpsPhysicalCameraAnimator* animator = new FpsPhysicalCameraAnimator(this,sceneManager->getDisplay()->getCursor());
				addAnimator(animator);
				m_Animator = animator;
			}
		}

		sbool CameraNode::OnEvent(Event& sEvent)
		{
			inputEvent = sEvent;

			if ( inputEvent.eventType == SKEY_EVENT )
			{
				for( unsigned i = 0; i<256; i++ ) 
					keyCodes[inputEvent.keyInput.Key] = inputEvent.keyInput.PressedDown;
			}

			float x = (float)inputEvent.mouseInput.X;
			float y = (float)inputEvent.mouseInput.Y;

			speedX = fabs(x - prevx);
			speedY = fabs(y - prevy);

			prevx = x;
			prevy = y;

			//printf("%f \t %f\n", speedX, speedY);
			su32 viewPort[4] = {(su32)m_ViewPort.x,(su32)m_ViewPort.y,(su32)m_ViewPort.width,(su32)m_ViewPort.height};

			switch(inputEvent.mouseInput.Event)
			{
			case LEFT_MOUSE_PRESSED_DOWN:
				mouseEvent[0] = true;
				break;
			case SMOUSE_MOVED:
				//glReadPixels(x,600-y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
				//printf("\nDepth=%f",depth);
				//Matrix4f::UnProject(x,m_ViewPort.height-y,0.9999f,lookAtMatrix,projectionMatrix,viewPort,temp);
				//Matrix4f::UnProject(x,m_ViewPort.height-y,0.0f,lookAtMatrix,projectionMatrix,viewPort,temp1);
				//ray = getRayFromScreenCoordinates((si32)x,(si32)(m_ViewPort.height-y));
				break;
			case LEFT_MOUSE_LEFT_UP:
				mouseEvent[0] = false;
				break;
			case RIGHT_MOUSE_PRESSED_DOWN:
				mouseEvent[1] = true;
				break;
			case RIGHT_MOUSE_LEFT_UP:
				mouseEvent[1] = false;
				break;
			case MIDDLE_MOUSE_PRESSED_DOWN:
				mouseEvent[2] = true;
				break;
			case MIDDLE_MOUSE_LEFT_UP:
				mouseEvent[2] = false;
				break;
			default:
				break;
			}
			return true;
		}

		void CameraNode::Update(sf32 time)
		{
			for ( su32 j = 0 ; j < Animators.size() ; j ++ )
			{
				NodeAnimator& animator = *( Animators[j] ) ;

				animator.animate(time);
			}

			if ( _mTransformChanged )
			{
				// Set Global Transform to Inverse of this view matrix
				globalTransform = lookAtMatrix.getInverse();
			}

			for ( su32 i = 0 ; i < Children.size() ; i ++ )
			{
				Node& node = *( Children[i] ) ;
				node.Update(time);
			}
		}

		void CameraNode::OnRegister()
		{
			if ( isTransformChanged() )
			{
				Point3 ePos(eye);
				Point3 targetPos(target);
				Vector3  upVec(up);
				getParent()->getGlobalTransform().transform(ePos);
				getParent()->getGlobalTransform().transform(targetPos);
				getParent()->getGlobalTransform().transform(upVec);
				lookAt(ePos , targetPos , upVec );

				Matrix4f temp;
				temp.multiply(projectionMatrix , lookAtMatrix );
				ViewFrustum.eyePosition.set(ePos);
				ViewFrustum.set( temp );
			}

			if ( sceneManager->getActiveCamera() == this )
				this->sceneManager->registerNode(this , SceneManager::TARGET_CAMERA );

			Node::OnRegister();

		}


		AABB& CameraNode::getLocalAABB() 
		{
			return bbox;
		}


		void CameraNode::lookAt( Point3& eyePos , Point3& targetPos, Vector3& upVec)
		{
			lookAtMatrix.lookAt( eyePos , targetPos , upVec );
		}

		void CameraNode::perspective( float fov , float aspectRatio, float zNear , float zFar )
		{
			this->fov = fov;
			this->aspectRatio = aspectRatio;
			this->zNear = zNear;
			this->zFar  = zFar;
			projectionMatrix.perspective(fov , aspectRatio, zNear , zFar);
		}

		void CameraNode::frustrum( float left, float right, float bottom, float top, float znear, float zfar )
		{
			projectionMatrix.frustum( left, right, bottom, top, znear, zfar );
		}

		void CameraNode::ortho( float left, float right, float bottom, float top, float znear, float zfar )
		{
			projectionMatrix.ortho( left, right, bottom, top, znear, zfar );
		}

		void CameraNode::setProjectionPolicy( projectionPolicy policy )
		{
			this->policy = policy;
			if( policy == ORTHOGRAPHIC )
			{
				projectionMatrix.ortho( -1.0f, 1.0f, -1.0f, 1.0f, 0.001f, 1000.0f );
			}
			if( policy == PERSPECTIVE )
			{
				projectionMatrix.perspective(fov , aspectRatio, zNear , zFar);
			}
		}

		void CameraNode::setNominalEye()
		{
			eye.set(0, 1 , 2);
			target.set(0 , 0 , 0 );
			up.set(0 , 1 , 0);
			lookAtMatrix.lookAt( eye , target , up );
		}

		void CameraNode::setEyePosition(Point3& eyePos)
		{
			initialEye = eye = eyePos;
		}

		void CameraNode::setEyePosition(sf32 eyePosX, sf32 eyePosY, sf32 eyePosZ)
		{
			initialEye.x = eye.x = eyePosX;
			initialEye.y = eye.y = eyePosY;
			initialEye.z = eye.z = eyePosZ;
		}

		void CameraNode::setTargetPosition(Point3& targetPos)
		{
			initialTarget = target = targetPos;
		}

		void CameraNode::setTargetPosition(sf32 targetX, sf32 targetY, sf32 targetZ)
		{
			initialTarget.x = target.x = targetX;
			initialTarget.y = target.y = targetY;
			initialTarget.z = target.z = targetZ;
		}

		void CameraNode::setUpVector(Vector3& up )
		{
			this->up = this->initUp = up;
		}

		void CameraNode::setUpVector(sf32 upX, sf32 upY, sf32 upZ)
		{
			initUp.x = up.x = upX;
			initUp.y = up.y = upY;
			initUp.z = up.z = upZ;
		}

		void CameraNode::setAspectRatio(sf32 aspectRatio)
		{
			this->aspectRatio = aspectRatio;
			projectionMatrix.perspective((sf32)M_PI_4 , aspectRatio , zNear , zFar);
		}

		void CameraNode::setNearPlane(sf32 zNear)
		{
			this->zNear = zNear;
			perspective(fov , aspectRatio , zNear , zFar );
		}

		void CameraNode::setFarPlane(sf32 zFar)
		{
			this->zFar = zFar;
			perspective(fov , aspectRatio , zNear , zFar );
		}

		void CameraNode::setFOV(sf32 fov)
		{
			this->fov = fov;
			perspective(fov , aspectRatio , zNear , zFar );
		}

		sf32 CameraNode::getNearPlane()
		{
			return zNear;
		}

		sf32 CameraNode::getFarPlane()
		{
			return zFar;
		}

		sf32 CameraNode::getFOV()
		{
			return fov;
		}

		sf32 CameraNode::getAspectRatio()
		{
			return aspectRatio;
		}

		Matrix4f& CameraNode::getViewMatrix()
		{
			return lookAtMatrix;
		}

		Matrix4f& CameraNode::getProjectionMatrix()
		{
			return projectionMatrix;
		}

		projectionPolicy CameraNode::getProjectionPolicy()
		{
			return policy;
		}

		SViewFrustum* CameraNode::getViewFrustum()
		{
			return &ViewFrustum;
		}

		void CameraNode::setViewPort(Rectangle& rect)
		{
			m_ViewPort.set(rect);
		}

		void CameraNode::setMoveFrontFactor(sf32 moveFrontFactor)
		{
			this->move_Front_Factor = moveFrontFactor;
		}
			
		void CameraNode::setMoveSideFactor(sf32 moveSideFactor)
		{
			this->move_Side_Factor = moveSideFactor;
		}

		Rectangle CameraNode::getViewPort()
		{
			return m_ViewPort;
		}

		Ray CameraNode::getRayFromScreenCoordinates(si32 screenX, si32 screenY)
		{
			Ray ray;
			//1. Compute the normalized, symmetric position [ vx vy ] on the view from the Screen co-ordinates [ screenX screenY ] and viewPort extent [ Nx Ny ] like so:
			float vx = ( screenX + 0.5f ) / m_ViewPort.width - 0.5f;
			float vy = ( screenY + 0.5f ) / m_ViewPort.height - 0.5f;
			//This formula let the ray pass through the center of the pixel.

			//2. Unnormalize that position using the (normalized) up vector cy and right vector cx from the camera, and the extent [ Sx Sy ] of the view in the world.
			Vector3 R(lookAtMatrix.m00, lookAtMatrix.m01, lookAtMatrix.m02);
			Vector3 U(lookAtMatrix.m10, lookAtMatrix.m11, lookAtMatrix.m12);
			Vector3 V(lookAtMatrix.m20, lookAtMatrix.m21, lookAtMatrix.m22);
			float Sy = 2.0f * zNear * tan(fov*0.5f);
			float Sx = aspectRatio * Sy;
			R.scale(Sx * vx);
			U.scale(Sy * vy);
			Vector3 p1(R);
			p1.add(U);

			//3. Determine the ray from the camera position pc and forward vector cz
			//a. for an orthogonal projection as
			//R(t) := pc + p1 + t * cz 
			//b. for a perspective projection with view distance D as
			V.scale(-zNear);
			Vector3 d(p1);
			d.add(V);
			d.normalize();
			ray.set(getEyePosition(),d);
			//R(t) := pc + t * d / |d|

			//You may need to use the correspondence of the view distance D, (horizontal) view size Sx, and (horizontal) opening angle a
			//tan( a ) = 2D / Sx
			//to determine D.
			return ray;
		}

		void CameraNode::render()
		{
			sceneManager->getRenderer()->setTransform(lookAtMatrix , VIEW);
			sceneManager->getRenderer()->setTransform(projectionMatrix , PROJECTION);
			//Matrix4f inv;
			//inv = lookAtMatrix.getInverse();
			//inv.m03 = inv.m13 = inv.m23 = 0.0f;
			//sceneManager->getRenderer()->setTransform(inv , TEXTURE);
		}

	}

}