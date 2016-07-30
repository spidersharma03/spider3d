#ifndef _IMAGE_H
#define _IMAGE_H


namespace spider3d
{
	enum IMAGE_FORMAT { EIF_RGB , EIF_RGBA};

	namespace appearance
	{
		class Image
		{
		private:
			unsigned width, height;
			void* data;
			IMAGE_FORMAT format;
			bool m_bIsTransparent;
		public:
			Image(unsigned width, unsigned height);
			Image(unsigned width, unsigned height, IMAGE_FORMAT format );
			unsigned getWidth();
			unsigned getHeight();
			IMAGE_FORMAT getFormat();
			void* getData();
			void flipVertical();
			Image* getScaledImage(float width, float height);
			void save(char* name = "image.jpg");
			void pad(unsigned int padding , unsigned char r , unsigned char g ,unsigned char b ,unsigned char a);
			bool isTransparent();
			virtual ~Image();
		};
	}
}
#endif