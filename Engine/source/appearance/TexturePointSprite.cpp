#include "appearance/TexturePointSprite.h"
#include "appearance/Image.h"

namespace spider3d
{
	namespace appearance
	{
		TexturePointSprite::TexturePointSprite( TEXTURE_FORMAT format, unsigned width, unsigned height)
			:Texture(texBoundaryMode,format,width,height)
		{
			type = ETT_TEXTURE_POINT_SPRITE;
			m_ThresholdSize = 1.0f;
			m_MinimumSize = 1.0f;
			m_MaximumSize = 1.0f;
			m_DistanceAttenuation = Point3(1.0f,0.0f,0.0f);
		}


		Image* TexturePointSprite::getImage()
		{
			return image;
		}

		Image* TexturePointSprite::getImage(si32 index)
		{
			return image;
		}

		void TexturePointSprite::setImage(Image* image)
		{
			this->image = image;
		}

		void TexturePointSprite::setThresholdFadePointSize(sf32 size)
		{
			m_ThresholdSize = size;
		}
		
		void TexturePointSprite::setMinumumPointSize(sf32 size)
		{
			m_MinimumSize = size;
		}
		
		void TexturePointSprite::setMaximumPointSize(sf32 size)
		{
			m_MaximumSize = size;
		}
		
		void TexturePointSprite::setDistanceAttenuation(sf32 constant, sf32 linear, sf32 quadratic)
		{
			m_DistanceAttenuation.set(constant,linear,quadratic);
		}
		
		void TexturePointSprite::setDistanceAttenuation(Point3& distanceAttenuation)
		{
			m_DistanceAttenuation.set(distanceAttenuation);
		}
	}
}