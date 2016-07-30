#ifndef _CAMERANODE_H

#define _CAMERANODE_H

#include "Node.h"
#include "Ray.h"
#include "SViewFrustum.h"
#include "EventListener.h"
#include "Rectangle.h"

namespace spider3d
{

	namespace scene
	{

		class NodeAnimator;

		enum projectionPolicy { PERSPECTIVE = 0 , ORTHOGRAPHIC };   
		
		enum CamereType { ECT_NORMAL , ECT_ORBITAL , ECT_FPS, ECT_TPS, ECT_FPS_PHYSICAL };

		class CameraNode:public Node , public EventListener
		{

		public:

			Point3 temp,temp1;
			
			Ray ray;

			CameraNode(Nodeptr, SceneManagerptr);

			CameraNode(Nodeptr, SceneManagerptr , CamereType cameraType);

			virtual sbool OnEvent(Event& sEvent);

			virtual void OnRegister();
			
			virtual void render();

			virtual void Update(sf32 time);

			AABB& getLocalAABB() ;

			void lookAt( Point3& , Point3& , Vector3&);

			void perspective( float , float , float , float );

			void frustrum( float , float , float , float , float , float );

			void ortho( float left, float right, float bottom, float top, float znear, float zfar );

			void setProjectionPolicy( projectionPolicy );

			void setNominalEye();

			void setAspectRatio(sf32 aspectRatio);

			void setNearPlane(sf32);

			void setFarPlane(sf32);

			void setFOV(sf32);

			void setEyePosition(Point3& eyePos);

			void setEyePosition(sf32 eyePosX, sf32 eyePosY, sf32 eyePosZ);

			void setTargetPosition(Point3& targetPos);

			void setTargetPosition(sf32 targetX, sf32 targetY, sf32 targetZ);

			void setUpVector(Vector3& up );

			void setUpVector(sf32 upX, sf32 upY, sf32 upZ);

			void setViewPort(Rectangle&);

			void setMoveFrontFactor(sf32);
			
			void setMoveSideFactor(sf32);

			Rectangle getViewPort();

			Ray getRayFromScreenCoordinates(si32 screenX, si32 screenY);

			sf32 getNearPlane();

			sf32 getFarPlane();

			sf32 getFOV();

			sf32 getAspectRatio();

			Matrix4f& getViewMatrix();

			Matrix4f& getProjectionMatrix();

			SViewFrustum* getViewFrustum();

			projectionPolicy getProjectionPolicy();
			
			void setEnable(bool bEnable)
			{
				m_bEnable = bEnable;
			}

			inline Point3 getEyePosition()
			{
				return eye;
			}

			inline void setActiveFlag(bool bActive)
			{
				m_bIsActive = bActive;
			}

			NodeAnimator* getAnimator()
			{
				return m_Animator;
			}

			Point3 start , end;

			friend class OrbitalCameraAnimator;
			friend class FpsCameraAnimator;
			friend class FpsPhysicalCameraAnimator;
			friend class TpsCameraAnimator;

		private:

			bool m_bEnable;
			bool m_bIsActive;
			Point3 eye ;
			Point3 target;
			Vector3 velocity;
			Vector3 up , initUp;
			Rectangle m_ViewPort;
			float fov , aspectRatio, zNear , zFar , left , right , bottom , top ;
            
			CamereType  cameraType;
			projectionPolicy policy;
			Matrix4f lookAtMatrix;
			Matrix4f projectionMatrix;
			SViewFrustum ViewFrustum;
			
			Event inputEvent;
			Point3 initialEye ;
			Point3 initialTarget;
			Vector3 rightBasis;
			// Sensitivity Factors for Camera movement in case of Orbital Or FPS Camera
			float rotX_Factor , rotY_Factor , zoomFactor;
			float move_Front_Factor, move_Side_Factor;

			sbool mouseEvent[3];
			sbool keyCodes[256];

			NodeAnimator *m_Animator;

			public:
			float prevx, prevy;
			float speedX, speedY;
		};


	}

}
#endif 