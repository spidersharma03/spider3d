#ifndef _OCTREE_H
#define _OCTREE_H

#include "coreMath.h"
#include "spiderDataTypes.h"
#include "AABB.h"
#include <vector>
#include "Ray.h"
#include "Mesh.h"
#include "IndexedMeshBuffer.h"

using namespace std;
using namespace spider3d::core;
using namespace spider3d::scene;
using namespace spider3d;

#define minimumPolygons 10
//#define PACKED_OCTREE
namespace spider3d
{
	namespace picking
	{

		class OctreeNode 
		{
		public:

			OctreeNode(sf32* vertices, vector<su16>* vecIndices, int currentDepth, AABB bbox)
			{
				m_Depth = 0;
				//m_NodeCount++;
				// Set the Children to Point to Nothing. later this can be used as a check for leaf nodes.
				for (int c=0; c<8; c++)
				{
					m_Children[c] = 0;
				}

				// if index list is empty, delete it and release memory.
				if( vecIndices->size() <= 0 )
				{
					delete vecIndices;
					return;
				}

				// Find Bounding box of this Node
#ifdef PACKED_OCTREE
				for (unsigned v=0; v<vecIndices->size(); v++) 
				{				
					su16 index0 = 3*( (*vecIndices)[v] );

					Point3 vertex( vertices[index0], vertices[index0+1], vertices[index0+2]);

					m_BoundingBox.addPoint(vertex);
				}
				m_BoundingBox.reCalculateAABB();
#else
				m_BoundingBox = bbox;
#endif		

				// If number of Primtives are greater than the threshold
				float primitiveCount = (float)vecIndices->size()/4.0f;
				// Create Octree Child Nodes only if the primitivCount is more than the Threshold 
				if( primitiveCount > minimumPolygons )
				{
					m_Depth = currentDepth;
					// Iterate through each Child
					for (int c=0; c<8; c++)
					{
						bool bInside = false;
						// Bounding box of this Child
						AABB aabb;
						// Indices which will not be inside this AABB.
						vector<su16> leftOutIndices;
						// Indices which will be part of this AABB.
						vector<su16> *tempIndices = new vector<su16>();
						// define the Child AABB
						aabb.MinEdge = m_BoundingBox.getCentre();
						aabb.MaxEdge = m_BoundingBox.getCentre();
						aabb.addPoint(m_BoundingBox.getEdge(c));
						aabb.reCalculateAABB();

						vector<unsigned short>::iterator it;
						// Iterate through the primitives
						for ( it=vecIndices->begin() ; it < vecIndices->end(); it+=4)
						{
							su16 index0 = 3*(*it);
							su16 index1 = 3*(*(it+1));
							su16 index2 = 3*(*(it+2));
							su16 index3 = 3*(*(it+3));

							// Primitive Vertices
							Point3 vertex0( vertices[index0], vertices[index0+1], vertices[index0+2]);
							Point3 vertex1( vertices[index1], vertices[index1+1], vertices[index1+2]);
							Point3 vertex2( vertices[index2], vertices[index2+1], vertices[index2+2]);
							Point3 vertex3( vertices[index3], vertices[index3+1], vertices[index3+2]);

							// Find if the vertices of this Primitive are within the AABB of this Child
							bool bVertexInside[] = { aabb.isPointInside(vertex0), aabb.isPointInside(vertex1), aabb.isPointInside(vertex2), aabb.isPointInside(vertex3) };
							int insideCounter = 0;
							for( int i=0; i<4; i++ )
							{
								insideCounter = bVertexInside[i] ? 	insideCounter+1 : insideCounter;
							}
							// Vertices Found inside. Now push them into the vector
							if( insideCounter == 2)//< 4 && insideCounter > 0 )
							{
								//nMissed += 4;
							}
							// Primitive is Cut By this Octree Node's AABB. Add these Primitives into this Node.
							if(insideCounter > 0 && insideCounter < 3)
							{
								m_vecIndices.push_back(index0/3);
								m_vecIndices.push_back(index1/3);
								m_vecIndices.push_back(index2/3);
								m_vecIndices.push_back(index3/3);
							}
							// Primitive is Completely Inside this Octree Node's AABB
							else if( insideCounter > 3 )
							{
								bInside = true;

								tempIndices->push_back(index0/3);
								tempIndices->push_back(index1/3);
								tempIndices->push_back(index2/3);
								tempIndices->push_back(index3/3);
							}
							// Vertices were not found inside this Octree Node's AABB, so push them in the oter vector
							// Primitive is Outside this Octree Node's AABB
							else// if(insideCounter > 1) 
							{
								leftOutIndices.push_back(index0/3);
								leftOutIndices.push_back(index1/3);
								leftOutIndices.push_back(index2/3);
								leftOutIndices.push_back(index3/3);
							}
						}

						// If there are a few Primitives inside this Node, Create Child Node of this Octree Node
						if( bInside )
						{
							vecIndices->resize(leftOutIndices.size());
							for( unsigned i=0; i<leftOutIndices.size(); i++ )
								(*vecIndices)[i] = leftOutIndices[i];
							//memcpy(vecIndices, &leftOutIndices, leftOutIndices.size()*sizeof(unsigned short));
							leftOutIndices.clear();
							m_Children[c] = new OctreeNode(vertices, tempIndices, m_Depth+1, aabb);
						}
						else 
						{
							leftOutIndices.clear();
							tempIndices->clear();
							delete tempIndices;
						}
					}
				}
				// If number of Primtives are Less than the threshold, then do not create any child, copy the indices into this Node's index list, and delete the original index list.
				else 
				{
					for (unsigned v=0; v<vecIndices->size(); v++) 
					{
						m_vecIndices.push_back((*vecIndices)[v]);
					}
					vecIndices->clear();
					delete vecIndices;
					return;
				}

			}

			bool intersectRay(Ray& ray, vector<OctreeNode*>* vecOctreeNodes)
			{
				bool flag = ray.intersectAABB(m_BoundingBox);
				if( flag )
				{
					vecOctreeNodes->push_back(this);
					for( int c=0; c<8; c++ )
					{
						if( m_Children[c] )
						{
							m_Children[c]->intersectRay(ray, vecOctreeNodes);
						}
					}
				}

				return vecOctreeNodes->size() > 0;
			}

			OctreeNode* getChild(int c)
			{
				return m_Children[c];
			}

			vector<unsigned short>& getIndices()
			{
				return m_vecIndices;
			}

			int getDepth()
			{
				return m_Depth;
			}

			AABB m_BoundingBox;

			vector<AABB*> getAllAABB(OctreeNode* root);

		private:

			OctreeNode* m_Children[8];

			vector<unsigned short> m_vecIndices;

			unsigned short m_Depth;
		};




		class Octree
		{
		public:
			Octree(Mesh* mesh)
			{
				MeshBuffer& meshBuffer = mesh->getMeshBuffer(0);
				sf32 *vertices = meshBuffer.getCoordinates();
				vector<su16> vecIndices;
				AABB& aabb = meshBuffer.getAABB();
				aabb.MaxEdge.x += 0.001f;
				aabb.MaxEdge.y += 0.001f;
				aabb.MaxEdge.z += 0.001f;

				aabb.MinEdge.x -= 0.001f;
				aabb.MinEdge.y -= 0.001f;
				aabb.MinEdge.z -= 0.001f;

				if( meshBuffer.isIndexed() )
				{
					IndexedMeshBuffer& indexedBuffer = (IndexedMeshBuffer&)meshBuffer;
					su16* indices = indexedBuffer.getIndices();
					for( si32 i=0; i<indexedBuffer.getIndexCount(); i++ )
						vecIndices.push_back(indices[i]);
				}
				else
				{
					for( si32 i=0; i<meshBuffer.getVertexCount()*3; i++ )
						vecIndices.push_back(i);
				}
				rootNode = new OctreeNode(vertices, &vecIndices, 0, aabb );
			}

			OctreeNode* getRootNode()
			{
				return rootNode;
			}

		private:
			OctreeNode *rootNode;
		};

	}
}

#endif