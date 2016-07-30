#ifndef _FPS_PHYSICAL_CAMERA_ANIMATOR_H
#define _FPS_PHYSICAL_CAMERA_ANIMATOR_H

#include "FpsCameraAnimator.h"

namespace spider3d
{
	namespace scene
	{

		class CameraNode;

		class FpsPhysicalCameraAnimator: public FpsCameraAnimator
		{
		public:
			void animate(sf32 time);

			FpsPhysicalCameraAnimator(CameraNode* cameraNode, Cursor* cursor);
			
			void setGravity(Vector3 gravity);

			inline void setLimitAngle( sf32 limitAngle)
			{
				m_LimitAngle = limitAngle; 
			}

		private:
			Vector3 initialForward;
			Vector3 m_Gravity;
			sf32 m_CurrentTime;
			sf32 m_PrevTime;
			sf32 m_EyeHeight;
			sf32 m_JumpSpeed;
			sf32 m_MaxModulatedHeight;
			sbool m_bFalling;
			sf32 m_LimitAngle;
			sf32 m_HeightModulation;
			sf32 m_HeightModulationSpeed;
			sf32 m_ErrorCorrectionFactor;
		};

	}
}

#endif
