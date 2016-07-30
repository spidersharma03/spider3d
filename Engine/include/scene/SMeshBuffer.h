#ifndef _SMESHBUFFER_H

#define _SMESHBUFFER_H

#include "MeshBuffer.h"

namespace spider3d
{
	namespace scene
	{
		class SMeshBuffer:public MeshBuffer
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
				if ( texCoordSet > numTexCoordSet || texCoordSet < 0 )
				{
					printf("Illegal TexCoordSet\n");
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

			virtual void setColors(sf32* inColors)
			{
			   if ( (vertexFormat & 0x04 ) != 0 )
				{
					if ( (vertexFormat & 0x08 ) != 0 )
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
			}

			virtual void setTexCoords(sf32* inTexcoords)
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

			virtual void setTexCoords(su32 texCoordSet , sf32* inTexcoords)
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
				
				sf32* vertexAttrs = m_vecVertexAttributes[vertexAttributeIndex];
				su32 size = m_vertexAttributeSizes[vertexAttributeIndex]*vertexCount;
				for( su32 i = 0 ; i < size ; i++ )
					*( vertexAttrs + i ) = *( vertexAttributes + i );
			}

			void setPrimitiveType(PRIMITIVE_TYPE type)  
			{
				this->type = type;
			}

			PRIMITIVE_TYPE getPrimitiveType() const
			{
				return type;
			}

			sbool isIndexed() const
			{
				return false;
			}

			SMeshBuffer(su32 vertexCount , su32 vertexFormat):MeshBuffer(vertexCount , vertexFormat)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				vertexSize = 3;
				m_GeometryRenderingMode = EGRM_VERTEX_ARRAYS;
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
						texcoords  = new float[vertexCount*2];
						vecTexCoords.push_back( texcoords );
						texCoordSize = 2;
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						texcoords  = new float[vertexCount*3];
						vecTexCoords.push_back( texcoords );
						texCoordSize = 3;
					}
				}

			};

			SMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet):MeshBuffer(vertexCount , vertexFormat, numTexCoordSet)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				vertexSize = 3;
				m_GeometryRenderingMode = EGRM_VERTEX_ARRAYS;
				if ( (vertexFormat & COORDINATES) == 0 )
			    {
				 printf("Illegal Format::Must have COORDINATES\n");
				 return;
			    }

				if ( numTexCoordSet < 1 )
				{
				 printf("Illegal Number of TexCoordSet::Must be Greater than 0\n");
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
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*2];
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 2;
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*3];
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 3;
					}
				}

			};

			SMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet, si32 vertexAttributeCount, si32* vertexAttributeSizes):MeshBuffer(vertexCount , vertexFormat, numTexCoordSet, vertexAttributeCount, vertexAttributeSizes)
			{
				coordinates = 0;
				normals = 0;
				colors = 0;
				texcoords = 0;
				vertexSize = 3;
				m_GeometryRenderingMode = EGRM_VERTEX_ARRAYS;
				if ( (vertexFormat & COORDINATES) == 0 )
			    {
				 printf("Illegal Format::Must have COORDINATES\n");
				 return;
			    }

				if ( numTexCoordSet < 1 )
				{
				 printf("Illegal Number of TexCoordSet::Must be Greater than 0\n");
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
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*2];
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 2;
					}
					if ( (vertexFormat & TEXTURE_COORDINATE_3) != 0 )
					{
						for ( unsigned i = 0 ; i < numTexCoordSet ; i++ )
						{
							texcoords  = new float[vertexCount*3];
							vecTexCoords.push_back( texcoords );
						}
						texCoordSize = 3;
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

			};

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

			virtual ~SMeshBuffer()
			{
				delete[] coordinates;
				delete[] normals;
				delete[] colors;
				delete[] texcoords;
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
			AABB BoundingBox;
		};

	}

}

#endif