#include "appearance/TextureCubeMap.h"
#include "appearance/Image.h"
#include <stdio.h>

namespace spider3d
{
	namespace appearance
	{
		TextureCubeMap::TextureCubeMap(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height)
			:Texture(texBoundaryMode,format,width,height)
		{
			type = ETT_TEXTURE_CUBE_MAP;
			posx_image = negx_image = 0;
			posy_image = negy_image = 0;
			posz_image = negz_image = 0;
		}

		Image* TextureCubeMap::getImage()
		{
			return 0;
		}

		Image* TextureCubeMap::getImage(si32 level)
		{
			return 0;
		}

		Image* TextureCubeMap::getImage(int level, CUBE_MAP_FACE face)
		{
			switch(face)
			{
			case ECMF_POSX:
				if(m_vecMipMapPosXImages.size() >0 )
					return m_vecMipMapPosXImages[level];
				return 0;
				break;
			case ECMF_NEGX:
				if(m_vecMipMapNegXImages.size() >0 )
					return m_vecMipMapNegXImages[level];
				return 0;
				break;
			case ECMF_POSY:
				if(m_vecMipMapPosYImages.size() >0 )
					return m_vecMipMapPosYImages[level];
				return 0;
				break;
			case ECMF_NEGY:
				if(m_vecMipMapNegYImages.size() >0 )
					return m_vecMipMapNegYImages[level];
				return 0;
				break;
			case ECMF_POSZ:
				if(m_vecMipMapPosZImages.size() >0 )
					return m_vecMipMapPosZImages[level];
				return 0;
				break;
			case ECMF_NEGZ:
				if(m_vecMipMapNegZImages.size() >0 )
					return m_vecMipMapNegZImages[level];
				return 0;
				break;
			default:
				printf("Cube Map Face Not Supported:\n");
				return 0;
				break;
			}
		}

		void TextureCubeMap::setImage( int level, CUBE_MAP_FACE face , Image* image)
		{
			switch(face)
			{
			case ECMF_POSX:
				posx_image = image;
				m_vecMipMapPosXImages.push_back(image);
				m_NumMipMaps = m_vecMipMapPosXImages.size();
				break;
			case ECMF_NEGX:
				negx_image = image;
				m_vecMipMapNegXImages.push_back(image);
				m_NumMipMaps = m_vecMipMapNegXImages.size();
				break;
			case ECMF_POSY:
				posy_image = image;
				m_vecMipMapPosYImages.push_back(image);
				m_NumMipMaps = m_vecMipMapPosYImages.size();
				break;
			case ECMF_NEGY:
				negy_image = image;
				m_vecMipMapNegYImages.push_back(image);
				m_NumMipMaps = m_vecMipMapNegYImages.size();
				break;
			case ECMF_POSZ:
				posz_image = image;
				m_vecMipMapPosZImages.push_back(image);
				m_NumMipMaps = m_vecMipMapPosZImages.size();
				break;
			case ECMF_NEGZ:
				negz_image = image;
				m_vecMipMapNegZImages.push_back(image);
				m_NumMipMaps = m_vecMipMapNegZImages.size();
				break;
			default:
				printf("Cube Map Face Not Supported:\n");
				break;
			}
		}


		void TextureCubeMap::setMipMapImages(CUBE_MAP_FACE face , Image* image)
		{
			int w = image->getWidth();
			int h = image->getHeight();
			int mipMapLevel = 0;
			this->setImage(mipMapLevel, face, image);
			do
			{
				w /= 2;
				h /= 2;
				mipMapLevel++;
				Image * mipMapImage = image->getScaledImage(w,h);
				this->setMipMapMode(E_MIPMAP_MODE_MULTI_LEVEL);
				this->setImage(mipMapLevel, face, mipMapImage);
			}
			while( w !=1 );
		}
	}
}