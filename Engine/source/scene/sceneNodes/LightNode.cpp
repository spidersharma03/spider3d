#include "LightNode.h"
#include "SceneManager.h"

#include<stdio.h>


namespace spider3d
{

	namespace scene
	{


		void LightNode::print()
		{
		}

		void LightNode::render()
		{
			//printf("%s%s\n" , "Rendering " , getName() );
			//TO DO---> View matrix and World Matrix can be stored, and send to the Renderer directly
			Matrix4f& viewMat = sceneManager->getActiveCamera()->getViewMatrix();
			Matrix4f temp;
			temp.multiply(globalTransform);
			sceneManager->getRenderer()->setTransform(temp , WORLD);
            sceneManager->getRenderer()->addLight(lightData);
		}


		void LightNode::setLightData(LightData& lightData)
		{
			this->lightData.set(lightData);
		}

		void LightNode::setLightType(LIGHT_TYPE type)
		{
			this->lightData.type = type;
		}

		LightData& LightNode::getLightData()
		{
			return lightData;
		}

		void LightNode::OnRegister()
		{
			//printf("%s%s\n" , "Registering " , getName() );

			sceneManager->registerNode(this , SceneManager::TARGET_LIGHT );

			Node::OnRegister();

		}

		
		AABB& LightNode::getLocalAABB() 
		{
			return bbox;
		}

        
		LightNode::LightNode(Nodeptr parent , SceneManagerptr sceneManager):Node(parent , sceneManager)
		{
			setName("LightNode");
			bbox.MaxEdge.set(0.1f,0.1f,0.1f);
			bbox.MinEdge.set(-0.1f,-0.1f,-0.1f);
			bbox.reCalculateAABB();
		}

		LightNode::LightNode(SceneManagerptr sceneManager):Node(0 , sceneManager)
		{
			setName("LightNode");
			bbox.MaxEdge.set(0.1f,0.1f,0.1f);
			bbox.MinEdge.set(-0.1f,-0.1f,-0.1f);
			bbox.reCalculateAABB();
		}




		LightNode::~LightNode()
		{
			printf("Light Deleted");
		}


	}

}