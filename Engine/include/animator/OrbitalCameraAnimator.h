#ifndef _ORBITAL_CAMERA_ANIMATOR_H

#define _ORBITAL_CAMERA_ANIMATOR_H


#include "NodeAnimator.h"
#include "coreMath.h"

namespace spider3d
{

	namespace scene
	{

		class CameraNode;

		class OrbitalCameraAnimator: public NodeAnimator
		{
		public:
			void animate(sf32 time);

			OrbitalCameraAnimator(CameraNode* cameraNode);

			CameraNode* getCameraNode();

		private:
			CameraNode* cameraNode;
			Vector3 initTarget;
			sf32 rotX, rotY;
			si32 mouseX , mouseY;
			sbool leftMousePressed , rightMousePressed;
		};

	}
}
#endif