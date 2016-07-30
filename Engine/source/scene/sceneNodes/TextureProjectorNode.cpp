#include "TextureProjectorNode.h"
#include "SceneManager.h"
#include "TextureProjector.h"
#include "Appearance.h"

using namespace spider3d::appearance;

Point3 pos, target;
Vector3 up;
Point3 n1, n2, n3, n4;
Point3 f1, f2, f3, f4;

namespace spider3d
{
	namespace scene
	{
		TextureProjectorNode::ProjectorUpdater::ProjectorUpdater(TextureProjector* textureProjector)
			:textureProjector(textureProjector)
		{
		}

		void TextureProjectorNode::ProjectorUpdater::setProjectionPolicy(int projectionPolicy)
		{
			this->projectionPolicy = projectionPolicy;
		}

		void TextureProjectorNode::ProjectorUpdater::animate(sf32 time)
		{
			Matrix4f projectorMatrix;
			if( projectionPolicy == TextureProjectorNode::PROJECTION_POLICY_CAMERA )
			{
				textureProjector->getCameraProjectorMatrix(projectorMatrix);
			}
			else if ( projectionPolicy == TextureProjectorNode::PROJECTION_POLICY_PROJECTOR )
			{
				textureProjector->getProjectorMatrix(projectorMatrix);
			}
			else
			{
				textureProjector->getProjectorMatrix3D(projectorMatrix);
			}
			projectorTextureAttributes->texture_transform = projectorMatrix;
		}

		TextureProjectorNode::TextureProjectorNode( Texture* texture, SceneManagerptr sceneManagerPtr, Nodeptr parent )
			:Node(parent , sceneManagerPtr)
		{
			this->texture = texture;
			projectionPolicy = PROJECTION_POLICY_PROJECTOR;
			projectorPosition.x = 15.0f; projectorPosition.y = 16.0f; projectorPosition.z = -12.0f;
			projectorTarget.x = 0.0f; projectorTarget.y = 0.0f; projectorTarget.z = 0.0f;
			projectorUp.x = 0.0f; projectorUp.y =1.0f; projectorUp.z = 0.0f;
			textureProjector = new TextureProjector(sceneManager->getActiveCamera() , projectorPosition, projectorTarget, projectorUp);
			
			projectorUpdator = new ProjectorUpdater(textureProjector);
			
			addAnimator(projectorUpdator);

			projectorTexUnitState = new TextureUnitState(texture);
			projectorTexUnitState->getTextureAttributes()->texture_mode = ETEXM_MODULATE;
			projectorTexUnitState->getTexCoordGeneration()->bEnable = true;
			projectorTexUnitState->getTexCoordGeneration()->texGenMode = ETGM_EYE_LINEAR;
			projectorTexUnitState->getTexCoordGeneration()->texComponentFormat = ETCF_4;
			projectorUpdator->projectorTextureAttributes = projectorTexUnitState->getTextureAttributes();
		    
			Vector3 dir, right, upVector;
			dir.sub( projectorTarget, projectorPosition);
			dir.normalize();
			CameraNode* camera = sceneManager->getActiveCamera();
			float fov = camera->getFOV();
			dir.scale( camera->getNearPlane() );
			Matrix4f& view = textureProjector->getViewMatrix();
			right.x = view.m00; right.y = view.m01;right.z = view.m02; 
			upVector.x = view.m10; upVector.x = view.m11; upVector.x = view.m12;
			float height = 2.0f*camera->getNearPlane()*tan(fov/2.0f);
			float width = height* camera->getAspectRatio();
			width = height = 1.0f;

			n1.x = projectorPosition.x + dir.x + 0.5f*width*right.x + 0.5f*height*upVector.x; 
			n1.y = projectorPosition.y + dir.y + 0.5f*width*right.y + 0.5f*height*upVector.y; 
			n1.z = projectorPosition.z + dir.z + 0.5f*width*right.z + 0.5f*height*upVector.z; 

			n2.x = projectorPosition.x + dir.x + 0.5f*width*right.x - 0.5f*height*upVector.x; 
			n2.y = projectorPosition.y + dir.y + 0.5f*width*right.y - 0.5f*height*upVector.y; 
			n2.z = projectorPosition.z + dir.z + 0.5f*width*right.z - 0.5f*height*upVector.z; 

			n3.x = projectorPosition.x + dir.x - 0.5f*width*right.x + 0.5f*height*upVector.x; 
			n3.y = projectorPosition.y + dir.y - 0.5f*width*right.y + 0.5f*height*upVector.y; 
			n3.z = projectorPosition.z + dir.z - 0.5f*width*right.z + 0.5f*height*upVector.z; 

			n4.x = projectorPosition.x + dir.x - 0.5f*width*right.x - 0.5f*height*upVector.x; 
			n4.y = projectorPosition.y + dir.y - 0.5f*width*right.y - 0.5f*height*upVector.y; 
			n4.z = projectorPosition.z + dir.z - 0.5f*width*right.z - 0.5f*height*upVector.z; 

		}
		
		void TextureProjectorNode::addScope( MeshNode* meshNode )
		{
			// Save TextureUnit of this meshNode, so that it can be recovered while removing this meshnode from the Scope.
			if( meshNode )
			{
				Appearance* appearance = 0;
				appearance = meshNode->getAppearance();
				TextureUnitState *texUnit = 0;
				if ( appearance )
				{
					// Save the old texture unit
					texUnit = appearance->getTextureUnitState(0);
					// Set the projector Texture Unit
					appearance->setTextureUnitState(0,projectorTexUnitState);
				}
				//if(texUnit)
				m_oldTexUnitMap[meshNode] = texUnit;
			}
		}

		void TextureProjectorNode::removeScope( MeshNode* meshNode )
		{
			// While removing the Meshnode from the scope list, we need to restore the original TextureUnit. 
			if ( m_oldTexUnitMap.find(meshNode) != m_oldTexUnitMap.end() )
			{
				Appearance* appearance = 0;
				appearance = meshNode->getAppearance();
				if ( appearance )
				{
				  appearance->setTextureUnitState(0,m_oldTexUnitMap.find(meshNode)->second);
				}
				m_oldTexUnitMap.erase(meshNode);
			}
		}

		void TextureProjectorNode::removeAllScopes()
		{
			while (!m_oldTexUnitMap.empty())
			{
				MeshNode* meshNode = m_oldTexUnitMap.begin()->first;
				Appearance* appearance = meshNode->getAppearance();
				TextureUnitState* texUnit = m_oldTexUnitMap.begin()->second;
				appearance->setTextureUnitState(0,texUnit);
				m_oldTexUnitMap.erase(m_oldTexUnitMap.begin());
			}
		}

		void TextureProjectorNode::setProjectionPolicy(int policy)
		{
			this->projectionPolicy = policy;
			projectorUpdator->setProjectionPolicy(policy);
		}

		void TextureProjectorNode::render()
		{
			//TO DO---> View matrix and World Matrix can be stored, and send to the Renderer directly
			Matrix4f& viewMat = sceneManager->getActiveCamera()->getViewMatrix();
			Matrix4f temp;
			temp.multiply(viewMat , globalTransform);
			sceneManager->getRenderer()->setTransform(temp , WORLD);
			//sceneManager->getRenderer()->DrawPoint( projectorPosition, Color3(255,0,0) );

			//sceneManager->getRenderer()->DrawPoint( n1, Color3(0,255,0) );
			//sceneManager->getRenderer()->DrawPoint( n2, Color3(0,255,0) );
			//sceneManager->getRenderer()->DrawPoint( n3, Color3(0,255,0) );
			//sceneManager->getRenderer()->DrawPoint( n4, Color3(0,255,0) );
		}

		void TextureProjectorNode::OnRegister()
		{
			pos = projectorPosition;
			target = projectorTarget;
			up = projectorUp;
			globalTransform.transform(pos);
			globalTransform.transform(target);
			globalTransform.transform(up);
			textureProjector->lookAt(pos, target, up);
			sceneManager->registerNode(this , SceneManager::TARGET_OPAQUE );
			Node::OnRegister();
		}

		void TextureProjectorNode::print()
		{
		}

		TextureUnitState* TextureProjectorNode::getProjectorTextureUnitState()
		{
			return projectorTexUnitState;
		}
	}
}