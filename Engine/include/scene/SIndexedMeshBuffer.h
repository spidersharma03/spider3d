#ifndef _SINDEXEDMESHBUFFER_H

#define _SINDEXEDMESHBUFFER_H

#include "IndexedMeshBuffer.h"

namespace spider3d
{
	namespace scene
	{
		class SIndexedMeshBuffer:public IndexedMeshBuffer
		{
		public:

			sf32* getCoordinates() const
			{
				return coordinates;
			}

			su32 getVertexCount() const
			{
				return vertexCount;
			}

			sf32* getColors() const
			{
				return colors;
			}

			sf32* getTexCoords(su32 texCoordSet) const
			{
				if ( texCoordSet >= numTexCoordSet || texCoordSet < 0 )
				{
					//printf("Illegal TexCoordSet\n");
					return 0;
				}
				return vecTexCoords[texCoordSet];
			}

			sf32* getTexCoords() const
			{
				return texcoords;
			}

			vector<sf32*>& getAllTexCoords()
			{
				return vecTexCoords;
			}

			vector<sf32*>& getAllVertexAttributes()
			{
				return m_vecVertexAttributes;
			}

			vector<ShaderVertexAttributeSet*>& getAllShaderVertexAttributes()
			{
				return m_vecShaderVertexAttributes;
			}

			sf32* getNormals() const
			{
				return normals;
			}

			su16* getIndices() const
			{
				return indices;
			}

			void setCoordinates(sf32* inCoords)
			{
				if ( (vertexFormat & COORDINATES) != 0 )
				{
					for( su32 i = 0 ; i < 3*vertexCount ; i++ )
						*(coordinates + i)  = *(inCoords + i);
				}
			}

			virtual void setNormals(sf32* inNormals)
			{
				if ( (vertexFormat & NORMALS) != 0 )
				{
					for( su32 i = 0 ; i < 3*vertexCount ; i++ )
						*( normals + i ) = *( inNormals + i );
				}
			}

			void setColors(sf32* inColors)
			{
				if ( (vertexFormat & 0x08) != 0 )
			 {
				 for( su32 i = 0 ; i < 4*vertexCount ; i++ )
					 *( colors + i ) = *( inColors + i );
			 }
				else 
			 {
				 for( su32 i = 0 ; i < 3*vertexCount ; i++ )
					 *( colors + i ) = *( inColors + i );
			 }
			}

			void setTexCoords(sf32* inTexcoords)
			{
				if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
				{
					for( su32 i = 0 ; i < 3*vertexCount ; i++ )
						*( texcoords + i ) = *( inTexcoords + i );
				}
				if ( (vertexFormat & TEXTURE_COORDINATE_2) != 0 )
				{
					for( su32 i = 0 ; i < 2*vertexCount ; i++ )
						*( texcoords + i ) = *( inTexcoords + i );
				}
			}

			void setTexCoords(su32 texCoordSet , sf32* inTexcoords)
			{
				if ( texCoordSet >= numTexCoordSet || texCoordSet < 0)
				{
					printf("Illegal TexCoordSet\n");
					return;
				}

				sf32* texcoords = vecTexCoords[texCoordSet];
				for( su32 i = 0 ; i < texCoordSize*vertexCount ; i++ )
					*( texcoords + i ) = *( inTexcoords + i );
			}

			void setVertexAttributes( su32 vertexAttributeIndex, sf32* vertexAttributes)
			{
				if ( vertexAttributeIndex >= m_vertexAttributeCount || vertexAttributeIndex < 0)
				{
					printf("Illegal vertexAttributeIndex\n");
					return;
				}

				sf32* vertexAttrs = m_vecShaderVertexAttributes[vertexAttributeIndex]->m_ShaderVertexAttributePointer;
				su32 size = m_vertexAttributeSizes[vertexAttributeIndex]*vertexCount;
				for( su32 i = 0 ; i < size ; i++ )
					*( vertexAttrs + i ) = *( vertexAttributes + i );
			}

			void setIndices(su16* inIndices)
			{
				for( su32 i = 0 ; i < indexCount ; i++ )
					*( indices + i ) = *( inIndices + i );
			}

			su32 getIndexCount() const
			{
				return indexCount;
			}

			virtual su32 getPrimitiveCount() const
			{
				return 0;
			}

			void setPrimitiveType(PRIMITIVE_TYPE type)  
			{
				this->type = type;
			}


			PRIMITIVE_TYPE getPrimitiveType() const
			{
				return type;
			}

			SIndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 indexCount ):IndexedMeshBuffer(vertexCount , vertexFormat , indexCount)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				indices = 0;
				vertexSize = 3;
				if ( (vertexFormat & COORDINATES) == 0 )
				{
					printf("Illegal Format::Must have COORDINATES\n");
				}
				else
				{
					coordinates = new float[vertexCount*3];
				}
				if ( (vertexFormat & NORMALS) != 0 )
				{
					normals  = new float[vertexCount*3];
				}

				if ( (vertexFormat & 0x04 ) != 0 )
				{
					if ( (vertexFormat & 0x08 ) != 0 )
					{
						colors  = new float[vertexCount*4]; 
						colorSize = 4;
					}
					else
					{
						colors  = new float[vertexCount*3]; 
						colorSize = 3;
					}
				}

				if ( ( vertexFormat & ( TEXTURE_COORDINATE_2 | TEXTURE_COORDINATE_3 ) ) != 0 )
				{
					if ( (vertexFormat & TEXTURE_COORDINATE_2) != 0 )
					{
						texCoordSize  = 2;
						texcoords  = new float[vertexCount*texCoordSize];
						vecTexCoords.push_back( texcoords );
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						texCoordSize  = 3;
						texcoords  = new float[vertexCount*texCoordSize];
						vecTexCoords.push_back( texcoords );
					}

				}

				indices = new su16[indexCount];

			}

			SIndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet , su32 indexCount ):IndexedMeshBuffer(vertexCount , vertexFormat , numTexCoordSet , indexCount)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				indices = 0;
				vertexSize = 3;
				if ( (vertexFormat & COORDINATES) == 0 )
				{
					 printf("Illegal Format::Must have COORDINATES\n");
				}
				else
				{
					coordinates = new float[vertexCount*3];
				}
				if ( (vertexFormat & NORMALS) != 0 )
				{
					normals  = new float[vertexCount*3];
				}

				if ( (vertexFormat & 0x04 ) != 0 )
				{
					if ( (vertexFormat & 0x08 ) != 0 )
					{
						colors  = new float[vertexCount*4]; 
						colorSize = 4;
					}
					else
					{
						colors  = new float[vertexCount*3]; 
						colorSize = 3;
					}
				}

				if ( ( vertexFormat & ( TEXTURE_COORDINATE_2 | TEXTURE_COORDINATE_3 | TEXTURE_COORDINATE_4 ) ) != 0 )
				{
					if ( (vertexFormat & TEXTURE_COORDINATE_2) != 0 )
					{
						texCoordSize  = 2;
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*texCoordSize];
							vecTexCoords.push_back( texcoords );
						}
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						texCoordSize  = 3;
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*texCoordSize];
							vecTexCoords.push_back( texcoords );
						}
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_4) != 0 )
					{
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*4];
							memset(texcoords,1,vertexCount*4);
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 4;
					}
				}

				indices = new su16[indexCount];

			}

			SIndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet, si32 vertexAttributeCount, si32* vertexAttributeSizes , su32 indexCount ):IndexedMeshBuffer(vertexCount , vertexFormat , numTexCoordSet, vertexAttributeCount, vertexAttributeSizes , indexCount)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				indices = 0;
				vertexSize = 3;
				if ( (vertexFormat & COORDINATES) == 0 )
			 {
				 printf("Illegal Format::Must have COORDINATES\n");
			 }
				else
				{
					coordinates = new float[vertexCount*3];
				}
				if ( (vertexFormat & NORMALS) != 0 )
				{
					normals  = new float[vertexCount*3];
				}

				if ( (vertexFormat & 0x04 ) != 0 )
				{
					if ( (vertexFormat & 0x08 ) != 0 )
					{
						colors  = new float[vertexCount*4]; 
						colorSize = 4;
					}
					else
					{
						colors  = new float[vertexCount*3]; 
						colorSize = 3;
					}
				}

				if ( ( vertexFormat & ( TEXTURE_COORDINATE_2 | TEXTURE_COORDINATE_3 ) ) != 0 )
				{
					if ( (vertexFormat & TEXTURE_COORDINATE_2) != 0 )
					{
						texCoordSize  = 2;
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*texCoordSize];
							vecTexCoords.push_back( texcoords );
						}
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						texCoordSize  = 3;
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*texCoordSize];
							vecTexCoords.push_back( texcoords );
						}
					}

					if ( (vertexFormat & TEXTURE_COORDINATE_4) != 0 )
					{
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*4];
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 4;
					}
				}

				if ( (vertexFormat & VERTEX_ATTRIBUTES) != 0 )
				{
					for ( unsigned i = 0 ; i < m_vertexAttributeCount ; i++ )
					{
						sf32 *vertexAttributes = new sf32[m_vertexAttributeSizes[i]*vertexCount];
						ShaderVertexAttributeSet *vaSet = new ShaderVertexAttributeSet();
						vaSet->m_ShaderVertexAttributePointer = vertexAttributes;
						vaSet->m_Size = m_vertexAttributeSizes[i];
						m_vecShaderVertexAttributes.push_back(vaSet);
						m_vecVertexAttributes.push_back(vertexAttributes);
					}
				}

				indices = new su16[indexCount];

			}

			AABB& getAABB()
			{
				return BoundingBox;
			}

			void setAABB(AABB& other)
			{
				BoundingBox.set(other);
			}

			void reCalculateAABB()
			{
				if ( !coordinates )
				{
					BoundingBox.MaxEdge.set(0.0f,0.0f,0.0f);
					BoundingBox.MinEdge.set(0.0f,0.0f,0.0f);
				}
				else
				{
					for (su32 i=0; i<3*vertexCount; i+=3 )
						BoundingBox.addPoint(coordinates[i] ,  coordinates[i+1] , coordinates[i+2]);
				}
				BoundingBox.reCalculateAABB();
			}

			virtual ~SIndexedMeshBuffer()
			{
				delete[] coordinates;
				delete[] normals;
				delete[] colors;
				delete[] texcoords;
				delete[] indices;

				for( unsigned i = 0 ; i < vecTexCoords.size() ; i++ )
					delete[] vecTexCoords[i];
				vecTexCoords.clear();
				for( unsigned i = 0 ; i < m_vecVertexAttributes.size() ; i++ )
					delete[] m_vecVertexAttributes[i];
				m_vecVertexAttributes.clear();
			}

		private:

			sf32* coordinates;
			sf32* normals;
			sf32* colors;
			sf32* texcoords;
			vector<sf32*> vecTexCoords;
			vector<sf32*> m_vecVertexAttributes;
			vector<ShaderVertexAttributeSet*> m_vecShaderVertexAttributes;
			su16* indices;
			AABB BoundingBox;
		};

	}

}

#endif