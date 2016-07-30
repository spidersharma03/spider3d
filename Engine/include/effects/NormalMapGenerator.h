#ifndef _NORMAL_MAP_GENERATOR_H_
#define _NORMAL_MAP_GENERATOR_H_

#include "Image.h"
#include "FreeImage.h"

using namespace spider3d::appearance;


namespace spider3d
{
	namespace effects
	{
		class NormalMapGenerator
		{
		public:
			NormalMapGenerator()
			{
				m_Image = 0;
			}

			NormalMapGenerator(Image* image)
			{
				m_Image = image;
			}

			void setImage(Image* image)
			{
				m_Image = image;
			}
			
			Image* generateGreyScaleImage()
			{
				if( !m_Image )
					return 0;
				
				int imageHeight = m_Image->getHeight();
				int imageWidth  = m_Image->getWidth();
				unsigned char* imageData = (unsigned char*)m_Image->getData();
					
				Image *greyScaleMapImage = new Image(imageWidth, imageHeight, m_Image->getFormat());
				unsigned char* greyScaleImageData = (unsigned char*)greyScaleMapImage->getData();

				int numChannels = 3;
				numChannels = m_Image->getFormat() == EIF_RGBA ? 4 : 3;

				for( int i=0; i<imageHeight; i++ )
				{
					for( int j=0; j<imageWidth; j++ )
					{
							unsigned char r = imageData[numChannels*i*imageWidth + numChannels*j + 0];
							unsigned char g = imageData[numChannels*i*imageWidth + numChannels*j + 1];
							unsigned char b = imageData[numChannels*i*imageWidth + numChannels*j + 2];
							
							float color = ( r + g + b )/3;
							greyScaleImageData[numChannels*i*imageWidth + numChannels*j + 0] = color;
							greyScaleImageData[numChannels*i*imageWidth + numChannels*j + 1] = color;
							greyScaleImageData[numChannels*i*imageWidth + numChannels*j + 2] = color;
					}
				}
				return greyScaleMapImage;
			}

			Image* generateNormalMapImage()
			{
				if( !m_Image )
					return 0;
				
				int imageHeight = m_Image->getHeight();
				int imageWidth  = m_Image->getWidth();
				unsigned char* imageData = (unsigned char*)m_Image->getData();
					
				Image *normalMapImage = new Image(imageWidth, imageHeight, m_Image->getFormat());
				unsigned char* normalMapImageData = (unsigned char*)normalMapImage->getData();

				int numChannels = 3;
				numChannels = m_Image->getFormat() == EIF_RGBA ? 4 : 3;

				for( int i=0; i<imageHeight; i++ )
				{
					for( int j=0; j<imageWidth; j++ )
					{
						if( (i !=0) && (j != 0) && (i != (imageHeight-1)) && (j != (imageWidth-1)) )
						{
							unsigned char rxf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 0];
							unsigned char gxf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 1];
							unsigned char bxf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 2];
							
							float xf1 = ( rxf1 + gxf1 + bxf1 )/3;

							unsigned char rxb1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 0];
							unsigned char gxb1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 1];
							unsigned char bxb1 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 2];

							float xb1 = ( rxb1 + gxb1 + bxb1 )/3;

							unsigned char rxf2 = imageData[numChannels*i*imageWidth + numChannels*(j+1) + 0];
							unsigned char gxf2 = imageData[numChannels*i*imageWidth + numChannels*(j+1) + 1];
							unsigned char bxf2 = imageData[numChannels*i*imageWidth + numChannels*(j+1) + 2];
							
							float xf2 = ( rxf2 + gxf2 + bxf2 )/3;

							unsigned char rxb2 = imageData[numChannels*i*imageWidth + numChannels*(j-1) + 0];
							unsigned char gxb2 = imageData[numChannels*i*imageWidth + numChannels*(j-1) + 1];
							unsigned char bxb2 = imageData[numChannels*i*imageWidth + numChannels*(j-1) + 2];

							float xb2 = ( rxb2 + gxb2 + bxb2 )/3;

							unsigned char rxf3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 0];
							unsigned char gxf3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 1];
							unsigned char bxf3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 2];
							
							float xf3 = ( rxf3 + gxf3 + bxf3 )/3;

							unsigned char rxb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 0];
							unsigned char gxb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 1];
							unsigned char bxb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 2];

							float xb3 = ( rxb3 + gxb3 + bxb3 )/3;

							unsigned char ryf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*j + 0];
							unsigned char gyf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*j + 1];
							unsigned char byf1 = imageData[numChannels*(i+1)*imageWidth + numChannels*j + 2];

							float yf1 = ( ryf1 + gyf1 + byf1 )/3;

							unsigned char ryb1 = imageData[numChannels*(i-1)*imageWidth + numChannels*j + 0];
							unsigned char gyb1 = imageData[numChannels*(i-1)*imageWidth + numChannels*j + 1];
							unsigned char byb1 = imageData[numChannels*(i-1)*imageWidth + numChannels*j + 2];

							float yb1 = ( ryb1 + gyb1 + byb1 )/3;

							unsigned char ryf2 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 0];
							unsigned char gyf2 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 1];
							unsigned char byf2 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j+1) + 2];

							float yf2 = ( ryf2 + gyf2 + byf2 )/3;

							unsigned char ryb2 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 0];
							unsigned char gyb2 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 1];
							unsigned char byb2 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j+1) + 2];

							float yb2 = ( ryb2 + gyb2 + byb2 )/3;

							unsigned char ryf3 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 0];
							unsigned char gyf3 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 1];
							unsigned char byf3 = imageData[numChannels*(i+1)*imageWidth + numChannels*(j-1) + 2];

							float yf3 = ( ryf3 + gyf3 + byf3 )/3;

							unsigned char ryb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 0];
							unsigned char gyb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 1];
							unsigned char byb3 = imageData[numChannels*(i-1)*imageWidth + numChannels*(j-1) + 2];

							float yb3 = ( ryb3 + gyb3 + byb3 )/3;

							float smooth = 0.25f;

							float dx = -(xf1 + 2*xf2 + xf3 - xb1 - 2*xb2 - xb3)/255;
							float dy = -(2*yf1 + yf2 + yf3 - 2*yb1 - yb2 - yb3)/255;
							
							dx *= smooth;
							dy *= smooth;
							dx = 255*(dx * 0.5f + 0.5f);
							dy = 255*(dy * 0.5f + 0.5f);
							
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 0] = dx;
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 1] = dy;
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 2] = 255;

							/*normalMapImageData[3*i*imageWidth +  3*j + 0] = abs(yf1-yb1);
							normalMapImageData[3*i*imageWidth +  3*j + 1] = abs(yf1-yb1);
							normalMapImageData[3*i*imageWidth +  3*j + 2] = abs(yf1-yb1);*/
						}
						else
						{
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 0] = 0;
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 1] = 0;
							normalMapImageData[numChannels*i*imageWidth +  numChannels*j + 2] = 255;
						}
					}
				}

				/*FIBITMAP *dib = 0;
				int BPP = numChannels == 3 ? 24 : 32;
				IMAGE_FORMAT imageFormat = EIF_RGB;
				imageFormat = m_Image->getFormat();

				dib = FreeImage_Allocate(m_Image->getWidth(), m_Image->getHeight(), BPP);
				BYTE* bytes = FreeImage_GetBits(dib);
				for(int i = 0;i<m_Image->getWidth()*m_Image->getHeight()*numChannels;i+=numChannels)
				{
					BYTE temp = normalMapImageData[i];
					normalMapImageData[i]   = normalMapImageData[i+2];
					normalMapImageData[i+2]   = temp;
				}
				memcpy(bytes , normalMapImageData , m_Image->getWidth()*m_Image->getHeight()*numChannels);
				FreeImage_Save(FIF_BMP,dib,"NormalMap.bmp");
				FreeImage_Unload(dib);*/
				return normalMapImage;
			}

		private:
			Image *m_Image;
		};
	}
}
#endif
