#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "math/coreMath.h"
#include <vector>

using namespace std;
using namespace spider3d::core;

namespace spider3d
{
	enum TEXTURE_BOUNDARY_MODE { ETEX_BM_CLAMP , ETEX_BM_WRAP, ETEX_BM_CLAMP_TO_EDGE, ETEX_BM_CLAMP_TO_BOUNDARY  }; 
	
	enum TEXTURE_FORMAT { ETF_RGB , ETF_RGBA, ETF_DEPTH};

	enum TEXTURE_TYPE { ETT_TEXTURE_2D, ETT_TEXTURE_POINT_SPRITE, ETT_TEXTURE_3D, ETT_TEXTURE_CUBE_MAP };

	enum TEXTURE_MIN_FILTER { ETMINF_POINT, ETMINF_LINEAR };

	enum TEXTURE_MAG_FILTER { ETMAXF_POINT, ETMAXF_LINEAR };

	enum MIPMAP_MODE { E_MIPMAP_MODE_BASE_LEVEL, E_MIPMAP_MODE_MULTI_LEVEL };

	namespace appearance
	{
		class Image;

		class Texture
		{
		public:
			Texture(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height):texBoundaryMode(texBoundaryMode),format(format),width(width),height(height)
			{
				image = 0;
				m_NumMipMaps = 1;
				m_MipMapMode = E_MIPMAP_MODE_BASE_LEVEL;
			}

			Texture(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height, unsigned boundaryWidth):texBoundaryMode(texBoundaryMode),format(format),width(width),height(height),boundaryWidth(boundaryWidth)
			{
				image = 0;
				m_NumMipMaps = 1;
				m_MipMapMode = E_MIPMAP_MODE_BASE_LEVEL;
			}

			virtual ~Texture()
			{
			}

			TEXTURE_FORMAT getFormat()
			{
				return format;
			}
			
			TEXTURE_BOUNDARY_MODE getBoundaryMode()
			{
				return texBoundaryMode;
			}

			TEXTURE_TYPE getTextureType()
			{
				return type;
			}

			TEXTURE_MIN_FILTER getMinFilter()
			{
				return minFilter;
			}

			TEXTURE_MAG_FILTER getMagFilter()
			{
				return magFilter;
			}

			unsigned getWidth()
			{
				return width;
			}

			unsigned getHeight()
			{
				return height;
			}

			unsigned* getTextureName()
			{
				return &textureName;
			}

			Color4& getBoundaryColor()
			{
				return boundaryColor;
			}

			void setBoundaryColor(Color4& boundaryColor)
			{
				this->boundaryColor = boundaryColor;
			}

			unsigned getBoundaryWidth()
			{
				return boundaryWidth;
			}

			void setMipMapMode(MIPMAP_MODE mipMapMode)
			{
				m_MipMapMode = mipMapMode;
			}

			MIPMAP_MODE getMipMapMode()
			{
				return m_MipMapMode;
			}

			unsigned getNumMipMaps()
			{
				return m_NumMipMaps;
			}

			virtual Image* getImage() = 0;

			virtual Image* getImage(si32) = 0;

		protected:
			TEXTURE_BOUNDARY_MODE texBoundaryMode;
			TEXTURE_FORMAT format;
			TEXTURE_TYPE type;
			TEXTURE_MIN_FILTER minFilter;
			TEXTURE_MAG_FILTER magFilter;
			MIPMAP_MODE m_MipMapMode;
			unsigned width, height;
			unsigned boundaryWidth;
			unsigned textureName;
			Image* image;
			si32 m_NumMipMaps;
			Color4 boundaryColor;
			sbool m_bIsMipMapped;
		};

	}
}


#endif