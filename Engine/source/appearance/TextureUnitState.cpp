#include "appearance/TextureUnitState.h"
#include "appearance/TextureCubeMap.h"
#include "appearance/Image.h"
//#include <GL/glut.h>
#ifdef WIN32
#include <GL/glu.h>
#endif
#ifdef _MACOSX_
#include <OpenGL/OpenGL.h>
#endif

namespace spider3d
{
	namespace appearance
	{
		TextureUnitState::TextureUnitState(Texture* texture):texture(texture)
		{
			// Create Texture Object
			glGenTextures(1 , texture->getTextureName() );
			// Bind Texture Object to Opengl Context
			GLenum target;
			if( texture->getTextureType() == ETT_TEXTURE_2D || texture->getTextureType() == ETT_TEXTURE_POINT_SPRITE )
				target = GL_TEXTURE_2D;
			if( texture->getTextureType() == ETT_TEXTURE_3D )
				target = GL_TEXTURE_3D;
			if( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
				target = GL_TEXTURE_CUBE_MAP;

			glBindTexture(target , *(texture->getTextureName()) );

			// Figure out the Texture Format
			int textureFormat = GL_RGB;
			switch( texture->getFormat() )
			{
			case ETF_RGBA :
				textureFormat = GL_RGBA;
				break;
			case ETF_DEPTH:
				textureFormat = GL_DEPTH_COMPONENT;
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
				glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
				break;
			default:
				break;
			}
			// send image data to opengl
			if ( texture->getTextureType() == ETT_TEXTURE_2D )
			{
				if ( texture->getBoundaryWidth() > 0 )
				{
					glTexImage2D(GL_TEXTURE_2D , 0 , textureFormat , texture->getWidth() + 2*texture->getBoundaryWidth() , texture->getHeight() + 2*texture->getBoundaryWidth(), 0 , GL_RGBA , GL_UNSIGNED_BYTE , texture->getImage()->getData() );
					GLfloat borderColor[4] = {1.0, 1.0, 1.0, 1.0};
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
				}
				else
				{
					void* data = 0;

					for( unsigned i=0; i<texture->getNumMipMaps(); i++ )
					{
						int w,h;
						if(texture->getImage(i))
						{
							data = texture->getImage(i)->getData();
							w = texture->getImage(i)->getWidth();
							h = texture->getImage(i)->getHeight();
						}
						else
						{
							w = texture->getWidth();
							h = texture->getHeight();
						}
						glTexImage2D(GL_TEXTURE_2D , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE ,data  );
					}
				}
			}
			else if ( texture->getTextureType() == ETT_TEXTURE_POINT_SPRITE )
			{
				void* data = 0;
				if(texture->getImage())
					data = texture->getImage()->getData();
				glTexImage2D(GL_TEXTURE_2D , 0 , textureFormat , texture->getWidth() , texture->getHeight() , 0 , textureFormat , GL_UNSIGNED_BYTE ,data  );

				glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE, GL_TRUE);
			}
			else if ( texture->getTextureType() == ETT_TEXTURE_CUBE_MAP )
			{
				TextureCubeMap* texCubeMap = static_cast<TextureCubeMap*>(texture);
				void* imageData = 0;

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_POSX))
					{
						imageData = texCubeMap->getImage(i,ECMF_POSX)->getData();
						w = texCubeMap->getImage(i,ECMF_POSX)->getWidth();
						h = texCubeMap->getImage(i,ECMF_POSX)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_NEGX))
					{
						imageData = texCubeMap->getImage(i,ECMF_NEGX)->getData();
						w = texCubeMap->getImage(i,ECMF_NEGX)->getWidth();
						h = texCubeMap->getImage(i,ECMF_NEGX)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_POSY))
					{
						imageData = texCubeMap->getImage(i,ECMF_POSY)->getData();
						w = texCubeMap->getImage(i,ECMF_POSY)->getWidth();
						h = texCubeMap->getImage(i,ECMF_POSY)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_NEGY))
					{
						imageData = texCubeMap->getImage(i,ECMF_NEGY)->getData();
						w = texCubeMap->getImage(i,ECMF_NEGY)->getWidth();
						h = texCubeMap->getImage(i,ECMF_NEGY)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_POSZ))
					{
						imageData = texCubeMap->getImage(i,ECMF_POSZ)->getData();
						w = texCubeMap->getImage(i,ECMF_POSZ)->getWidth();
						h = texCubeMap->getImage(i,ECMF_POSZ)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

				for( unsigned i=0; i<texCubeMap->getNumMipMaps(); i++ )
				{
					int w,h;
					if(texCubeMap->getImage(i,ECMF_NEGZ))
					{
						imageData = texCubeMap->getImage(i,ECMF_NEGZ)->getData();
						w = texCubeMap->getImage(i,ECMF_NEGZ)->getWidth();
						h = texCubeMap->getImage(i,ECMF_NEGZ)->getHeight();
					}
					else
					{
						w = texCubeMap->getWidth();
						h = texCubeMap->getHeight();
					}
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z , i , textureFormat , w , h , 0 , textureFormat , GL_UNSIGNED_BYTE , imageData );
				}

			}

			//Create TextureAttributes Object
			textureAtt  = new TextureAttributes();
			texCoordGen = new TexCoordGeneration();

			textureAtt->texture_mode = ETEXM_MODULATE;

			glTexParameterf( target, GL_TEXTURE_MIN_FILTER , GL_LINEAR );
			glTexParameterf( target, GL_TEXTURE_MAG_FILTER , GL_LINEAR );
			glTexParameterf( target, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf( target, GL_TEXTURE_WRAP_T, GL_REPEAT );

			if( texture->getMipMapMode() == E_MIPMAP_MODE_MULTI_LEVEL )
			{
				glTexParameterf( target, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR );
				glTexParameterf( target, GL_TEXTURE_MAG_FILTER,
					GL_LINEAR_MIPMAP_LINEAR );
			}
			if( texture->getTextureType() == ETT_TEXTURE_2D )
			{
				texCoordGen->texGenMode = ETGM_EYE_LINEAR;
				glTexGenfv( GL_S , GL_EYE_PLANE , texCoordGen->planeS );
				glTexGenfv( GL_T , GL_EYE_PLANE , texCoordGen->planeT );
				glTexGenfv( GL_R , GL_EYE_PLANE , texCoordGen->planeR );
				glTexGenfv( GL_Q , GL_EYE_PLANE , texCoordGen->planeQ );
			}

			glBindTexture(target , 0 );
		}

		TextureUnitState::TextureUnitState(Texture* texture, TexCoordGeneration* texCoordGen)
		{
		}

		TextureUnitState::TextureUnitState(Texture* texture, TextureAttributes* textureAtt)
		{
		}

		TextureUnitState::TextureUnitState(Texture* texture, TexCoordGeneration* texCoordGen, TextureAttributes* textureAtt)
		{
		}

		TextureAttributes* TextureUnitState::getTextureAttributes()
		{
			return textureAtt;
		}

		Texture* TextureUnitState::getTexture()
		{
			return texture;
		}

		TexCoordGeneration* TextureUnitState::getTexCoordGeneration()
		{
			return texCoordGen;
		}

		TextureUnitState::~TextureUnitState()
		{
			glDeleteTextures(1,texture->getTextureName() );
		}
	}
}