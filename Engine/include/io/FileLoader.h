#ifndef _FILE_LOADER_H
#define _FILE_LOADER_H

#include <string>

using namespace std;

namespace spider3d
{
	namespace appearance
	{
		class Image;
		class Texture;
	}
}

using namespace spider3d::appearance;

namespace spider3d
{
	namespace io
	{
		class TextureLoader
		{
		private:
			static TextureLoader* textureLoader;
			Texture *m_Texture;
			Image *m_Image;
			// BMP Loader
			void loadBmp();
			void loadTga();
			void loadFile();
			void saveFile(const char* fileName);
			char fileName[20];
			string imageFileName;

		public:
			static const int GENERATE_MIP_MAPS = 0x01;

			TextureLoader(const char* fileName, int flags);
			void load(const char* fileName);
			Texture* getTexture();
			Image* getImage();
			static Image* createSpotlightTexture();
		};

	}
}
#endif