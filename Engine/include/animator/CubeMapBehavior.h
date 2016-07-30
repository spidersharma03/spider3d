#include "NodeAnimator.h"
#include "CameraNode.h"
#include "Appearance.h"

using namespace spider3d::effects;

namespace spider3d
{
	namespace scene
	{ 
		class CubeMapBehavior:public NodeAnimator
		{
		public:
			CubeMapBehavior(CameraNode* cameraNode, Appearance* targetApp):targetApp(targetApp),cameraNode(cameraNode)
			{
			}

			void animate(sf32 time)
			{
				if( targetApp)
				{
					Matrix4f& textureTransform = targetApp->getTextureUnitState(1)->getTextureAttributes()->texture_transform;
					Matrix4f cameraTransform = cameraNode->getViewMatrix();
					cameraTransform.m03 = cameraTransform.m13 = cameraTransform.m23 = 0.0f;
					Matrix4f invTransform;
					invTransform = cameraTransform.getInverse();
					textureTransform = invTransform;
				}
			}

		private:
			Appearance* targetApp;
			CameraNode* cameraNode;
		};

	}
}