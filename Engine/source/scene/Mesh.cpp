#include "scene/Mesh.h"
#include "math/Ray.h"
#include "scene/MeshBuffer.h"
#include "scene/IndexedMeshBuffer.h"
#include "scene/sceneNodes/MeshNode.h"
#include "picking/PickResult.h"
#include "accelerationStructs/Octree.h"

using namespace spider3d::core;


namespace spider3d
{
	namespace scene
	{
		void Mesh::addMeshBuffer(MeshBuffer* meshBuffer)
		{
			meshBuffers.push_back(meshBuffer);
		}

		MeshBuffer& Mesh::getMeshBuffer( su32 index)
		{
			return *meshBuffers[index];
		}

		su32 Mesh::getNumMeshBuffers()
		{
			return meshBuffers.size();
		}

		MeshNode* Mesh::getMeshNode()
		{
			return m_pMeshNode;
		}

		sbool Mesh::detach()
		{
			if( m_pMeshNode )
			{
				return m_pMeshNode->removeMesh(this);
			}

			return false;
		}

		AABB& Mesh::getAABB()
		{
			return BoundingBox;
		}

		void Mesh::setAppearance(Appearance* app)
		{
			m_pAppearance = app;
		}

		Appearance* Mesh::getAppearance()
		{
			return m_pAppearance;
		}

		void Mesh::setAABB(AABB& other)
		{
			BoundingBox.set(other);
		}

		void Mesh::reCalculateAABB()
		{
			if (meshBuffers.size())
			{
				BoundingBox.set ( meshBuffers[0]->getAABB() );
				for (su32 i=1; i<meshBuffers.size(); ++i)
					BoundingBox.addAABB( meshBuffers[i]->getAABB() );
			}
			else
			{
				BoundingBox.MaxEdge.set(0.0f,0.0f,0.0f);
				BoundingBox.MinEdge.set(0.0f,0.0f,0.0f);
			}
			BoundingBox.reCalculateAABB();
		}

		bool Mesh::intersect(Ray& ray)
		{
			bool bPicked = false;
			bool bIsAnyPicked = false;
			for( unsigned m = 0; m<meshBuffers.size(); m++ )
			{
				MeshBuffer *meshBuffer = meshBuffers[m];
				if ( meshBuffer->isIndexed() )
				{
					IndexedMeshBuffer *indexBuffer = (IndexedMeshBuffer*)meshBuffer;

					if(indexBuffer->getPrimitiveType() == TRIANGLES )
						bPicked = pickIndexedTriangles(ray, indexBuffer);
					
					if(indexBuffer->getPrimitiveType() == QUADS )
						bPicked = pickIndexedQuads(ray, indexBuffer);

					if( bPicked )
						bIsAnyPicked = true;
				}
				else
				{
					if(meshBuffer->getPrimitiveType() == TRIANGLES )
						bPicked = pickTriangles(ray, meshBuffer);

					if(meshBuffer->getPrimitiveType() == QUADS )
						bPicked = pickQuads(ray, meshBuffer);

					if( bPicked )
						bIsAnyPicked = true;
				}
			}
			return bIsAnyPicked;
		}

		Mesh::Mesh()
		{
			m_pMeshNode = 0;
			m_pAppearance = 0;
			m_Octree = 0;
			m_vecOctreeNodes = new vector<OctreeNode*>();
		}

		void Mesh::constructOctree()
		{
			if( !m_Octree )
				m_Octree = new Octree(this);
		}

		Mesh::Mesh(const Mesh& other)
		{
		}

		Mesh& Mesh::operator= (const Mesh& other)
		{
			return *this;
		}

		Mesh:: ~Mesh()
		{
		}

		//////////////////////////////////////////////
		/////////////////////////////////////////////

		bool Mesh::pickTriangles(Ray ray, MeshBuffer* meshBuffer)
		{
			return false;
		}

		bool Mesh::pickIndexedTriangles(Ray ray, IndexedMeshBuffer* indexBuffer)
		{
			bool bPicked = false;
			sf32* vertices  = indexBuffer->getCoordinates();
			sf32* normals   = indexBuffer->getNormals();
			sf32* texcoords = indexBuffer->getTexCoords(0);
			su16* indices   = indexBuffer->getIndices();
			// Store the original ray, for calculating the intersection point
			Ray originalRay = ray;
			// Transform the ray, into this meshNode's Local space.
			// this is done because the coordinates of triangles are specified in local space
			if( m_pMeshNode )
			{
				Matrix4f parentTransform = m_pMeshNode->globalTransform;
				parentTransform = parentTransform.getInverse();
				ray.transform(parentTransform);
			}
			
			// Set Maximum Distance.
			float initialDistanceSquared = 10000.0f;
			m_pMeshNode->m_pickResult.m_NumIntersections = 0;

			// Check Intersection
			for( unsigned i=0; i<indexBuffer->getIndexCount(); i+=3 )
			{
				su16 index0 = 3*indices[i];
				su16 index1 = 3*indices[i+1];
				su16 index2 = 3*indices[i+2];

				su16 texindex0 = 2*indices[i];
				su16 texindex1 = 2*indices[i+1];
				su16 texindex2 = 2*indices[i+2];
				
				Point3 v0(vertices[index0], vertices[index0+1], vertices[index0+2]);
				Point3 v1(vertices[index1], vertices[index1+1], vertices[index1+2]);
				Point3 v2(vertices[index2], vertices[index2+1], vertices[index2+2]);
				
				Point3 t0, t1, t2;
				Point3 n0, n1, n2;

				if(texcoords)
				{
					t0.set(texcoords[texindex0], texcoords[texindex0+1], 0.0f);
					t1.set(texcoords[texindex1], texcoords[texindex1+1], 0.0f);
					t2.set(texcoords[texindex2], texcoords[texindex2+1], 0.0f);
				}
				if( normals )
				{
					n0.set(normals[index0], normals[index0+1], normals[index0+2]);
					n1.set(normals[index1], normals[index1+1], normals[index1+2]);
					n2.set(normals[index2], normals[index2+1], normals[index2+2]);
				}

				Point3 outWorld;
				Point3 outLocal;
				
				float u,v,t;
				// Ray Triangle Intersection
				sbool flag = ray.intersectTriangle(v0,v2,v1,&u,&v,&t);
				if ( flag )
				{
					// Intersected Point in Global World space
					outWorld.x = originalRay.origin.x + t*originalRay.direction.x;
					outWorld.y = originalRay.origin.y + t*originalRay.direction.y;
					outWorld.z = originalRay.origin.z + t*originalRay.direction.z;

					outLocal.x = ray.origin.x + t*ray.direction.x;
					outLocal.y = ray.origin.y + t*ray.direction.y;
					outLocal.z = ray.origin.z + t*ray.direction.z;

					bPicked = true;
					//Fill in the PickResult DataStructure
					m_pMeshNode->m_pickResult.m_pMesh = this;
					m_pMeshNode->m_pickResult.m_pNode = m_pMeshNode;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointLocal = outLocal;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveType = TRIANGLES;
					m_pMeshNode->m_pickResult.m_NumIntersections++;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointGlobal = outWorld;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[0] = v0;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[1] = v1;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[2] = v2;
					
					m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.x = t2.x*v + t1.x*u + (1-u-v)*t0.x;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.y = t2.y*v + t1.y*u + (1-u-v)*t0.y;
					
					m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.x = n2.x*v + n1.x*u + (1-u-v)*n0.x;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.y = n2.y*v + n1.y*u + (1-u-v)*n0.y;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.z = n2.z*v + n1.z*u + (1-u-v)*n0.z;

					Vector3 globalNormal(m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal);
					m_pMeshNode->globalTransform.transform(globalNormal);
					m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalGlobal = globalNormal;

					float distanceSq = originalRay.origin.distSquared(outWorld);
					// Store the closest Intersection Point Distance
					if ( distanceSq < initialDistanceSquared )
					{
						initialDistanceSquared = distanceSq;
						m_pMeshNode->m_pickResult.m_PickDistanceSquared = distanceSq;
					}
				}
			}
			return bPicked;
		}

		bool Mesh::pickQuads(Ray ray, MeshBuffer* meshBuffer)
		{
			Ray originalRay = ray;

			if( m_pMeshNode )
			{
				Matrix4f parentTransform = m_pMeshNode->globalTransform;
				parentTransform = parentTransform.getInverse();
				ray.transform(parentTransform);
			}

			float initialDistanceSquared = 10000.0f;
			bool bPicked = false;
			sf32* vertices = meshBuffer->getCoordinates();
			sf32* texcoords = meshBuffer->getTexCoords(0);
			sf32* normals = meshBuffer->getNormals();

			for( unsigned i=0; i<meshBuffer->getVertexCount(); i+=4 )
			{
				su16 index0 = 3*i;
				su16 index1 = 3*(i+1);
				su16 index2 = 3*(i+2);
				su16 index3 = 3*(i+3);

				su16 texindex0 = 2*i;
				su16 texindex1 = 2*(i+1);
				su16 texindex2 = 2*(i+2);
				su16 texindex3 = 2*(i+3);

				Point3 v0(vertices[index0], vertices[index0+1], vertices[index0+2]);
				Point3 v1(vertices[index1], vertices[index1+1], vertices[index1+2]);
				Point3 v2(vertices[index2], vertices[index2+1], vertices[index2+2]);
				Point3 v3(vertices[index3], vertices[index3+1], vertices[index3+2]);
				
				Point3 t0, t1, t2, t3;
				Point3 n0, n1, n2;

				if(texcoords)
				{
					t0.set(texcoords[texindex0], texcoords[texindex0+1], 0.0f);
					t1.set(texcoords[texindex1], texcoords[texindex1+1], 0.0f);
					t2.set(texcoords[texindex2], texcoords[texindex2+1], 0.0f);
					t3.set(texcoords[texindex3], texcoords[texindex3+1], 0.0f);
				}

				if( normals )
				{
					n0.set(normals[index0], normals[index0+1], normals[index0+2]);
					n1.set(normals[index1], normals[index1+1], normals[index1+2]);
					n2.set(normals[index2], normals[index2+1], normals[index2+2]);
				}

				Point3 outWorld, outLocal;
				
				float u_1,v_1,t;
				float u_2,v_2;
				sbool flag1 = ray.intersectTriangle(v0,v2,v1,&u_1,&v_1,&t);
				sbool flag2 = ray.intersectTriangle(v0,v3,v2,&u_2,&v_2,&t);

				if ( flag1 || flag2 )
				{
					outWorld.x = originalRay.origin.x + t*originalRay.direction.x;
					outWorld.y = originalRay.origin.y + t*originalRay.direction.y;
					outWorld.z = originalRay.origin.z + t*originalRay.direction.z;
					
					outLocal.x = ray.origin.x + t*ray.direction.x;
					outLocal.y = ray.origin.y + t*ray.direction.y;
					outLocal.z = ray.origin.z + t*ray.direction.z;

					bPicked = true;
					m_pMeshNode->m_pickResult.m_pMesh = this;
					m_pMeshNode->m_pickResult.m_pNode = m_pMeshNode;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointLocal = outLocal;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveType = QUADS;

					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[0] = v0;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[1] = v1;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[2] = v2;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[3] = v3;

					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointGlobal = outWorld;
					
					if( flag1 )
					{
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.x = t2.x*v_1 + t1.x*u_1 + (1-u_1-v_1)*t0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.y = t2.y*v_1 + t1.y*u_1 + (1-u_1-v_1)*t0.y;
		
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.x = n2.x*v_1 + n1.x*u_1 + (1-u_1-v_1)*n0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.y = n2.y*v_1 + n1.y*u_1 + (1-u_1-v_1)*n0.y;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.z = n2.z*v_1 + n1.z*u_1 + (1-u_1-v_1)*n0.z;

						Vector3 globalNormal(m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal);
						m_pMeshNode->globalTransform.transform(globalNormal);
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalGlobal = globalNormal;
					}
					if( flag2 )
					{
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.x = t3.x*v_2 + t2.x*u_2 + (1-u_2-v_2)*t0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.y = t3.y*v_2 + t2.y*u_2 + (1-u_2-v_2)*t0.y;

						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.x = n2.x*v_2 + n1.x*u_2 + (1-u_2-v_2)*n0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.y = n2.y*v_2 + n1.y*u_2 + (1-u_2-v_2)*n0.y;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.z = n2.z*v_2 + n1.z*u_2 + (1-u_2-v_2)*n0.z;

						Vector3 globalNormal(m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal);
						m_pMeshNode->globalTransform.transform(globalNormal);
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalGlobal = globalNormal;
					}


					float distanceSq = originalRay.origin.distSquared(outWorld);
					if ( distanceSq < initialDistanceSquared )
					{
						initialDistanceSquared = distanceSq;
						m_pMeshNode->m_pickResult.m_PickDistanceSquared = distanceSq;
					}
					//printf("QUAD No %d Picked\n", i/4);
				}
			}
			return bPicked;
		}

		bool Mesh::pickIndexedQuads(Ray ray, IndexedMeshBuffer* indexBuffer)
		{
			sbool bPicked = false;
			sbool bOptimizedPicking = false;
			sf32* vertices = indexBuffer->getCoordinates();
			sf32* normals = indexBuffer->getNormals();
			sf32* texcoords = indexBuffer->getTexCoords(0);
			su16* indices  = indexBuffer->getIndices();
			
			Ray originalRay = ray;
			Octree *octree = 0;
			OctreeNode *rootNode = 0;

			if( m_pMeshNode )
			{
				Octree *octree = m_Octree;
				if( m_pMeshNode->m_bOptimizePicking)
					rootNode = octree->getRootNode();

				Matrix4f parentTransform = m_pMeshNode->globalTransform;
				parentTransform = parentTransform.getInverse();
				ray.transform(parentTransform);
			}
			
			int nIndices = indexBuffer->getIndexCount();
			vector<su16> vecPickedIndices;
			vecPickedIndices.reserve(nIndices);
			if( rootNode )
			{
				m_vecOctreeNodes->clear();
				rootNode->intersectRay(ray, m_vecOctreeNodes );
				for( si32 i=0; i<m_vecOctreeNodes->size(); i++ )
				{
					OctreeNode *octreeNode = ((*m_vecOctreeNodes)[i]);
					vector<su16>& vecIndices = octreeNode->getIndices();
					for( unsigned v=0; v<vecIndices.size(); v++ )
						vecPickedIndices.push_back(vecIndices[v]);
				}
			}
			sf32 initialDistanceSquared = 10000.0f;
			//printf("Total Primitives = %d\n", indexBuffer->getIndexCount() );
			//printf("Picked Primitives = %d\n", vecPickedIndices.size() );
			if( vecPickedIndices.size() > 0 )
			{
				indices = &vecPickedIndices[0];
				nIndices = vecPickedIndices.size();
			}
			//for( unsigned i=0; i<indexBuffer->getIndexCount(); i+=4 )
			for( unsigned i=0; i<nIndices; i+=4 )
			{
				su16 index0 = 3*indices[i];
				su16 index1 = 3*indices[i+1];
				su16 index2 = 3*indices[i+2];
				su16 index3 = 3*indices[i+3];

				Point3 v0(vertices[index0], vertices[index0+1], vertices[index0+2]);
				Point3 v1(vertices[index1], vertices[index1+1], vertices[index1+2]);
				Point3 v2(vertices[index2], vertices[index2+1], vertices[index2+2]);
				Point3 v3(vertices[index3], vertices[index3+1], vertices[index3+2]);
				
				su16 texindex0 = 2*indices[i];
				su16 texindex1 = 2*indices[i+1];
				su16 texindex2 = 2*indices[i+2];
				su16 texindex3 = 2*indices[i+3];
				
				Point3 t0, t1, t2, t3;
				Point3 n0, n1, n2;

				if(texcoords)
				{
					t0.set(texcoords[texindex0], texcoords[texindex0+1], 0.0f);
					t1.set(texcoords[texindex1], texcoords[texindex1+1], 0.0f);
					t2.set(texcoords[texindex2], texcoords[texindex2+1], 0.0f);
					t3.set(texcoords[texindex3], texcoords[texindex3+1], 0.0f);
				}

				if( normals )
				{
					n0.set(normals[index0], normals[index0+1], normals[index0+2]);
					n1.set(normals[index1], normals[index1+1], normals[index1+2]);
					n2.set(normals[index2], normals[index2+1], normals[index2+2]);
				}

				Point3 outWorld;
				Point3 outLocal;

				float u_1,v_1,t;
				float u_2,v_2;
				sbool flag1 = ray.intersectTriangle(v0,v2,v1,&u_1,&v_1,&t);
				float temp = t;
				sbool flag2 = ray.intersectTriangle(v0,v3,v2,&u_2,&v_2,&t);
				if( !flag2)
					t = temp;

				if ( flag1 || flag2 )
				{
					outWorld.x = originalRay.origin.x + t*originalRay.direction.x;
					outWorld.y = originalRay.origin.y + t*originalRay.direction.y;
					outWorld.z = originalRay.origin.z + t*originalRay.direction.z;
					
					outLocal.x = ray.origin.x + t*ray.direction.x;
					outLocal.y = ray.origin.y + t*ray.direction.y;
					outLocal.z = ray.origin.z + t*ray.direction.z;

					bPicked = true;
					m_pMeshNode->m_pickResult.m_pMesh = this;
					m_pMeshNode->m_pickResult.m_pNode = m_pMeshNode;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointLocal = outLocal;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveType = QUADS;

					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[0] = v0;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[1] = v1;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[2] = v2;
					m_pMeshNode->m_pickResult.m_pickIntersection.m_PrimitiveCoordinatesLocal[3] = v3;

					m_pMeshNode->m_pickResult.m_pickIntersection.m_pickPointGlobal = outWorld;
					
					if( flag1 )
					{
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.x = t2.x*v_1 + t1.x*u_1 + (1-u_1-v_1)*t0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.y = t2.y*v_1 + t1.y*u_1 + (1-u_1-v_1)*t0.y;
		
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.x = n2.x*v_1 + n1.x*u_1 + (1-u_1-v_1)*n0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.y = n2.y*v_1 + n1.y*u_1 + (1-u_1-v_1)*n0.y;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.z = n2.z*v_1 + n1.z*u_1 + (1-u_1-v_1)*n0.z;

						Vector3 globalNormal(m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal);
						m_pMeshNode->globalTransform.transform(globalNormal);
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalGlobal = globalNormal;
					}
					if( flag2 )
					{
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.x = t3.x*v_2 + t2.x*u_2 + (1-u_2-v_2)*t0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_TextureCoordinate.y = t3.y*v_2 + t2.y*u_2 + (1-u_2-v_2)*t0.y;

						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.x = n2.x*v_2 + n1.x*u_2 + (1-u_2-v_2)*n0.x;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.y = n2.y*v_2 + n1.y*u_2 + (1-u_2-v_2)*n0.y;
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal.z = n2.z*v_2 + n1.z*u_2 + (1-u_2-v_2)*n0.z;
						
						Vector3 globalNormal(m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalLocal);
						m_pMeshNode->globalTransform.transform(globalNormal);
						m_pMeshNode->m_pickResult.m_pickIntersection.m_NormalGlobal = globalNormal;
					}


					float distanceSq = originalRay.origin.distSquared(outWorld);
					if ( distanceSq < initialDistanceSquared )
					{
						initialDistanceSquared = distanceSq;
						m_pMeshNode->m_pickResult.m_PickDistanceSquared = distanceSq;
					}
				}
			}
			return bPicked;
		}

	}

}

