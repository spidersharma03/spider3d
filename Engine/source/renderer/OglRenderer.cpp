#include "OglRenderer.h"
#include "Appearance.h"
#include "ShaderAppearance.h"
#include "GLSLShaderProgram.h"
#include "CameraNode.h"
#include "Image.h"

using namespace std;

float data[16];
unsigned Appearance::MAX_TEXTURE_UNITS;

bool bEnableOglTexCoordgeneration[8];
int texComponent;
int currTexUnitId;
bool bPointAttributesRequired = false;
bool bLineAttributesRequired = false;
bool bDirtyBlending = false;
int MAX_TEX_UNITS = 3;

namespace spider3d
{
	namespace render
	{

		OglRenderer* OglRenderer::oglRenderer = 0;

		void OglRenderer::drawIndexedMeshBuffer(IndexedMeshBuffer &meshBuffer)
		{
			su32 primitiveCount = 0;
			switch ( meshBuffer.getPrimitiveType() ) 
			{
			case TRIANGLES:
				primitiveCount = meshBuffer.getIndexCount()/3;
				break;
			case QUADS:
				primitiveCount = meshBuffer.getIndexCount()/4;
				break;
			}
			if( currentAppearance )
			{
				// If it is a ShaderAppearance
				if( currentAppearance->isShaderAppearance() )
				{
					ShaderProgram* sp = ((ShaderAppearance*)currentAppearance)->getShaderProgram();
					if( sp )
					{
						vector<ShaderVertexAttributeSet*>& vaS = meshBuffer.getAllShaderVertexAttributes();
						vector<int>& vecAttrLoc = sp->getAllAttributeLocations();
						if( vecAttrLoc.size() != vaS.size() )
						{
							printf("Error::ShaderVertexAttributes size mismatch\n");
						}
						else
						{
							for( int i=0; i<vaS.size(); i++ )
							{
								vaS[i]->m_Location = vecAttrLoc[i];
							}
						}
					}
				}
			}
			drawIndexedVertexPrimitiveList(meshBuffer.getVertexCount()
				, meshBuffer.getCoordinates()
				, meshBuffer.getVertexSize()
				, meshBuffer.getNormals()
				, meshBuffer.getColors()
				, meshBuffer.getColorSize()
				, meshBuffer.getAllTexCoords()
				, meshBuffer.getTexCoordSize()
				, meshBuffer.getAllShaderVertexAttributes()
				, meshBuffer.getIndices()
				, primitiveCount , meshBuffer.getPrimitiveType() ); 
		}


		void OglRenderer::drawMeshBuffer(MeshBuffer &meshBuffer)
		{
			if( currentAppearance )
			{
				// If it is a ShaderAppearance
				if( currentAppearance->isShaderAppearance() )
				{
					ShaderProgram* sp = ((ShaderAppearance*)currentAppearance)->getShaderProgram();
					if( sp )
					{
						vector<ShaderVertexAttributeSet*>& vaS = meshBuffer.getAllShaderVertexAttributes();
						vector<int>& vecAttrLoc = sp->getAllAttributeLocations();
						if( vecAttrLoc.size() != vaS.size() )
						{
							printf("Error::ShaderVertexAttributes size mismatch\n");
						}
						else
						{
							for( int i=0; i<vaS.size(); i++ )
							{
								vaS[i]->m_Location = vecAttrLoc[i];
							}
						}
					}
				}
			}

			drawVertexPrimitiveList(meshBuffer.getVertexCount()
				, meshBuffer.getCoordinates()
				, meshBuffer.getVertexSize()
				, meshBuffer.getNormals()
				, meshBuffer.getColors()
				, meshBuffer.getColorSize()
				, meshBuffer.getAllTexCoords()
				, meshBuffer.getTexCoordSize()
				, meshBuffer.getAllShaderVertexAttributes() 
				, meshBuffer.getPrimitiveType() ); 
		}



		void OglRenderer::drawIndexedVertexPrimitiveList
			(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize, sf32* texcoords, si32 texCoordSize, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , 0 , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , 0 , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_FLOAT , 0 , colors );
			}
			if ( texcoords )
			{  
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(texCoordSize,GL_FLOAT,0,texcoords);
				GLenum texId = GL_TEXTURE0;
				for ( int t = 0 ; t < MAX_TEX_UNITS ; t ++ )
				{
					glActiveTexture(texId);
					texId++;
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					glDisable(GL_TEXTURE_GEN_R);
				}
				//glDisable(GL_TEXTURE_GEN_Q);
			}

			switch ( type )
			{
			case TRIANGLES:

				glDrawElements(GL_TRIANGLES , primitiveCount*3 , GL_UNSIGNED_SHORT , indices ); 
				break;

			case QUADS:
				glDrawElements(GL_QUADS , primitiveCount*4 , GL_UNSIGNED_SHORT , indices );
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(normals)
				glDisableClientState(GL_NORMAL_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);
			if(texcoords)
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}


		void OglRenderer::drawIndexedVertexPrimitiveList
			(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize, vector<sf32*>& texcoords, si32 texCoordSize, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , 0 , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , 0 , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_FLOAT , 0 , colors );
			}

			GLenum texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if(!currentAppearance)
					break;
				TextureUnitState* tu = currentAppearance->getTextureUnitState(t);
				if(!tu)
					continue;
				sbool bEnable = tu->getTexCoordGeneration()->bEnable;
				if ( texcoords[t] && !bEnable )
				{
					if( GL_ARB_multitexture) 
						glClientActiveTexture(texId);

					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texCoordSize,GL_FLOAT,0,texcoords[t]);
					glActiveTexture(texId);
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					if ( texCoordSize == 3 )
						glDisable(GL_TEXTURE_GEN_R);
					if ( texCoordSize == 4 )
					{
						glDisable(GL_TEXTURE_GEN_R);
						glDisable(GL_TEXTURE_GEN_Q);
					}
				}
				texId++;
			}

			switch ( type )
			{
			case TRIANGLES:

				glDrawElements(GL_TRIANGLES , primitiveCount*3 , GL_UNSIGNED_SHORT , indices ); 
				break;

			case QUADS:
				glDrawElements(GL_QUADS , primitiveCount*4 , GL_UNSIGNED_SHORT , indices );
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(normals)
				glDisableClientState(GL_NORMAL_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);
			texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if( GL_ARB_multitexture)
					glClientActiveTexture(texId);
				if ( texcoords[t] )
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				texId++;
			}
		}


		void OglRenderer::drawIndexedVertexPrimitiveList
			(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize,  vector<sf32*>& texcoords, si32 texCoordSize, vector<ShaderVertexAttributeSet*>& shaderVertexAttributes, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , 0 , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , 0 , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_FLOAT , 0 , colors );
			}
			if( shaderVertexAttributes.size() > 0 )
			{
				for( unsigned v=0; v<shaderVertexAttributes.size(); v++ )
				{
					GLint location = shaderVertexAttributes[v]->m_Location;
					GLint size     = shaderVertexAttributes[v]->m_Size;
					if( location != -1 )
					{
						glEnableVertexAttribArrayARB(location);
						glVertexAttribPointerARB(location,size, GL_FLOAT, false, 0, shaderVertexAttributes[v]->m_ShaderVertexAttributePointer );
					}
				}
			}

			GLenum texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if(!currentAppearance)
					continue;
				TextureUnitState* tu = currentAppearance->getTextureUnitState(t);
				if(!tu)
					continue;
				sbool bEnable = tu->getTexCoordGeneration()->bEnable;
				if ( texcoords[t] && !bEnable )
				{
					if( GL_ARB_multitexture)
						glClientActiveTexture(texId);

					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texCoordSize,GL_FLOAT,0,texcoords[t]);
					glActiveTexture(texId);
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					if ( texCoordSize == 3 )
						glDisable(GL_TEXTURE_GEN_R);
					if ( texCoordSize == 4 )
						glDisable(GL_TEXTURE_GEN_Q);
				}
				texId++;
			}

			switch ( type )
			{
			case TRIANGLES:

				glDrawElements(GL_TRIANGLES , primitiveCount*3 , GL_UNSIGNED_SHORT , indices ); 
				break;

			case QUADS:
				glDrawElements(GL_QUADS , primitiveCount*4 , GL_UNSIGNED_SHORT , indices );
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(normals)
				glDisableClientState(GL_NORMAL_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);

			if( shaderVertexAttributes.size() > 0 )
			{
				for( unsigned v=0; v<shaderVertexAttributes.size(); v++ )
				{
					GLint location = shaderVertexAttributes[v]->m_Location;
					if( location != -1 )
						glDisableVertexAttribArrayARB(location);
				}
			}
			texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if( GL_ARB_multitexture)
					glClientActiveTexture(texId);
				if ( texcoords[t] )
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				texId++;
			}
		}

		void OglRenderer::drawVertexPrimitiveList
			(su32 vertexCount, void* coords , si32 vertexSize , sf32* normals , void* colors, si32 colorSize, sf32* texcoords, si32 texCoordSize, si32 stride, PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , stride , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , stride , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_UNSIGNED_BYTE , stride , colors );
			}

			GLenum texId = GL_TEXTURE0;
			glClientActiveTexture(texId);

			if ( texcoords )
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(texCoordSize,GL_FLOAT,stride,texcoords);
				for ( int t = 0 ; t < MAX_TEX_UNITS ; t ++ )
				{
					glActiveTexture(texId);
					texId++;
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					glDisable(GL_TEXTURE_GEN_R);
					glDisable(GL_TEXTURE_GEN_Q);
				}
			}

			switch ( type )
			{
			case EPT_POINTS:
				glDrawArrays(GL_POINTS , 0 , vertexCount);
				break;

			case LINES:
				glDrawArrays(GL_LINES , 0 , vertexCount);
				break;

			case TRIANGLES:
				glDrawArrays(GL_TRIANGLES , 0 , vertexCount);
				break;

			case QUADS:
				glDrawArrays(GL_QUADS , 0 , vertexCount);
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);
			if(normals) 
				glDisableClientState(GL_NORMAL_ARRAY);

			
			texId = GL_TEXTURE0;
			if( GL_ARB_multitexture)
				glClientActiveTexture(texId);

			if ( texcoords )
			{
				for ( int t = 0 ; t < MAX_TEX_UNITS ; t ++ )
				{
					glActiveTexture(texId);
					glClientActiveTexture(texId);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					texId++;
				}
			}
		}

		void OglRenderer::drawVertexPrimitiveList
			(su32 vertexCount, sf32* coords , si32 vertexSize , sf32* normals , sf32* colors, si32 colorSize, vector<sf32*>& texcoords, si32 texCoordSize, si32 stride, PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , stride , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , stride , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_FLOAT , stride , colors );
			}

			GLenum texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				TextureUnitState* tu = currentAppearance->getTextureUnitState(t);
				if(!tu)
					continue;
				sbool bEnable = tu->getTexCoordGeneration()->bEnable;
				if ( texcoords[t] && !bEnable)
				{
					if( GL_ARB_multitexture)
						glClientActiveTexture(texId);

					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texCoordSize,GL_FLOAT,stride,texcoords[t]);
					glActiveTexture(texId);
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					if ( texCoordSize == 3 )
						glDisable(GL_TEXTURE_GEN_R);
					if ( texCoordSize == 4 )
						glDisable(GL_TEXTURE_GEN_Q);
				}
				texId++;
			}

			switch ( type )
			{
			case EPT_POINTS:
				glDrawArrays(GL_POINTS , 0 , vertexCount);
				break;

			case TRIANGLES:
				glDrawArrays(GL_TRIANGLES , 0 , vertexCount);
				break;

			case QUADS:
				glDrawArrays(GL_QUADS , 0 , vertexCount);
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);
			if(normals) 
				glDisableClientState(GL_NORMAL_ARRAY);

			texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if( GL_ARB_multitexture)
					glClientActiveTexture(texId);
				if ( texcoords[t] )
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				texId++;
			}
		}

		void OglRenderer::drawVertexPrimitiveList(su32 vertexCount, sf32* coords , si32 vertexSize , sf32* normals , sf32* colors, si32 colorSize, vector<sf32*>& texcoords, si32 texCoordSize, vector<ShaderVertexAttributeSet*>& shaderVertexAttributes, PRIMITIVE_TYPE type)
		{
			bPointAttributesRequired = false;
			bLineAttributesRequired = false;
			texComponent = texCoordSize;
			if( type == EPT_POINTS )
				bPointAttributesRequired = true;

			if( type == LINES )
				bLineAttributesRequired = true;

			setRenderState(currentAppearance);

			if ( coords )
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize , GL_FLOAT , 0 , coords );
			}
			if ( normals )
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT , 0 , normals );
			}
			if ( colors )
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorSize , GL_FLOAT , 0 , colors );
			}

			if( shaderVertexAttributes.size() > 0 )
			{
				for( unsigned v=0; v<shaderVertexAttributes.size(); v++ )
				{
					GLint location = shaderVertexAttributes[v]->m_Location;
					GLint size     = shaderVertexAttributes[v]->m_Size;
					if( location != -1 )
					{
						glEnableVertexAttribArrayARB(location);
						glVertexAttribPointerARB(location,size, GL_FLOAT, false, 0, shaderVertexAttributes[v]->m_ShaderVertexAttributePointer );
					}
				}
			}
			
			GLenum texId = GL_TEXTURE0;
			if( currentAppearance )
			{
				for ( int t = 0 ; t < texcoords.size() ; t ++ )
				{
					TextureUnitState* tu = currentAppearance->getTextureUnitState(t);
					if(!tu)
						continue;
					sbool bEnable = tu->getTexCoordGeneration()->bEnable;
					if ( texcoords[t] && !bEnable)
					{
						if( GL_ARB_multitexture)
							glClientActiveTexture(texId);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(texCoordSize,GL_FLOAT,0,texcoords[t]);
						glActiveTexture(texId);
						glDisable(GL_TEXTURE_GEN_S);
						glDisable(GL_TEXTURE_GEN_T);
						if ( texCoordSize == 3 )
							glDisable(GL_TEXTURE_GEN_R);
						if ( texCoordSize == 4 )
							glDisable(GL_TEXTURE_GEN_Q);
					}
					texId++;
				}
			}

			switch ( type )
			{
			case TRIANGLES:
				glDrawArrays(GL_TRIANGLES , 0 , vertexCount);
				break;

			case QUADS:
				glDrawArrays(GL_QUADS , 0 , vertexCount);
				break;
			}

			//Disable Client States
			glDisableClientState(GL_VERTEX_ARRAY);
			if(colors)
				glDisableClientState(GL_COLOR_ARRAY);
			if(normals) 
				glDisableClientState(GL_NORMAL_ARRAY);

			if( shaderVertexAttributes.size() > 0 )
			{
				for( unsigned v=0; v<shaderVertexAttributes.size(); v++ )
				{
					GLint location = shaderVertexAttributes[v]->m_Location;
					if( location != -1 )
						glDisableVertexAttribArrayARB(location);
				}
			}

			texId = GL_TEXTURE0;
			for ( int t = 0 ; t < texcoords.size() ; t ++ )
			{
				if( GL_ARB_multitexture)
					glClientActiveTexture(texId);
				if ( texcoords[t] )
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				texId++;
			}
		}

		void OglRenderer::addLight(LightData &lightData)
		{
			if ( LightCount > 7 )
				return;

			LightCount++;
			short lightid = GL_LIGHT0 + LightCount;
			float val[4];

			switch(lightData.type)
			{
			case POINTLIGHT:
				val[0] = lightData.position.x;
				val[1] = lightData.position.y;
				val[2] = lightData.position.z;
				val[3] = 1.0f; // 1.0f for positional light
				glLightfv(lightid, GL_POSITION, val);

				//glLightf(lightid, GL_SPOT_EXPONENT, 0.0f);
				//glLightf(lightid, GL_SPOT_CUTOFF, 180.0f);
				break;

			case SPOTLIGHT:
				val[0] = lightData.position.x;
				val[1] = lightData.position.y;
				val[2] = lightData.position.z;
				val[3] = 1.0f; // 1.0f for positional light
				glLightfv(lightid, GL_POSITION, val);

				glLightf(lightid, GL_SPOT_EXPONENT, lightData.concentration);
				glLightf(lightid, GL_SPOT_CUTOFF, lightData.spotAngle);
				break;

			case DIRECTIONALLIGHT:
				val[0] = -lightData.direction.x;
				val[1] = -lightData.direction.y;
				val[2] = -lightData.direction.z;
				val[3] = 0.0f; // 0.0f for directional light
				glLightfv(lightid, GL_POSITION, val);

				//glLightf(lightid, GL_SPOT_EXPONENT, 0.0f);
				//glLightf(lightid, GL_SPOT_CUTOFF, 180.0f);
				break;
			}
			val[0] = lightData.ambient.x;
			val[1] = lightData.ambient.y;
			val[2] = lightData.ambient.z;
			val[3] = lightData.ambient.w;
			glLightfv(lightid , GL_AMBIENT , val);

			val[0] = lightData.diffuse.x;
			val[1] = lightData.diffuse.y;
			val[2] = lightData.diffuse.z;
			val[3] = lightData.diffuse.w;
			glLightfv(lightid , GL_DIFFUSE , val);

			val[0] = lightData.specular.x;
			val[1] = lightData.specular.y;
			val[2] = lightData.specular.z;
			val[3] = lightData.specular.w;
			glLightfv(lightid , GL_SPECULAR , val);

			// set attenuation
			glLightf(lightid, GL_CONSTANT_ATTENUATION, lightData.attenuation.x);
			glLightf(lightid, GL_LINEAR_ATTENUATION, lightData.attenuation.y);
			glLightf(lightid, GL_QUADRATIC_ATTENUATION, lightData.attenuation.z);

			glEnable(lightid);
		}


		void OglRenderer::setTransform(Matrix4f &matrix , TANSFORMATION_STATE transform_state )
		{
			switch( transform_state )
			{
			case WORLD:
			case VIEW:
				glMatrixMode(GL_MODELVIEW);
				matrix.pointer(data);
				glLoadIdentity();
				glLoadMatrixf(data);
				break;
			case PROJECTION:
				glMatrixMode(GL_PROJECTION);
				matrix.pointer(data);
				glLoadIdentity();
				glLoadMatrixf(data);
				break;
			case TEXTURE:
				glMatrixMode(GL_TEXTURE);
				matrix.pointer(data);
				glLoadIdentity();
				glLoadMatrixf(data);
				break;
			}
		}


		void OglRenderer::startScene(Color4& color)
		{
			glClearColor(color.x , color.y , color.z ,  color.w);
			glClearDepth(1.0);
			clearBuffers(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT );
			previousAppearance = 0;
			LightCount = -1;
		}

		void OglRenderer::startScene(Color4& color, int bitSet)
		{
			glClearColor(color.x , color.y , color.z , color.w);
			clearBuffers(bitSet);
			previousAppearance = 0;
			LightCount = -1;
		}

		void OglRenderer::clearBuffers(su32 bufferBit)
		{
			int bitMask = 0;
			if( bufferBit & COLOR_BUFFER_BIT)
				bitMask = GL_COLOR_BUFFER_BIT;
			if( bufferBit & DEPTH_BUFFER_BIT)
				bitMask |= GL_DEPTH_BUFFER_BIT;
			if( bufferBit & STENCIL_BUFFER_BIT)
				bitMask |= GL_STENCIL_BUFFER_BIT;
			if( bitMask )
				glClear( bitMask );
		}

		void OglRenderer::endScene()
		{
			//SwapBuffers(deviceContext);
		}

		OglRenderer::OglRenderer(DisplayParams& displayParams)
		//WindowID( static_cast<HWND>(displayParams.WindowID) )
			//deviceContext(0) 
		//, OpenglContext(0), LightCount(-1)
		{
			initRenderer(displayParams);
		}

		void OglRenderer::initRenderer(DisplayParams& displayParams)
		{
#ifdef WIN32
			// Set up ixel format descriptor with desired parameters
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
				1,				// Version Number
				PFD_DRAW_TO_WINDOW |		// Format Must Support Window
				PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,		// Must Support Double Buffering
				PFD_TYPE_RGBA,			// Request An RGBA Format
				displayParams.Bits,				// Select Our Color Depth
				0, 0, 0, 0, 0, 0,		// Color Bits Ignored
				0,				// No Alpha Buffer
				0,				// Shift Bit Ignored
				0,				// No Accumulation Buffer
				0, 0, 0, 0,			// Accumulation Bits Ignored
				24,				// Z-Buffer (Depth Buffer)
				displayParams.Stencilbuffer ? 1 : 0,	// Stencil Buffer Depth
				0,				// No Auxiliary Buffer
				PFD_MAIN_PLANE,			// Main Drawing Layer
				0,				// Reserved
				0, 0, 0				// Layer Masks Ignored
			};

			GLuint PixelFormat;

			// get hdc
			deviceContext = GetDC(WindowID);

			if (!deviceContext)
			{
				//os::Printer::log("Cannot create a GL device context.", ELL_ERROR);
				//return false;
			}

			// search for pixel format the simple way
			{
				for (su32 i=0; i<5; ++i)
				{
					if (i == 1)
					{
						if (displayParams.Stencilbuffer)
						{
							displayParams.Stencilbuffer = false;
							pfd.cStencilBits = 0;
						}
						else
							continue;
					}
					else
						if (i == 2)
						{
							pfd.cDepthBits = 24;
						}
						if (i == 3)
						{
							if (displayParams.Bits!=16)
								pfd.cDepthBits = 16;
							else
								continue;
						}
						else
							// choose pixelformat
							PixelFormat = ChoosePixelFormat(deviceContext, &pfd);
						if (PixelFormat)
							break;
				}
			}

			// set pixel format
			if (!SetPixelFormat(deviceContext, PixelFormat, &pfd))
			{
			}

			// create rendering context
			OpenglContext = wglCreateContext(deviceContext);
			if (!OpenglContext)
			{
			}

			// activate rendering context
			if (!wglMakeCurrent(deviceContext, OpenglContext))
			{
				//os::Printer::log("Cannot activate GL rendering context", ELL_ERROR);
				wglDeleteContext(OpenglContext);
				//return false;
			}
#endif
			int *maxTextureUnits = new int[1];
			glGetIntegerv(GL_MAX_TEXTURE_UNITS, maxTextureUnits);
			int nbTextureUnits = maxTextureUnits[0];

			Appearance::MAX_TEXTURE_UNITS = nbTextureUnits;


			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			// Reset The Current Viewport
			setViewPort(0, 0, displayParams.WindowWidth , displayParams.WindowHeight);

			glClearDepth(1.0);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_DEPTH_TEST);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_NORMALIZE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK , GL_FILL );

			previousAppearance = new Appearance();
			Material& mat = previousAppearance->getMaterial();
			TransparencyAttributes& transatt = previousAppearance->getTransparencyAttributes();

			mat.ambient.set(0.1f,0.1f,0.1f,1.0f);
			mat.diffuse.set(0.7f,0.7f,0.7f,1.0f);
			mat.specular.set(0.7f,0.7f,0.7f,1.0f);
			mat.emissive.set(0.0f,0.0f,0.0f,1.0f);
			mat.shininess = 40.0f;
			mat._mLightingEnabled = true;

			glEnable(GL_COLOR_MATERIAL);

			float* val = new float[4];
			val[0] = mat.ambient.x ;val[1] = mat.ambient.y ;val[2] = mat.ambient.z ;val[3] = mat.ambient.w;
			glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT  ,val );
			val[0] = mat.diffuse.x ;val[1] = mat.diffuse.y ;val[2] = mat.diffuse.z ;val[3] = mat.diffuse.w;
			glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE  ,val );
			val[0] = mat.specular.x ;val[1] = mat.specular.y ;val[2] = mat.specular.z ;val[3] = mat.specular.w;
			glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR  ,val );
			val[0] = mat.emissive.x ;val[1] = mat.emissive.y ;val[2] = mat.emissive.z ;val[3] = mat.emissive.w;
			glMaterialfv(GL_FRONT_AND_BACK , GL_EMISSION  ,val );
			val[0] = mat.shininess;
			glMaterialfv(GL_FRONT_AND_BACK , GL_SHININESS , val);

			glEnable(GL_LIGHTING);

			transatt.mode = ETM_NONE;
			transatt.srcBlendFactor = EBF_BLEND_ONE;
			transatt.dstBlendFactor = EBF_BLEND_ZERO;
			transatt.transparency = 0.0f;

			mat.colorTarget = ECT_DIFFUSE_AND_AMBIENT;

#ifdef WIN32
			GLenum err = glewInit();

			if (GLEW_OK != err)
			{
				printf("Error: %s\n", glewGetErrorString(err));
			}
#endif
			
			for ( int i = 0 ; i < 8 ; i++ )
				bEnableOglTexCoordgeneration[i] = false;
		}

		Renderer*  OglRenderer::getSingleton(DisplayParams& displayParams)
		{
			if ( !oglRenderer )
			{
				oglRenderer = new OglRenderer(displayParams);
			}

			return oglRenderer;
		}


		void OglRenderer::DrawLine(Point3& start , Point3& end , Color3& color )
		{
			glLineWidth(3.0f);
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			glColor3f(color.x, color.y , color.z );
			glVertex3f(start.x, start.y, start.z);
			glVertex3f(end.x, end.y, end.z);
			glEnd();
			glColor3f(1.0f, 1.0f , 1.0f);
			glEnable(GL_LIGHTING);
			glLineWidth(1.0f);
		}

		void OglRenderer::DrawRect(Rectangle& rect, Color3& color)
		{
			setRenderState(currentAppearance);
			int halfw = rect.width * 0.5f;
			int halfh = rect.height * 0.5f;
			glColor3f(color.x, color.y , color.z );
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(rect.x - halfw , rect.y - halfh);
			glTexCoord2f(0.0f,10.0f);
			glVertex2f(rect.x - halfw , rect.y + halfh);
			glTexCoord2f(10.0f,10.0f);
			glVertex2f(rect.x + halfw , rect.y + halfh);
			glTexCoord2f(10.0f,0.0f);
			glVertex2f(rect.x + halfw , rect.y - halfh);
			glEnd();

			glColor3f(1.0f, 1.0f , 1.0f );
		}

		void OglRenderer::DrawPoint(Point3& point , Color3& color )
		{
			glPointSize(3.0f);
			glDisable(GL_LIGHTING);
			glBegin(GL_POINTS);
			glColor3f(color.x, color.y , color.z );
			glVertex3f(point.x, point.y, point.z);
			glEnd();
			glColor3f(1.0f, 1.0f , 1.0f);
			glEnable(GL_LIGHTING);
		}

		void OglRenderer::DrawBox(AABB& aabb , Color3& color)
		{
			DrawLine(aabb.getEdge(5), aabb.getEdge(1), color);
			DrawLine(aabb.getEdge(1), aabb.getEdge(3), color);
			DrawLine(aabb.getEdge(3), aabb.getEdge(7), color);
			DrawLine(aabb.getEdge(7), aabb.getEdge(5), color);
			DrawLine(aabb.getEdge(0), aabb.getEdge(2), color);
			DrawLine(aabb.getEdge(2), aabb.getEdge(6), color);
			DrawLine(aabb.getEdge(6), aabb.getEdge(4), color);
			DrawLine(aabb.getEdge(4), aabb.getEdge(0), color);
			DrawLine(aabb.getEdge(1), aabb.getEdge(0), color);
			DrawLine(aabb.getEdge(3), aabb.getEdge(2), color);
			DrawLine(aabb.getEdge(7), aabb.getEdge(6), color);
			DrawLine(aabb.getEdge(5), aabb.getEdge(4), color);
		}

		void OglRenderer::setViewPort(sf32 left , sf32 top , sf32 width , sf32 height)
		{
			ViewPort.set(left , top , width , height);
			glViewport(left , top , width , height);
		}

		Rectangle& OglRenderer::getViewPort()
		{
			return ViewPort;
		}

		void OglRenderer::resize(sf32 width, sf32 height)
		{
			//setViewPort(0 , 0 , width,  height);
		}

		void OglRenderer::setAppearance(Appearance* appearance)
		{
			currentAppearance = appearance;
		}

		void OglRenderer::setRenderState(Appearance* appearance)
		{
			if ( !appearance )
				return;

			if ( !previousAppearance && previousAppearance == currentAppearance 
				&& previousAppearance->getTextureUnitState(0) == currentAppearance->getTextureUnitState(0)
				&& !currentAppearance->isDirty() )
				return;
			
			/*if ( previousAppearance == currentAppearance)
			{
				if ( previousAppearance )
				{
					if(previousAppearance->getTextureUnitState(0) == currentAppearance->getTextureUnitState(0))
					 return;
				}
			}*/

			for ( int i = 0 ; i < 8 ; i++ )
				bEnableOglTexCoordgeneration[i] = false;

			TransparencyAttributes* prevTransatt = 0;
			if ( previousAppearance )
				prevTransatt = &previousAppearance->getTransparencyAttributes();
			TransparencyAttributes& currTransatt = appearance->getTransparencyAttributes();

			Material* prevMat = 0;
			if ( previousAppearance )
				prevMat = &previousAppearance->getMaterial();
			Material& currMat = appearance->getMaterial();

			// TransparencyAttributes states
			/*
			*/

			if ( !prevTransatt ||
				(prevTransatt->srcBlendFactor != currTransatt.srcBlendFactor) ||
				(prevTransatt->dstBlendFactor != currTransatt.dstBlendFactor) )
			{
				glBlendFunc( currTransatt.srcBlendFactor , currTransatt.dstBlendFactor );
			}
			if ( !prevTransatt || prevTransatt->mode != currTransatt.mode  )
			{
				if ( currTransatt.mode == ETM_BLEND )
					glEnable(GL_BLEND);
				if ( currTransatt.mode == ETM_NONE )
					glDisable(GL_BLEND);
			}

			// Material states

			float val[] = {0.0f , 0.0f , 0.0f , 0.0f};
			if ( !prevMat || prevMat->colorTarget != currMat.colorTarget || prevMat->useColorMaterial != currMat.useColorMaterial )
			{
				if ( currMat.colorTarget != ECT_NONE && currMat.useColorMaterial )
				{
					glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT_AND_BACK , currMat.colorTarget);
				}
				else
				{
					glDisable(GL_COLOR_MATERIAL);
				}
			}


			if ( !prevMat || (prevMat->ambient != currMat.ambient) ||
				(prevMat->diffuse != currMat.diffuse) ||
				(prevMat->specular != currMat.specular) || 
				(prevMat->emissive != currMat.emissive) ) 
			{

				if ( (currMat.colorTarget != ECT_AMBIENT) ||  (currMat.colorTarget != ECT_DIFFUSE_AND_AMBIENT)  )
				{
					val[0] = currMat.ambient.x ; val[1] = currMat.ambient.y ;
					val[2] = currMat.ambient.z ; val[3] = currMat.ambient.w;
					glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT  ,val );
				}
				if ( (currMat.colorTarget != ECT_DIFFUSE) || (currMat.colorTarget != ECT_DIFFUSE_AND_AMBIENT)  )
				{
					val[0] = currMat.diffuse.x ; val[1] = currMat.diffuse.y ;
					val[2] = currMat.diffuse.z ;
					if ( currTransatt.transparency > 0.0f )
						val[3] = currMat.diffuse.w * (1.0f - currTransatt.transparency);
					glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE  ,val );
				}
				if ( !currMat.useColorMaterial || ( (currMat.shininess != 0.0f) && (currMat.colorTarget!= ECT_SPECULAR) ) )
				{
					val[0] = currMat.specular.x ; val[1] = currMat.specular.y ;
					val[2] = currMat.specular.z ; val[3] = currMat.specular.w;
					glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR  ,val );
				}
				if ( (currMat.colorTarget!= ECT_EMMISSIVE) || !currMat.useColorMaterial )
				{
					val[0] = currMat.emissive.x ; val[1] = currMat.emissive.y ;
					val[2] = currMat.emissive.z ; val[3] = currMat.emissive.w;
					glMaterialfv(GL_FRONT_AND_BACK , GL_EMISSION  ,val );
				}

			}

			if ( !prevMat || prevMat->shininess != currMat.shininess )
			{
				val[0] = currMat.shininess;
				glMaterialfv(GL_FRONT_AND_BACK , GL_SHININESS  ,val );
			}
			if ( !prevMat || prevMat->_mLightingEnabled != currMat._mLightingEnabled)
			{
				if ( currMat._mLightingEnabled )
				{
					glEnable(GL_LIGHTING);
				}
				else
					glDisable(GL_LIGHTING);
			}

			// PolygonAttributes states

			PolygonAttributes* prevPatt = 0;
			if ( previousAppearance )
				prevPatt = &previousAppearance->getPolygonAttributes();
			PolygonAttributes& currPatt = appearance->getPolygonAttributes();

			//if ( !prevPatt || prevPatt->cullFace != currPatt.cullFace )
			{
				switch(currPatt.cullFace)
				{
				case ECF_FRONT:
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
					break;
				case ECF_BACK: 
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					break;
				case ECF_NONE:
					glDisable(GL_CULL_FACE);
					break;
				default:
					break;
				}
			}
			if ( !prevPatt || prevPatt->polygonMode != currPatt.polygonMode )
			{
				switch(currPatt.polygonMode)
				{
				case EPM_FILL:
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;
				case EPM_WIRE: 
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					bLineAttributesRequired = true;
					break;
				case EPM_POINT:
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					bPointAttributesRequired = true;
				default:
					break;
				}
			}
			/* Line and Point Attributes are Enabled only when there is a need.
			   that is only if either the geometry to render is line/points or the drawing mode is line/points in PolygonAttributes.
			*/
			// PointAttributeStates 
			PointAttributes* prevPointatt = 0;
			if ( previousAppearance )
				prevPointatt = &previousAppearance->getPointAttributes();
			PointAttributes& currPointatt = appearance->getPointAttributes();

			if ( (!prevPointatt || prevPointatt->size != currPointatt.size
				|| prevPointatt->antiAliased != currPointatt.antiAliased ) && bPointAttributesRequired)
			{
				glPointSize(currPointatt.size);
				/*if ( currPointatt.antiAliased && false)
				{
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_BLEND);
					glEnable(GL_POINT_SMOOTH); 
					glHint(GL_POINT_SMOOTH,GL_NICEST);
				}
				else
				{
					glDisable(GL_BLEND);
				}*/
				if(currPointatt.m_bPointSizeAttributesEnabled)
				{
					Point3 distanceAttenuation = currPointatt.m_DistanceAttenuation;
					float quadratic[] =  {distanceAttenuation.x , distanceAttenuation.y, distanceAttenuation.z };
					glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, currPointatt.m_ThresholdSize );
					glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, currPointatt.m_MinimumSize );
					glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, currPointatt.m_MaximumSize );
					glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
				}
			}


			// LineAttributeStates 
			LineAttributes* prevLineatt = 0;
			if ( previousAppearance )
				prevLineatt = &previousAppearance->getLineAttributes();
			LineAttributes& currLineatt = appearance->getLineAttributes();

			if ( (!prevLineatt || prevLineatt->width != currLineatt.width
				|| prevLineatt->antiAliased != currLineatt.antiAliased ) && bLineAttributesRequired)
			{
				glLineWidth(currLineatt.width);
				/*if ( currLineatt.antiAliased )
				{
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_BLEND);
					glEnable(GL_LINE_SMOOTH); 
					glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
				}
				else
				{
					glDisable(GL_BLEND);
				}*/
			}

			// RenderingAttributeStates
			RenderingAttributes* prevRatt = 0;
			if ( previousAppearance )
				prevRatt = &previousAppearance->getRenderingAttributes();
			RenderingAttributes& currRatt = appearance->getRenderingAttributes();

			if ( !prevRatt || prevRatt->m_bAlphaTestEnable != currRatt.m_bAlphaTestEnable )
			{
				if ( currRatt.m_bAlphaTestEnable )
				{
					glEnable(GL_ALPHA_TEST);
				}
				else
					glDisable(GL_ALPHA_TEST);
			}

			if ( currRatt.m_bAlphaTestEnable && 
				( !prevRatt || (prevRatt->alphaTestFunc != currRatt.alphaTestFunc)  ||
				(prevRatt->alphaRefValue != currRatt.alphaRefValue) ) )
			{
				glAlphaFunc( currRatt.alphaTestFunc, currRatt.alphaRefValue );
				//glAlphaFunc( GL_GEQUAL, 0.99f );
			}
			// ColoringAttributes states

			ColoringAttributes* prevColAtt = 0;
			if ( previousAppearance )
				prevColAtt = &previousAppearance->getColoringAttributes();
			ColoringAttributes& currColAtt = appearance->getColoringAttributes();

			if ( !prevColAtt || prevColAtt->ColorRedMask != currColAtt.ColorRedMask ||
				prevColAtt->ColorGreenMask != currColAtt.ColorGreenMask ||
				prevColAtt->ColorBlueMask != currColAtt.ColorBlueMask ||
				prevColAtt->ColorAlphaMask != currColAtt.ColorAlphaMask )
			{
				glColorMask ( currColAtt.ColorRedMask , currColAtt.ColorGreenMask , currColAtt.ColorBlueMask , currColAtt.ColorAlphaMask); 
			}

			if ( !prevColAtt || prevColAtt->ShadeModel != currColAtt.ShadeModel )
			{
				if ( currColAtt.ShadeModel == ESM_FLAT )
					glShadeModel(GL_FLAT);
				if ( currColAtt.ShadeModel == ESFN_GOURAUD )
					glShadeModel(GL_SMOOTH);
			}

			// DepthAttributes states
			DepthAttributes* prevDepthAtt = 0;
			if ( previousAppearance )
				prevDepthAtt = &previousAppearance->getDepthAttributes();
			DepthAttributes& currDepthAtt = appearance->getDepthAttributes();

			if ( !prevDepthAtt || prevDepthAtt->DepthTest != currDepthAtt.DepthTest )
			{
				if ( currDepthAtt.DepthTest )
				{
					glEnable(GL_DEPTH_TEST);
				}
				else
					glDisable(GL_DEPTH_TEST);
			}

			if ( !prevDepthAtt || prevDepthAtt->DepthMask != currDepthAtt.DepthMask )
			{
				glDepthMask(currDepthAtt.DepthMask);
			}

			if ( !prevDepthAtt || prevDepthAtt->CompareFunc != currDepthAtt.CompareFunc ) 
			{
				glDepthFunc(currDepthAtt.CompareFunc);
			}
			if ( !prevDepthAtt ||
				(prevDepthAtt->MaxRange != currDepthAtt.MaxRange) ||
				(prevDepthAtt->MinRange != currDepthAtt.MinRange) ) 
			{
				glDepthRange( currDepthAtt.MinRange , currDepthAtt.MaxRange);
			}

			// StencilAttributes states
			StencilAttributes* prevStencilAtt = 0;
			if ( previousAppearance )
				prevStencilAtt = &previousAppearance->getStencilAttributes();
			StencilAttributes& currStencilAtt = appearance->getStencilAttributes();

			if ( !prevStencilAtt || prevStencilAtt->StencilTest != currStencilAtt.StencilTest )
			{
				if ( currStencilAtt.StencilTest )
				{
					glEnable(GL_STENCIL);
				}
				else
					glDisable(GL_STENCIL);
			}

			if ( !prevStencilAtt ||
				prevStencilAtt->CompareFunc != currStencilAtt.CompareFunc ||
				prevStencilAtt->referenceValue != currStencilAtt.referenceValue ||
				prevStencilAtt->compareMask != currStencilAtt.compareMask )
			{
				glStencilFunc(currStencilAtt.CompareFunc, currStencilAtt.referenceValue, currStencilAtt.compareMask);
			}
			if ( !prevStencilAtt ||
				prevStencilAtt->failOp != currStencilAtt.failOp ||
				prevStencilAtt->depthFailOp != currStencilAtt.depthFailOp ||
				prevStencilAtt->depthPassOp != currStencilAtt.depthPassOp )
			{
				glStencilOp(currStencilAtt.failOp, currStencilAtt.depthFailOp, currStencilAtt.depthPassOp);
			}

			if ( !prevStencilAtt || prevStencilAtt->writeMask != currStencilAtt.writeMask )
				glStencilMask(currStencilAtt.writeMask);

			appearance->OnShaderSet();

			// Loop Through Texture Units
			unsigned t = 0;
			static unsigned prevnumTexUnits = 0;
			GLenum texUnitId = GL_TEXTURE0;
			unsigned numTexUnits = appearance->getNumTextureUnits();
			texUnitId = GL_TEXTURE0;
			for ( unsigned t = 0 ; t < MAX_TEX_UNITS ; t++ )
			{
				if( GL_ARB_multitexture)
					glActiveTexture(texUnitId);
				texUnitId++;
				TextureUnitState* currTexUnit = appearance->getTextureUnitState(t);
				TextureUnitState* prevTexUnit = 0;
				if ( previousAppearance )
					prevTexUnit = previousAppearance->getTextureUnitState(t);
				Texture* currTexture = 0;
				Texture* prevTexture = 0;
				TexCoordGeneration* currTexCoordGen = 0;
				TexCoordGeneration* prevTexCoordGen = 0;
				TextureAttributes* currTexAtt = 0;
				TextureAttributes* prevTexAtt = 0;

				if ( !currTexUnit && !prevTexUnit )
				{
					setTextureObject( currTexture );
				}

				if ( currTexUnit != prevTexUnit )
				{
					if(currTexUnit)
					{
						currTexture = currTexUnit->getTexture();
						currTexAtt  = currTexUnit->getTextureAttributes();
						currTexCoordGen = currTexUnit->getTexCoordGeneration();
					}
					if(prevTexUnit)
					{
						prevTexture = prevTexUnit->getTexture();
						prevTexAtt  = prevTexUnit->getTextureAttributes();
						prevTexCoordGen = prevTexUnit->getTexCoordGeneration();
					}

					if ( currTexture != prevTexture )
						setTextureObject( currTexture ); 

					if ( currTexCoordGen != prevTexCoordGen )
					{
						if ( currTexCoordGen )
						{
							bEnableOglTexCoordgeneration[t] = currTexCoordGen->bEnable;

							if ( !prevTexCoordGen || *currTexCoordGen != *prevTexCoordGen )
							{
								switch( currTexCoordGen->texGenMode )
								{
								case ETGM_OBJ_LINEAR:
									glTexGeni( GL_S , GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
									glTexGeni( GL_T , GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
									if ( currTexCoordGen->texComponentFormat == ETCF_3 )
										glTexGeni( GL_R , GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
									if ( currTexCoordGen->texComponentFormat == ETCF_4 )
									{
										glTexGeni( GL_R , GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
										glTexGeni( GL_Q , GL_TEXTURE_GEN_MODE , GL_OBJECT_LINEAR);
									}
									break;
								case ETGM_EYE_LINEAR:
									glTexGeni( GL_S , GL_TEXTURE_GEN_MODE , GL_EYE_LINEAR);
									glTexGeni( GL_T , GL_TEXTURE_GEN_MODE , GL_EYE_LINEAR);
									if ( currTexCoordGen->texComponentFormat == ETCF_3 )
										glTexGeni( GL_R , GL_TEXTURE_GEN_MODE , GL_EYE_LINEAR);
									if ( currTexCoordGen->texComponentFormat == ETCF_4 )
									{
										glTexGeni( GL_R , GL_TEXTURE_GEN_MODE , GL_EYE_LINEAR);
										glTexGeni( GL_Q , GL_TEXTURE_GEN_MODE , GL_EYE_LINEAR);							
									}
									break;
								case ETGM_SPHERE_MAP:
									glTexGenf( GL_S , GL_TEXTURE_GEN_MODE , GL_SPHERE_MAP  );
									glTexGenf( GL_T , GL_TEXTURE_GEN_MODE , GL_SPHERE_MAP  );
									break;
								case ETGM_REFLECTION_MAP:
									glTexGenf( GL_S , GL_TEXTURE_GEN_MODE , GL_REFLECTION_MAP );
									glTexGenf( GL_T , GL_TEXTURE_GEN_MODE , GL_REFLECTION_MAP );
									glTexGenf( GL_R , GL_TEXTURE_GEN_MODE , GL_REFLECTION_MAP );
									break;
								case ETGM_NORMAL_MAP:
									glTexGenf( GL_S , GL_TEXTURE_GEN_MODE , GL_NORMAL_MAP );
									glTexGenf( GL_T , GL_TEXTURE_GEN_MODE , GL_NORMAL_MAP );
									glTexGenf( GL_R , GL_TEXTURE_GEN_MODE , GL_NORMAL_MAP );
									break;
								default:
									break;
								}
							}
						}
					}
					if ( currTexAtt != prevTexAtt )
					{
						if ( currTexAtt )
						{
							if ( !prevTexAtt || currTexAtt->texture_transform != prevTexAtt->texture_transform )
								setTransform(currTexAtt->texture_transform , TEXTURE );

							if ( !prevTexAtt || currTexAtt->texture_mode != prevTexAtt->texture_mode )
							{
								switch( currTexAtt->texture_mode )
								{
								case ETEXM_MODULATE:
									glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
									break;
								case ETEXM_REPLACE:
									glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
									break;
								case ETEXM_DECAL:
									glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
									break;
								case ETEXM_BLEND:
									glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
									break;
								case ETEXM_COMBINE:
									glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE); 
									setTextureCobineAttributes(currTexAtt, texUnitId);
									break;
								default:
									printf("Texture Mode Not Supported by this driver\n");
									break;
								}
							}
						}
					}
				}
			}
			

			prevnumTexUnits = numTexUnits;
			previousAppearance = appearance;
		}

		su32 OglRenderer::createTextureObject()
		{
			su32 textureObject;
			glGenTextures(1 , &textureObject );
			return textureObject;
		}
	
		void OglRenderer::setTextureObject(Texture* texture)
		{
			if ( !texture )
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_TEXTURE_3D);
				glDisable(GL_TEXTURE_CUBE_MAP);
				return;
			}

			if ( texture->getTextureType() == ETT_TEXTURE_2D || texture->getTextureType() == ETT_TEXTURE_POINT_SPRITE)
				glBindTexture(GL_TEXTURE_2D , *(texture->getTextureName()) );
			if ( texture->getTextureType() == ETT_TEXTURE_3D )
				glBindTexture(GL_TEXTURE_3D , *(texture->getTextureName()) );
			if ( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
				glBindTexture(GL_TEXTURE_CUBE_MAP , *(texture->getTextureName()) );

			if ( texture->getTextureType() == ETT_TEXTURE_2D )
			{
				glDisable(GL_TEXTURE_CUBE_MAP);
				glDisable(GL_POINT_SPRITE_ARB);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				//if( texComponent == 3 )
					glEnable(GL_TEXTURE_GEN_R);
				//if( texComponent == 4 )
					glEnable(GL_TEXTURE_GEN_Q);
				//glDisable(GL_TEXTURE_GEN_R);
			}
			if ( texture->getTextureType() == ETT_TEXTURE_POINT_SPRITE )
			{
				glDisable(GL_TEXTURE_CUBE_MAP);
				glEnable(GL_POINT_SPRITE_ARB);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);
				glEnable(GL_TEXTURE_GEN_Q);
				//glDisable(GL_TEXTURE_GEN_R);
			}
			if ( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_POINT_SPRITE_ARB);
				glEnable(GL_TEXTURE_CUBE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);
			}
		}

		void OglRenderer::updateTexture(Rectangle& rect, const void* data, Texture* texture)
		{
			if ( !texture )
				return;

			GLenum target = GL_TEXTURE_2D;
			if( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
				target = GL_TEXTURE_CUBE_MAP_ARB;
			setTextureObject(texture);
			glTexSubImage2D(target, 0, rect.x, rect.y, rect.width, rect.height, GL_RGBA, GL_UNSIGNED_BYTE, data );
		}

		void OglRenderer::copyBufferToTexture(Rectangle& rect, Texture* texture)
		{
			Texture* tex = 0;
			static bool flag = true;
			// Figure out the Texture Format
			int textureFormat = GL_RGB;
			switch( texture->getFormat() )
			{
			case ETF_RGBA :
				textureFormat = GL_RGBA;
				break;
			case ETF_DEPTH:
				textureFormat = GL_DEPTH_COMPONENT;
				break;
			default:
				break;
			}
			if( texture )
			{
				setTextureObject(texture);
				if(flag)
				{
					glCopyTexImage2D(GL_TEXTURE_2D ,0 ,textureFormat ,rect.x ,rect.y ,rect.width ,rect.height ,0 );
					//flag = false;
				}
				//if(!flag)
				//	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,rect.x,rect.y,rect.width,rect.height);
				setTextureObject(tex);
			}
		}

		void OglRenderer::copyBufferToTextureCubeMap(Rectangle& rect, Texture* texture, CUBE_MAP_FACE face)
		{
			Texture* tex = 0;
			static bool flag = true;
			int textureFormat = GL_RGB;
			switch( texture->getFormat() )
			{
			case ETF_RGBA :
				textureFormat = GL_RGBA;
				break;
			case ETF_DEPTH:
				textureFormat = GL_DEPTH_COMPONENT;
				break;
			default:
				break;
			}
			if( texture )
			{
				setTextureObject(texture);
				GLuint cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
				switch( face )
				{
				case ECMF_POSX:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
					break;
				case ECMF_NEGX:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
					break;
				case ECMF_POSY:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
					break;
				case ECMF_NEGY:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
					break;
				case ECMF_POSZ:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
					break;
				case ECMF_NEGZ:
					cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
					break;
				default:
					printf("FrameBufferObjectTexture_CubeMap::onSet::Illegal Cube Map Face\n");
					break;
				}
				//if(flag)
				{
					glCopyTexImage2D(cubeMapFace ,0 ,textureFormat ,rect.x ,rect.y ,rect.width ,rect.height ,0 );
					//flag = false;
				}
				//if(!flag)
				//	glCopyTexSubImage2D(cubeMapFace,0,0,0,0,0,256,256);
				setTextureObject(tex);
			}
		}

		void OglRenderer::setTextureObject(TextureUnitState* textureUnitState)
		{
			Texture* texture = 0;
			if ( !textureUnitState || !(textureUnitState->getTexture()) )
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_TEXTURE_3D);
				glDisable(GL_TEXTURE_CUBE_MAP);
				return;
			}

			texture = textureUnitState->getTexture();

			if ( texture->getTextureType() == ETT_TEXTURE_2D )
				glBindTexture(GL_TEXTURE_2D , *(texture->getTextureName()) );
			if ( texture->getTextureType() == ETT_TEXTURE_3D )
				glBindTexture(GL_TEXTURE_3D , *(texture->getTextureName()) );
			if ( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
				glBindTexture(GL_TEXTURE_CUBE_MAP , *(texture->getTextureName()) );

			if ( texture->getTextureType() == ETT_TEXTURE_2D )
			{
				glDisable(GL_TEXTURE_CUBE_MAP);
				glDisable(GL_TEXTURE_3D);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				if ( texComponent == 3)
					glEnable(GL_TEXTURE_GEN_R);
				if ( texComponent == 4)
					glEnable(GL_TEXTURE_GEN_Q);
			}
			if ( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
			{
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_TEXTURE_CUBE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);
			}
		}


		void OglRenderer::setRenderTargetEnable(FrameBufferObject* fboObject, sbool bEnable)
		{
			if( fboObject && bEnable)
			{
				fboObject->onSet();
			}
			if( fboObject && !bEnable)
			{
				fboObject->unSet();
			}
		}

		FrameBufferObject* OglRenderer::createRenderTargetDepthTexture2D(su32 width, su32 height)
		{
			fboObject = new FrameBufferObjectTexture_Depth(width, height);
			return fboObject;
		}

		FrameBufferObject* OglRenderer::createRenderTargetTexture2D(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer)
		{
			fboObject = new FrameBufferObjectTexture_2D(width, height, depthBuffer, stencilBuffer);
			return fboObject;
		}

		FrameBufferObject* OglRenderer::createRenderTargetTextureCubeMap(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer)
		{
			fboObject = new FrameBufferObjectTexture_CubeMap(width, height, depthBuffer, stencilBuffer);
			return fboObject;
		}

		void OglRenderer::setTextureCobineAttributes(TextureAttributes* texAtt, int texUnit)
		{
			switch( texAtt->texture_Combine_RGB_Mode )
			{
			case ETCM_MODULATE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				break;
			case ETCM_REPLACE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
				break;
			case ETCM_ADD:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
				break;
			case ETCM_ADD_SIGNED:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
				break;
			case ETCM_SUBTRACT:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
				break;
			case ETCM_INTERPOLATE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
				break;
			case ETCM_DOT3:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
				break;
			}

			switch( texAtt->texture_Combine_Alpha_Mode )
			{
			case ETCM_MODULATE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
				break;
			case ETCM_REPLACE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
				break;
			case ETCM_ADD:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);
				break;
			case ETCM_ADD_SIGNED:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
				break;
			case ETCM_SUBTRACT:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SUBTRACT);
				break;
			case ETCM_INTERPOLATE:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);
				break;
			case ETCM_DOT3:
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_DOT3_RGBA);
				break;
			}

			GLint textureSourceAlpha = GL_SRC0_ALPHA;
			for( int i=0; i<3; i++ )
			{
				switch( texAtt->texture_Combine_Alpha_Source[i] )
				{
				case ETCS_COMBINE_OBJECT_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceAlpha, GL_PRIMARY_COLOR);
					break;
				case ETCS_COMBINE_TEXTURE_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceAlpha, texUnit); 
					break;
				case ETCS_COMBINE_CONSTANT_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceAlpha, GL_CONSTANT);
					break;
				case ETCS_COMBINE_PREVIOUS_TEXTURE_UNIT_STATE:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceAlpha, GL_PREVIOUS); 
					break;
				}
				textureSourceAlpha++;
			}

			GLint textureSourceRGB  = GL_SRC0_RGB;
			for( int i=0; i<3; i++ )
			{
				switch( texAtt->texture_Combine_RGB_Source[i] )
				{
				case ETCS_COMBINE_OBJECT_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceRGB, GL_PRIMARY_COLOR);
					break;
				case ETCS_COMBINE_TEXTURE_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceRGB, GL_TEXTURE);
					break;
				case ETCS_COMBINE_CONSTANT_COLOR:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceRGB, GL_CONSTANT); 
					break;
				case ETCS_COMBINE_PREVIOUS_TEXTURE_UNIT_STATE:
					glTexEnvi(GL_TEXTURE_ENV, textureSourceRGB, GL_PREVIOUS);
					break;
				}
				textureSourceRGB++;
			}

			GLint textureOperandRGB = GL_OPERAND0_RGB;

			for( int i=0; i<3; i++ )
			{
				switch( texAtt->texture_Combine_RGB_Operand[i] )
				{
				case ETC_RGBO_COMBINE_SRC_COLOR:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_SRC_COLOR);
					break;
				case ETC_RGBO_COMBINE_ONE_MINUS_SRC_COLOR:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_ONE_MINUS_SRC_COLOR); 
					break;
				case ETC_RGBO_COMBINE_SRC_ALPHA:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_SRC_ALPHA); 
					break;
				case ETC_RGBO_COMBINE_ONE_MINUS_SRC_ALPHA:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_ONE_MINUS_SRC_ALPHA); 
					break;
				}
				textureOperandRGB++;
			}

			GLint textureOperandAlpha = GL_OPERAND0_ALPHA;

			for( int i=0; i<3; i++ )
			{
				switch( texAtt->texture_Combine_Alpha_Operand[i] )
				{
				case ETC_ALPHAO_COMBINE_SRC_ALPHA:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_SRC_ALPHA);
					break;
				case ETC_ALPHAO_COMBINE_ONE_MINUS_SRC_ALPHA:
					glTexEnvf(GL_TEXTURE_ENV, textureOperandRGB, GL_ONE_MINUS_SRC_ALPHA); 
					break;
				}
				textureOperandAlpha++;
			}

			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, texAtt->texture_Combine_RGB_Scale);
			glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE, texAtt->texture_Combine_Alpha_Scale);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texAtt->texture_Constant_Color);
		}

		FrameBufferObject::FrameBufferObject(su32 width, su32 height):fboWidth(width),fboHeight(height)
		{		
			m_BufferAttachmentType = EBAT_COLOR;
			m_ColorAttachmentPoint = ECAP0;
			m_bMRT_Enabled = false;
			fboTexture = 0;
		}

		Texture* FrameBufferObject::getFboTexture()
		{
			return fboTexture;
		}

		void FrameBufferObject::attachTexture(Texture* texture, COLOR_ATTACHMENT_POINT colorAttchment)
		{
			fboTexture = texture;
			m_ColorAttachmentPoint = colorAttchment;
			GLint col_att_point = GL_COLOR_ATTACHMENT0_EXT;
			col_att_point += colorAttchment;
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);
			if( m_BufferAttachmentType == EBAT_COLOR)
			    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, col_att_point, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
			if( m_BufferAttachmentType == EBAT_DEPTH)
			    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		void FrameBufferObject::setColorAttachmentPoint(COLOR_ATTACHMENT_POINT colorAttachment)
		{
			m_ColorAttachmentPoint = colorAttachment;
		}
		
		void FrameBufferObject::setBufferAttachmentType(BUFFER_ATTACHMENT_TYPE bufferAttachmentType)
		{
			m_BufferAttachmentType = bufferAttachmentType;
		}

		void FrameBufferObject::attachMRTArray(si32 arrayLength, COLOR_ATTACHMENT_POINT* bufferArray)
		{
			GLenum buffers[4];
			for( int i=0; i<arrayLength; i++ )
				buffers[i] = GL_COLOR_ATTACHMENT0_EXT + bufferArray[i];

			m_bMRT_Enabled = true;
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);
			glDrawBuffers(arrayLength, buffers);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		TextureUnitState* FrameBufferObject::getFboTextureUnitState()
		{
			return fboTextureUnit;
		}

		void FrameBufferObject::getFrameBufferObjectName(unsigned* fboName)
		{
			*fboName = frameBufferName;
		}


		FrameBufferObjectTexture_2D::FrameBufferObjectTexture_2D(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer):FrameBufferObject(width,height)
		{
			//if( GLEW_ARB_framebuffer_object )
			{
				frameBufferName   = 0;
				depthBufferName   = 0;
				stencilBufferName = 0;
				// create a framebuffer object, you need to delete them when program exits.
				glGenFramebuffersEXT(1, &frameBufferName);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);

				// create a renderbuffer object to store depth info
				// NOTE: A depth renderable image should be attached the FBO for depth test.
				// If we don't attach a depth renderable image to the FBO, then
				// the rendering output will be corrupted because of missing depth test.
				// If you also need stencil test for your rendering, then you must
				// attach additional image to the stencil attachement point, too.
				if(depthBuffer)
				{
					glGenRenderbuffersEXT(1, &depthBufferName);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferName);
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, fboWidth, fboHeight);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
				}
				// attach a texture to FBO color attachement point
				//fboTexture = new Texture2D(ETEX_BM_CLAMP_TO_EDGE , ETF_RGB, fboWidth, fboHeight);
				//fboTextureUnit = new TextureUnitState(fboTexture);
				//fboTextureUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
				//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);

				// attach a renderbuffer to depth attachment point
				if( depthBuffer )
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBufferName);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
				m_BufferAttachmentType = EBAT_COLOR;
			}
		}

		void FrameBufferObjectTexture_2D::onSet()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);
			if( !m_bMRT_Enabled )
			{
				glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + m_ColorAttachmentPoint);
			    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + m_ColorAttachmentPoint, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
			}
		}

		void FrameBufferObjectTexture_2D::unSet()
		{
			glDrawBuffer(GL_BACK);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		FrameBufferObjectTexture_CubeMap::FrameBufferObjectTexture_CubeMap(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer):FrameBufferObject(width,height)
		{
			//if( GLEW_ARB_framebuffer_object )
			{
				frameBufferName   = 0;
				depthBufferName   = 0;
				stencilBufferName = 0;
				currentFace       = ECMF_POSX;
				// create a framebuffer object, you need to delete them when program exits.
				glGenFramebuffersEXT(1, &frameBufferName);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);

				// create a renderbuffer object to store depth info
				// NOTE: A depth renderable image should be attached the FBO for depth test.
				// If we don't attach a depth renderable image to the FBO, then
				// the rendering output will be corrupted because of missing depth test.
				// If you also need stencil test for your rendering, then you must
				// attach additional image to the stencil attachement point, too.
				if(depthBuffer)
				{
					glGenRenderbuffersEXT(1, &depthBufferName);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferName);
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, fboWidth, fboHeight);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
				}
				// attach a texture to FBO color attachement point
				fboTexture = new TextureCubeMap(ETEX_BM_CLAMP_TO_EDGE , ETF_RGB, fboWidth, fboHeight);
				fboTextureUnit = new TextureUnitState(fboTexture);
				fboTextureUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, *fboTexture->getTextureName(), 0);

				// attach a renderbuffer to depth attachment point
				if( depthBuffer )
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBufferName);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
				m_BufferAttachmentType = EBAT_COLOR;
			}
		}

		void FrameBufferObjectTexture_CubeMap::onSet()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + m_ColorAttachmentPoint);
			GLuint cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			switch( currentFace )
			{
			case ECMF_POSX:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
				break;
			case ECMF_NEGX:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
				break;
			case ECMF_POSY:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
				break;
			case ECMF_NEGY:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
				break;
			case ECMF_POSZ:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
				break;
			case ECMF_NEGZ:
				cubeMapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
				break;
			default:
				printf("FrameBufferObjectTexture_CubeMap::onSet::Illegal Cube Map Face\n");
				break;
			}
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, cubeMapFace, *fboTexture->getTextureName(), 0);
		}

		void FrameBufferObjectTexture_CubeMap::unSet()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		void FrameBufferObjectTexture_CubeMap::setCurrentFace(CUBE_MAP_FACE currentFace)
		{
			this->currentFace = currentFace;
		}

		FrameBufferObjectTexture_Depth::FrameBufferObjectTexture_Depth(su32 width, su32 height):FrameBufferObject(width,height)
		{
			//if( GLEW_ARB_framebuffer_object )
			{
				frameBufferName   = 0;
				depthBufferName   = 0;
				stencilBufferName = 0;
				// create a framebuffer object, you need to delete them when program exits.
				glGenFramebuffersEXT(1, &frameBufferName);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);

				// create a renderbuffer object to store depth info
				// NOTE: A depth renderable image should be attached the FBO for depth test.
				// If we don't attach a depth renderable image to the FBO, then
				// the rendering output will be corrupted because of missing depth test.
				// If you also need stencil test for your rendering, then you must
				// attach additional image to the stencil attachement point, too.
				// attach a texture to FBO color attachement point
				
				//fboTexture = new Texture2D(ETEX_BM_CLAMP_TO_EDGE , ETF_DEPTH, fboWidth, fboHeight);
				//fboTextureUnit = new TextureUnitState(fboTexture);

				//fboTextureUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;

				// attach a texture to FBO depth attachement point
				//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
				m_BufferAttachmentType = EBAT_DEPTH;
			}
		}

		void FrameBufferObjectTexture_Depth::onSet()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferName);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		void FrameBufferObjectTexture_Depth::unSet()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glDrawBuffer(GL_BACK);
			glReadBuffer(GL_BACK);
		}

	}
}
