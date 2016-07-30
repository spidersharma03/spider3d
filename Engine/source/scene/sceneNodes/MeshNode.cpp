#include "MeshNode.h"
#include "Appearance.h"
#include "SIndexedMeshBuffer.h"
#include<stdio.h>
#include "SceneManager.h"
#include "Octree.h"

using namespace spider3d::picking;

namespace spider3d
{

	namespace scene
	{

		MeshNode::MeshNode(Nodeptr parent , SceneManagerptr sceneManager):Node(parent,sceneManager)
		{
			mesh = 0;
			m_pAppearance = 0;
			DebugDraw = false;
			m_bAppearanceOverride = false;
			pickPrimitive = 0;
		}

		MeshNode::MeshNode(SceneManagerptr sceneManager):Node(0 , sceneManager)
		{
			mesh = 0;
			m_pAppearance = 0;
			DebugDraw = false;
			m_bAppearanceOverride = false;
			pickPrimitive = 0;
			m_bOptimizePicking = false;
			m_Octree = 0;
		}

		void MeshNode::OnRegister()
		{
			if( _mVisible )
			{
				float transparency = 0.0f;
				Appearance* meshApp = 0;
				Mesh *currentMesh = 0;
				if(m_vecMeshes.size() > 0) 
				{
					currentMesh = m_vecMeshes[0];
					meshApp = currentMesh->getAppearance();
				}

				if ( meshApp )
					transparency = meshApp->getTransparencyAttributes().transparency;

				if ( currentMesh && transparency > 1e-05 )
				{
					// Find distance of this MeshNode from Camera
					// Right Now it is Calculated only for Transparent objects in the Scene
					CameraNode* camra  = sceneManager->getActiveCamera();
					Point3 eyePosition = camra->getEyePosition();
					const Point3& centre = currentMesh->getAABB().getCentre();
					Point3 p( centre.x, centre.y, centre.z );
					globalTransform.transform(p);
					float dx = ( eyePosition.x - p.x );
					float dy = ( eyePosition.y - p.y );
					float dz = ( eyePosition.z - p.z );
					m_DistFromCameraSq =  dx*dx + dy*dy + dz*dz; 
					sceneManager->registerNode(this , SceneManager::TARGET_TRANSPARENT );
				}
				else
				{
					sceneManager->registerNode(this , SceneManager::TARGET_OPAQUE );
				}
				Node::OnRegister();
			}
		}

		
		void MeshNode::setMesh(Mesh* mesh, sbool bOptimizePicking)
		{
			this->mesh = mesh;
			mesh->m_pMeshNode = this;
			m_bOptimizePicking = bOptimizePicking;
			if( m_bOptimizePicking )
				//m_Octree = new Octree(mesh);
				this->mesh->constructOctree();
		}

		void MeshNode::addMesh(Mesh* mesh, sbool bOptimizePicking)
		{
			m_vecMeshes.push_back(mesh);
			mesh->m_pMeshNode = this;
			m_bOptimizePicking = bOptimizePicking;
			if( m_bOptimizePicking )
				//m_Octree = new Octree(mesh);
				mesh->constructOctree();
		}

		sbool MeshNode::removeMesh(Mesh* mesh)
		{
			bool bFind = false;
			for( unsigned m=0; m< m_vecMeshes.size(); m++ )
			{
				if( mesh == m_vecMeshes[m] )
				{
					m_vecMeshes.erase(	m_vecMeshes.begin() + m );
					bFind = true;
				}
			}
			return bFind;
		}

		Mesh* MeshNode::getMesh(si32 index)
		{
			if ( index < m_vecMeshes.size() && index > -1 )
			{
				return m_vecMeshes[index];
			}
			else
			{
				return 0;
			}
		}

		vector<Mesh*>& MeshNode::getAllMeshes()
		{
			return m_vecMeshes;
		}

		void MeshNode::removeAllMeshes()
		{
			m_vecMeshes.clear();
		}

		si32 MeshNode::getNumMeshes()
		{
			return m_vecMeshes.size();
		}

		void MeshNode::render()
		{
			//TO DO---> View matrix and World Matrix can be stored, and send to the Renderer directly
			Matrix4f& viewMat = sceneManager->getActiveCamera()->getViewMatrix();
			Matrix4f temp;
			temp.multiply(viewMat , globalTransform);
			sceneManager->getRenderer()->setTransform(temp , WORLD);
			//if( m_pAppearance )
			{
				sceneManager->getRenderer()->setAppearance( m_pAppearance );
			}

            Color3 color;
			color.x = 0.8f;color.y = 0.1f;color.z = 0.1f;
            bool bQuads = false;
			//if ( mesh )
			for( unsigned m=0; m<m_vecMeshes.size(); m++ )
			{
				Mesh *mesh = m_vecMeshes[m];
				
				Appearance *meshApp = mesh->getAppearance();

				if( meshApp )
				{
					sceneManager->getRenderer()->setAppearance( meshApp );
				}

				for ( su32 i = 0 ; i < mesh->getNumMeshBuffers() ; i ++ )
				{
					MeshBuffer& meshBuffer = mesh->getMeshBuffer(i);
					
					if( meshBuffer.getPrimitiveType() == spider3d::QUADS )
						bQuads = true;
					if ( meshBuffer.isIndexed() )
					{
						SIndexedMeshBuffer* indexedBuffer = static_cast<SIndexedMeshBuffer*>(&meshBuffer);
						// If double pass transparency is on, then draw back faces first and then the front faces 
						if( m_pAppearance && m_pAppearance->getTransparencyAttributes().m_doublePass )
						{
							// Draw Back Faces
							m_pAppearance->getPolygonAttributes().cullFace = ECF_FRONT;
							sceneManager->getRenderer()->drawIndexedMeshBuffer(*indexedBuffer); 
							// Draw Front Faces
							m_pAppearance->setDirty(true);
							m_pAppearance->getPolygonAttributes().cullFace = ECF_BACK;
							sceneManager->getRenderer()->drawIndexedMeshBuffer(*indexedBuffer); 
						}
						else
						{
							sceneManager->getRenderer()->drawIndexedMeshBuffer(*indexedBuffer); 
						}
						if ( DebugDraw )
						 sceneManager->getRenderer()->DrawBox( indexedBuffer->getAABB() , color );
					}
					else
					{
						// If double pass transparency is on, then draw back faces first and then the front faces 
						if( m_pAppearance && m_pAppearance->getTransparencyAttributes().m_doublePass )
						{
							// Draw Back Faces
							m_pAppearance->getPolygonAttributes().cullFace = ECF_FRONT;
							sceneManager->getRenderer()->drawMeshBuffer(meshBuffer);
							// Draw Front Faces
							m_pAppearance->setDirty(true);
							m_pAppearance->getPolygonAttributes().cullFace = ECF_BACK;
							sceneManager->getRenderer()->drawMeshBuffer(meshBuffer);
						}
						else
						{
						    sceneManager->getRenderer()->drawMeshBuffer(meshBuffer); 
						}
						 if ( DebugDraw )
						 sceneManager->getRenderer()->DrawBox( meshBuffer.getAABB() , color );
					}
				}
			}
			//sceneManager->getRenderer()->DrawPoint(pickPoint,color);
			/*sf32 t = 1.0f;
			Point3 p2(pickPoint.x + pickNormal.x*t, 
				 pickPoint.y + pickNormal.y*t
				,pickPoint.z + pickNormal.z*t);
			sceneManager->getRenderer()->DrawLine(pickPoint,p2,color);
			if( pickPrimitive && bQuads)
			{
				sceneManager->getRenderer()->DrawLine(pickPrimitive[0], pickPrimitive[1], color);
				sceneManager->getRenderer()->DrawLine(pickPrimitive[1], pickPrimitive[2], color);
				sceneManager->getRenderer()->DrawLine(pickPrimitive[0], pickPrimitive[3], color);
				sceneManager->getRenderer()->DrawLine(pickPrimitive[2], pickPrimitive[3], color);
			}
			if( pickPrimitive && !bQuads)
			{
				sceneManager->getRenderer()->DrawLine(pickPrimitive[0], pickPrimitive[1], color);
				sceneManager->getRenderer()->DrawLine(pickPrimitive[1], pickPrimitive[2], color);
				sceneManager->getRenderer()->DrawLine(pickPrimitive[0], pickPrimitive[2], color);
			}*/

		}

		void MeshNode::print()
		{
			//printf("%s%s\n" , "Rendering MeshNode " , this->getName() );
		}


		AABB& MeshNode::getLocalAABB() 
		{
			return mesh ? mesh->getAABB() : bbox;
		}

		void MeshNode::setDebugDraw(sbool flag)
		{
			DebugDraw = flag;
		}

		Mesh* MeshNode::getMesh()
		{
			return mesh;
		}

		sbool MeshNode::isAppearanceOverridden()
		{
			return m_bAppearanceOverride;
		}

		void MeshNode::setAppearanceOverrideEnable(sbool bEnable)
		{
			m_bAppearanceOverride = bEnable;
		}

		void MeshNode::setAppearance(Appearance* appearance)
		{
			m_pAppearance = appearance;
		}

		Appearance* MeshNode::getAppearance()
		{
			return m_pAppearance;
		}

		PickResult* MeshNode::intersectRay(Ray& ray)
		{
			if( !m_Pickable ) return false;
			
			bool bPicked = false;

			//if( mesh )
			for( unsigned m=0; m<m_vecMeshes.size(); m++ )
			{
				bPicked = m_vecMeshes[m]->intersect(ray);
			}

			return bPicked ? &m_pickResult : 0 ;
		}
	}


}