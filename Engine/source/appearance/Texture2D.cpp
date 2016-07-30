#include "appearance/Texture2D.h"
#include "appearance/Image.h"

namespace spider3d
{
	namespace appearance
	{
		Texture2D::Texture2D( TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height)
			:Texture(texBoundaryMode,format,width,height)
		{
			type = ETT_TEXTURE_2D;
			boundaryWidth = 0;

			//m_vecMipMapImages.resize(12);
		}

		Texture2D::Texture2D(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height, unsigned boundaryWidth)
			:Texture(texBoundaryMode,format,width,height,boundaryWidth)
		{
			type = ETT_TEXTURE_2D;
			boundaryColor.x = 1.0f;
			boundaryColor.y = 1.0f;
			boundaryColor.z = 1.0f;
			boundaryColor.w = 1.0f;
		}

		Image* Texture2D::getImage()
		{
			return image;
		}

		Image* Texture2D::getImage(si32 level)
		{
			if(m_vecMipMapImages.size() >0 )
				return m_vecMipMapImages[level];
			return 0;
		}

		void Texture2D::setImage(si32 level, Image* image)
		{
			this->image = image;
			m_vecMipMapImages.push_back(image);
			m_NumMipMaps = m_vecMipMapImages.size();
			if ( image->getFormat() == EIF_RGBA )
				format = ETF_RGBA;

			if ( image->getFormat() == EIF_RGB )
				format = ETF_RGB;

			if ( boundaryWidth > 0 )
			{
				this->image->pad ( boundaryWidth , boundaryColor.x*255 , boundaryColor.y*255, boundaryColor.z*255, boundaryColor.w*255);
			}
		}

		void Texture2D::setMipMapImages(Image* image)
		{
			int w = image->getWidth();
			int h = image->getHeight();
			int mipMapLevel = 0;
			this->setImage(mipMapLevel, image);
			do
			{
				w /= 2;
				h /= 2;
				mipMapLevel++;
				Image * mipMapImage = image->getScaledImage(w,h);
				this->setMipMapMode(E_MIPMAP_MODE_MULTI_LEVEL);
				this->setImage(mipMapLevel, mipMapImage);
			}
			while( w !=1 );
		}

	}
}