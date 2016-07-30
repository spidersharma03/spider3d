#ifndef _FPS_CAMERA_ANIMATOR_H

#define _FPS_CAMERA_ANIMATOR_H


#include "NodeAnimator.h"
#include "coreMath.h"
#include "Cursor.h"

namespace spider3d
{

	namespace scene
	{

		class CameraNode;

		class FpsCameraAnimator: public NodeAnimator
		{
		public:
			void animate(sf32 time);

			FpsCameraAnimator(CameraNode* cameraNode, Cursor* cursor);

			CameraNode* getCameraNode();

		protected:
			
			CameraNode* cameraNode;
			Cursor* cursor;
			sf32 rotX, rotY;
			si32 mouseX , mouseY;
			sf32 cursorPositionX, cursorPositionY;
			sf32 cursorCentrePositionX, cursorCentrePositionY;
			sbool leftMousePressed , rightMousePressed;
		};

	}
}
#endif