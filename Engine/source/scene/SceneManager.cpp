#include "SceneManager.h"
#include "MeshNode.h"
#include "Appearance.h"
#include "SpiderDisplay.h"
#include "Plane.h"
#include "STimer.h"
#include<stdio.h>
#include <algorithm>

namespace spider3d
{

	namespace scene
	{


		void SceneManager::drawScene()
		{
			getRenderer()->setTransform( Matrix4f::Identity , VIEW );
            getRenderer()->setTransform( Matrix4f::Identity , PROJECTION );

			sf32 time = timer->value();
			//STimer::value();
			Update(time);

			// register all renderable nodes
			OnRegister();

			renderCamera();

			renderLights();

			renderSkyBox();

			renderOpaque();  

			renderTransparent();
		}

		Node* SceneManager::getRootGraph()
		{
			return this;
		} 

		void SceneManager::setActiveCamera(CameraNode& activeCamera)
		{
			this->ActiveCamera = &activeCamera;
		}

		CameraNode* SceneManager::getActiveCamera()
		{
			return ActiveCamera;
		}

		SceneManager::SceneManager(DeviceContext* display):Node ( 0 , 0 )
		{
			this->display = display;
			renderer = 0;
			
			timer = new STimer();

			setName("RootGraph");

			ActiveCamera = new CameraNode(this , this);
		}

		void SceneManager::registerNode(Node* nodePtr , RENDER_TARGET target)
		{
			switch( target )
			{

			case TARGET_SKY_BOX:
				skyBoxList.push_back(nodePtr); 
				break;

			case TARGET_CAMERA:
				cameraList.push_back(nodePtr); 
				break;

			case TARGET_LIGHT:
				lightList.push_back(nodePtr);
				break;

			case TARGET_OPAQUE:
				
				//if ( !isCulled(nodePtr) )
				{
					opaqueList.push_back(nodePtr);
				}
				
				break;

			case TARGET_TRANSPARENT:
				//if ( !isCulled(nodePtr) )
				{
					transparentList.push_back(nodePtr);
				}
				break;

			default:
				break;
			}
		}

		Renderer* SceneManager::getRenderer()
		{
			return renderer;
		}

		DeviceContext* SceneManager::getDisplay()
		{
			return display;
		}

		void SceneManager::setRenderer(Renderer& renderer)
		{
			this->renderer = &renderer;
		}

		int SceneManager::numChildren()
		{
			return Children.size();
		}


		void SceneManager::renderCamera()
		{
			if ( cameraList.size() > 0 )
			{
				for (su32 i = 0 ; i < cameraList.size() ; i++ )
				{
					cameraList[i]->render();
				}
			}

			cameraList.clear();
		}

		void SceneManager::renderLights()
		{
			if ( lightList.size() > 0 ) 
			{
				for ( su32 i = 0 ; i < lightList.size() ; i++ )
				{
					lightList[i]->render();
				}
			}
			lightList.clear();
		}

        void SceneManager::renderSkyBox()
		{
			if ( skyBoxList.size() > 0 ) 
			{
				for ( su32 i = 0 ; i < skyBoxList.size() ; i++ )
				{
					skyBoxList[i]->render();
				}
			}
			skyBoxList.clear();
		}

		void SceneManager::renderOpaque()
		{
			if ( opaqueList.size() > 0 ) 
			{
				su32 n = opaqueList.size();
				for ( su32 i = 0 ; i < opaqueList.size() ; i++ )
				{
					opaqueList[i]->render();
				}
			}
			
			//glutWireSphere(2.0f,30,30);
			
			opaqueList.clear();		
		}

		bool TransparencySortPredicate( Node* n1,  Node* n2)
		{
			float d1 = n1->getDistanceFromCameraSq();
			float d2 = n2->getDistanceFromCameraSq();
			return d1 > d2;
		}

		void SceneManager::renderTransparent()
		{
			std::sort ( transparentList.begin() , transparentList.end() , TransparencySortPredicate);
			if ( transparentList.size() > 0 ) 
			{
				su32 n = transparentList.size();
				for ( su32 i = 0 ; i < transparentList.size() ; i++ )
				{
					transparentList[i]->render();
				}
			}
			transparentList.clear();
		}


		sbool SceneManager::isCulled(Node* node)
		{
			if ( node->getCullOperation() == CULL_OFF )
				return false;
			SViewFrustum frust = *ActiveCamera->getViewFrustum();

			//transform the frustum to the node's current absolute transformation
			Matrix4f invTrans , inv;
			invTrans.set(node->getGlobalTransform());
			inv = invTrans.getInverse();
			frust.transform(inv);

			AABB& box = node->getLocalAABB();

			for (su32 i=0; i<6; i++)
			{
	
				bool boxInFrustum=false;
				for (su32 j=0; j<8; j++)
				{
					if ( frust.planes[i].getPointRelation( box.getEdge(j) ) !=  spider3d::core::FRONT )
					{
						boxInFrustum=true;
						break;
					}
				}

				if (!boxInFrustum)
					return true;
			}
		
			return false;
		}



	}


}