#include "appearance/Image.h"
#include "FreeImage.h"
#include <math.h>
#include <string>

using namespace std;

namespace spider3d
{
	namespace appearance
	{
		Image::Image(unsigned width, unsigned height ):width(width),height(height),format(EIF_RGB)
		{
			data = new unsigned char[width*height*3];
		}

		Image::Image(unsigned width, unsigned height, IMAGE_FORMAT format ):width(width),height(height),format(format)
		{
			switch(format)
			{
				case EIF_RGB:
					data = new unsigned char[width*height*3];
					m_bIsTransparent = false;
					break;
				case EIF_RGBA:
					data = new unsigned char[width*height*4];
					m_bIsTransparent = true;
					break;
				default:
					m_bIsTransparent = false;
					break;
			}
		}

		IMAGE_FORMAT Image::getFormat()
		{
			return format;
		}

		void* Image::getData()
		{
			return data;
		}

		unsigned Image::getWidth()
		{
			return width;
		}

		unsigned Image::getHeight()
		{
			return height;
		}

		Image* Image::getScaledImage(float width, float height)
		{
			FIBITMAP *dib = 0;
			int numChannelPerPixel = 3;
			numChannelPerPixel = m_bIsTransparent ? 4 : 3;
			int BPP = numChannelPerPixel == 3 ? 24 : 32;
			IMAGE_FORMAT imageFormat = EIF_RGB;
			imageFormat = m_bIsTransparent ? EIF_RGBA : EIF_RGB;

			dib = FreeImage_Allocate(this->width, this->height, BPP);
			BYTE* bytes = FreeImage_GetBits(dib);
			memcpy(bytes , data , this->width*this->height*numChannelPerPixel);

			FIBITMAP *dibscaled = FreeImage_Rescale(dib,width,height,FILTER_BOX);
			//retrieve the image data
			BYTE* bits = FreeImage_GetBits(dibscaled);
			//FreeImage_Save(FIF_BMP, dib, "SkyDome.bmp");

			Image *newImage = new Image(width , height , imageFormat);
			memcpy(newImage->getData() , bits , width*height*numChannelPerPixel);
			FreeImage_Unload(dib);
			FreeImage_Unload(dibscaled);
			return newImage;
		}

		void Image::save(char* imageName)
		{
			FIBITMAP *dib = 0;
			int numChannelPerPixel = 3;
			numChannelPerPixel = m_bIsTransparent ? 4 : 3;
			int BPP = numChannelPerPixel == 3 ? 24 : 32;
			IMAGE_FORMAT imageFormat = EIF_RGB;
			imageFormat = m_bIsTransparent ? EIF_RGBA : EIF_RGB;

			dib = FreeImage_Allocate(this->width, this->height, BPP);
			BYTE* bytes = FreeImage_GetBits(dib);

			memcpy(bytes , data , this->width*this->height*numChannelPerPixel);
			/*BYTE* rawData = (BYTE*)data;
			for(int i = 0;i<width*height*numChannelPerPixel;i+=numChannelPerPixel)
			{
				BYTE temp = rawData[i];
				bytes[i]   = rawData[i+2];
				rawData[i+2]   = temp;
			}*/
			

			FreeImage_Save(FIF_BMP, dib, imageName);

			FreeImage_Unload(dib);
		}

		void Image::flipVertical()
		{
			int numColorComponents = ( format == EIF_RGB ? 3 : 4 );
			if ( data )
			{
				unsigned char* imageData = (unsigned char*)data;
				for ( int j = 0, k = height-1 ; j < height/2, k >= height/2 ; j ++ ,k--)
				{
					for ( int i = 0 ; i < numColorComponents*width ; i++ )
					{
						unsigned char temp = imageData[j*numColorComponents*width + i];
						imageData[j*numColorComponents*width + i] =  imageData[k*numColorComponents*width  + i];
						imageData[k*numColorComponents*width  + i] = temp; 
					}
				}
			}
		}

		void Image::pad(unsigned int padding , unsigned char r , unsigned char g ,unsigned char b ,unsigned char a)
		{
			int numColorComponents = ( format == EIF_RGB ? 3 : 4 );

			unsigned char *pixelData = new unsigned char[numColorComponents*width*height];
			unsigned char *imageData = (unsigned char*)data;
			for ( unsigned i = 0 ; i < numColorComponents*width*height; i++ )
				pixelData[i] = imageData[i];

			delete[] data;
			data  = new unsigned char[numColorComponents*(width+2*padding)*(height+2*padding) ];
		    unsigned char *newimageData = (unsigned char*)data;
			unsigned int l = 0 , m = 0;
			for ( int j = 0 ; j < height + 2*padding ; j ++ )
			{
				for ( int i = 0 ; i < width + 2*padding ; i++ )
				{
					if ( i < padding || i >= ( width + padding) || j < padding || j >= ( height + padding) )
					{
						newimageData[0] = 255;
						newimageData[1] = 255;
						newimageData[2] = 255;
						if ( numColorComponents == 4 )
						newimageData[3] = 255;
					}
					else
					{
						newimageData[0]   = pixelData[m];
						newimageData[1]   = pixelData[m+1];
						newimageData[2]   = pixelData[m+2];
						if ( numColorComponents == 4 )
						newimageData[3] = 255;
						m += numColorComponents;
					}
					newimageData += numColorComponents;
				}
			}
			/*for (int j = 0; j < height + 2*padding; ++j) {
				for (int i = 0; i < width + 2*padding; ++i) {
					if (padding <= j && j <= height + padding  && padding <= i && i <= width + padding ) {
						newimageData[0] = pixelData[4 * (j * width + i) + 0];
						newimageData[1] = pixelData[4 * (j * width + i) + 1];
						newimageData[2] = pixelData[4 * (j * width + i) + 2];
					} else {
						newimageData[0] = r * 0xff;
						newimageData[1] = g * 0xff;
						newimageData[2] = b * 0xff;
					}
					newimageData += 3;
				}
		  }*/
			delete[] pixelData;
		}

		bool Image::isTransparent()
		{
			return false;
		}

		Image::~Image()
		{
			delete[] data;
		}
	
	}
}