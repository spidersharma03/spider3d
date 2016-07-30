#ifndef _SCENEMANAGER_H

#define _SCENEMANAGER_H

#include "Node.h"
#include "CameraNode.h"
#include "LightNode.h"
#include "Renderer.h"


using namespace spider3d::render;

namespace spider3d
{
	class DeviceContext;
    
	class STimer;

	namespace scene
	{

		class SceneManager:public Node 
		{

		public:

			enum RENDER_TARGET { TARGET_CAMERA = 0 , TARGET_SKY_BOX , TARGET_LIGHT , TARGET_OPAQUE , TARGET_TRANSPARENT };

			void drawScene();

			Node* getRootGraph();

			CameraNode* getActiveCamera();

			SceneManager(DeviceContext*);

			Renderer* getRenderer();

			DeviceContext* getDisplay();

			void setActiveCamera(CameraNode&);

			void setRenderer(Renderer&);

			int numChildren();

			int numViews();

			int numLights();

			void registerNode(Node* , RENDER_TARGET);

			inline STimer* getTimer()
			{
				return timer;
			}

		private:

			void renderLights();

			void renderSkyBox();

			void renderOpaque();

			void renderTransparent();

			void renderCamera();

			sbool isCulled(Node*);

			void pushLight(LightNode*);

			void pushCamera(CameraNode*);

			//PointLight* pointLight;

			CameraNode* ActiveCamera;

			int numCamera;

			int numLight;

			//SpatialList lightList;

			//SpatialList cameraList;

			//SpatialList opaqueList;

			//SpatialList transparentList;

			vector<Node*> lightList;

			vector<Node*> cameraList;

			vector<Node*> skyBoxList;

			vector<Node*> opaqueList;

			vector<Node*> transparentList;

			Renderer* renderer;
			
			STimer* timer;

			DeviceContext* display;
		};


	}

}
#endif


