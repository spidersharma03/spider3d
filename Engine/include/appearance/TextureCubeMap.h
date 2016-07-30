#ifndef _TEXTURE_CUBE_MAP_H
#define _TEXTURE_CUBE_MAP_H

#include "Texture.h"

namespace spider3d
{
	enum CUBE_MAP_FACE { ECMF_POSX , ECMF_NEGX , ECMF_POSY , ECMF_NEGY , ECMF_POSZ , ECMF_NEGZ };
	
	namespace appearance
	{

		class TextureCubeMap:public Texture
		{
		private:
			Image* posx_image;
			Image* negx_image;
			Image* posy_image;
			Image* negy_image;
			Image* posz_image;
			Image* negz_image;
			vector<Image*> m_vecMipMapPosXImages;
			vector<Image*> m_vecMipMapNegXImages;
			vector<Image*> m_vecMipMapPosYImages;
			vector<Image*> m_vecMipMapNegYImages;
			vector<Image*> m_vecMipMapPosZImages;
			vector<Image*> m_vecMipMapNegZImages;
		public:
			TextureCubeMap(TEXTURE_BOUNDARY_MODE texBoundaryMode, TEXTURE_FORMAT format, unsigned width, unsigned height);
			virtual Image* getImage();
			virtual Image* getImage(si32);
			Image* getImage(int level,CUBE_MAP_FACE face);
			void setImage(int level, CUBE_MAP_FACE face , Image* image);
			void setMipMapImages(CUBE_MAP_FACE face , Image* image);
			virtual ~TextureCubeMap(){};
		};

	}
}
#endif