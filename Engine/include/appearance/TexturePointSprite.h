#ifndef _TEXTURE_POINT_SPRITE_H
#define _TEXTURE_POINT_SPRITE_H

#include "Texture.h"

namespace spider3d
{
	namespace appearance
	{

		class TexturePointSprite:public Texture
		{
		public:
			TexturePointSprite(TEXTURE_FORMAT format, unsigned width, unsigned height);
			virtual Image* getImage();
			virtual Image* getImage(si32);
			void setImage( Image* image);
			void setThresholdFadePointSize(sf32 size);
			void setMinumumPointSize(sf32 size);
			void setMaximumPointSize(sf32 size);
			void setDistanceAttenuation(sf32,sf32,sf32);
			void setDistanceAttenuation(Point3&);
			// inline methods
			inline sf32 getThresholdFadePointSize()
			{
				return m_ThresholdSize;
			}

			inline sf32 getMinimumSize()
			{
				return m_MinimumSize;
			}

			inline sf32 getMaximumSize()
			{
				return m_MaximumSize;
			}

			inline Point3 getDistanceAttenuation()
			{
				return m_DistanceAttenuation;
			}

			virtual ~TexturePointSprite()
			{
				delete image;
			}
		private:
			sf32 m_ThresholdSize;
			sf32 m_MinimumSize;
			sf32 m_MaximumSize;
			Point3 m_DistanceAttenuation;
		};

	}
}
#endif

				/*float quadratic[] =  { 0.01f, 0.01f, 0.01f };
				glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
				glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 5.0f );
				glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, 150.0f );
				glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );*/
