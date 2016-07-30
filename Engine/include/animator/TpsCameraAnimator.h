#ifndef _TPS_CAMERA_ANIMATOR_H

#define _TPS_CAMERA_ANIMATOR_H


#include "NodeAnimator.h"
#include "coreMath.h"
#include "Cursor.h"

namespace spider3d
{
	namespace scene
	{
		class CameraNode;

		class TpsCameraAnimator: public NodeAnimator
		{
		public:
			void animate(sf32 time);

			TpsCameraAnimator(CameraNode* cameraNode, Cursor* cursor);

			void setTarget(Node* target);

			Node* getTarget();

			CameraNode* getCameraNode();

			void setOrbitRadius(sf32 radius)
			{
				m_OrbitRadius = radius;
			}

			void setOffsetVector(Vector3 offsetVector)
			{
				m_OffsetVector = offsetVector;
			}

		protected:
			Node* m_TargetNode;
			CameraNode* cameraNode;
			Cursor* cursor;
			sf32 rotX, rotY;
			si32 mouseX , mouseY;
			sf32 cursorPositionX, cursorPositionY;
			sf32 cursorCentrePositionX, cursorCentrePositionY;
			sbool leftMousePressed , rightMousePressed;

			sf32 m_CurrentTime;
			sf32 m_PrevTime;

			sf32 m_OrbitRadius;
			sf32 m_Latitude, m_Longitude;
			Vector3 m_OffsetVector;

			Vector3 m_CameraVelocity;
			Vector3 m_CameraPosition;

			sf32 m_SpringDamping;
			sf32 m_SpringStiffness;
		};

	}
}
#endif