#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include "Texture.h"

namespace spider3d
{
	namespace appearance
	{

		class Texture2D:public Texture
		{
		public:
			Texture2D(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height);
			Texture2D(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height, unsigned boundaryWidth);
			virtual Image* getImage();
			Image* getImage(si32 level);
			void setImage(si32 level, Image* image);
			void setMipMapImages(Image* image);
			virtual ~Texture2D()
			{
				//delete image;
			}
		private:
			vector<Image*> m_vecMipMapImages;
		};

	}
}
#endif