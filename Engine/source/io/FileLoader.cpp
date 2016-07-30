#include "io/FileLoader.h"
#include "FreeImage.h"
#include "appearance/Image.h"
#include "appearance/Texture2D.h"
#include <math.h>
#include <fstream>
#include <stdlib.h>

long int getWidthHeight(int pix1, int pix2) {
	long int num=0 ,rem;
	int n = 8;
	do {   //high byte only
		rem = pix2 % 2;
		num = num + rem * (long)(pow((float)2, (float)n));
		pix2 = pix2 /2;
		n++;
	} while(n <= 16);
	num = num + pix1;
	return(num);
}

//pointer to the image, once loaded
FIBITMAP *dib = 0;

namespace spider3d
{
	namespace io
	{
		TextureLoader::TextureLoader(const char* fileName, int flags)
		{
			imageFileName.clear();
			int i = 0;
			while( *fileName != 0 )
			{
				this->fileName[i] = *fileName;
				imageFileName.push_back(*fileName);
				i++;fileName++;
			}

			FreeImage_Initialise();

			size_t found = imageFileName.find(".");
			string extension = imageFileName.substr(found+1,imageFileName.size());
			
			if( !extension.compare("bmp") || !extension.compare("BMP") )
				loadBmp();
			//else if( !extension.compare("tga") || !extension.compare("TGA") )
			//	return loadTga();
			else
				loadFile();
			  
			IMAGE_FORMAT format = m_Image->getFormat();
			
			int textureFormat = (format == EIF_RGB) ? ETF_RGB : ETF_RGBA;

			m_Texture = new Texture2D( ETEX_BM_CLAMP , (TEXTURE_FORMAT)textureFormat , m_Image->getWidth()  , m_Image->getHeight()   ); 
			
			int w = m_Image->getWidth();
			int h = m_Image->getHeight();
			int mipMapLevel = 0;

			((Texture2D*)m_Texture)->setImage(mipMapLevel,m_Image);

			if ( flags & GENERATE_MIP_MAPS )
			{
				do
				{
					w /= 2;
					h /= 2;
					mipMapLevel++;
					FIBITMAP *dibscaled = FreeImage_Rescale(dib,w,h,FILTER_BOX);
					//retrieve the image data
					BYTE* bits = FreeImage_GetBits(dibscaled);
					bool bTransparent = FreeImage_IsTransparent(dibscaled);
					int numChannelPerPixel = 3;
					numChannelPerPixel = bTransparent ? 4 : 3;

					IMAGE_FORMAT imageFormat = EIF_RGB;
					imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;

					Image *mipMapImage = new Image(w , h , imageFormat);
					memcpy(mipMapImage->getData() , bits , w*h*numChannelPerPixel);

					m_Texture->setMipMapMode(E_MIPMAP_MODE_MULTI_LEVEL);
					((Texture2D*)m_Texture)->setImage(mipMapLevel,mipMapImage);
					FreeImage_Unload(dibscaled);
				}
				while( w !=1 );
			}

			//Free FreeImage's copy of the data
			if( extension.compare("bmp") )
				FreeImage_Unload(dib);
		}

		void TextureLoader::load(const char* fileName)
		{
			unsigned i = 0;
			imageFileName.clear();
			while( *fileName != 0 )
			{
				this->fileName[i] = *fileName;
				imageFileName.push_back(*fileName);
				i++;fileName++;
			}
			size_t found = imageFileName.find(".");
			string extension = imageFileName.substr(found+1,imageFileName.size());
			
			if( !extension.compare("bmp") || !extension.compare("BMP") )
				return loadBmp();
			//else if( !extension.compare("tga") || !extension.compare("TGA") )
			//	return loadTga();
			else
				return loadFile();
		}

		void TextureLoader::saveFile(const char* fileName)
		{
			
		}

		Texture* TextureLoader::getTexture()
		{
			return m_Texture;
		}

		Image* TextureLoader::getImage()
		{
			return m_Image;
		}

		void TextureLoader::loadFile()
		{
			//image format
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			//pointer to the image data
			BYTE* bits = 0;
			//image width and height
			unsigned int width(0), height(0);
			//check the file signature and deduce its format
			fif = FreeImage_GetFileType(imageFileName.c_str(), 0);
			//if still unknown, try to guess the file format from the file extension
			if(fif == FIF_UNKNOWN) 
				fif = FreeImage_GetFIFFromFilename(imageFileName.c_str());
			//if still unkown, return failure
			if(fif == FIF_UNKNOWN)
				return;
			//check that the plugin has reading capabilities and load the file
			if(FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, imageFileName.c_str());
			//if the image failed to load, return failure
			if(!dib)
				return;
			//retrieve the image data
			bits = FreeImage_GetBits(dib);
			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			bool bTransparent = FreeImage_IsTransparent(dib);
			//if this somehow one of these failed (they shouldn't), return failure
			if((bits == 0) || (width == 0) || (height == 0))
				return;
			int numChannelPerPixel = 3;
			numChannelPerPixel = bTransparent ? 4 : 3;

			for(int i = 0;i<width*height*numChannelPerPixel;i+=numChannelPerPixel)
			{
				BYTE temp = bits[i];
				bits[i]   = bits[i+2];
				bits[i+2]   = temp;
			}
			//printf("%d%s%d%s%d%s" , bits[i]," ", bits[i+1], " ", bits[i+2], " ");
			IMAGE_FORMAT imageFormat = EIF_RGB;
			imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;

			m_Image = new Image(width , height , imageFormat);
			bool flag = FreeImage_IsTransparent(dib);
			memcpy(m_Image->getData() , bits , width*height*numChannelPerPixel);
		}

		void TextureLoader::loadBmp()
		{
			unsigned char pixel;
			int i, j , k=0 ,x, pixlow, pixhigh, pix3, pix4, BMok;
			ifstream inpaint( imageFileName.c_str(), ios::binary);
			if(!inpaint) {
				printf("File not opened / found\n");
				exit(1);
			}
			BMok = 0;
			inpaint >> pixel;
			if(pixel == 'B') BMok++;
			inpaint >> pixel;
			if(pixel == 'M') BMok++;
			if(BMok == 2) printf("is a valid bit map file\n");
			else exit(1);
			for(x=3; x<=54; x++) {
				inpaint >> pixel; //go to start of data after header stuff
				if(x == 19) pixlow = pixel; //low
				if(x == 20) pixhigh = pixel; //high
				if(x == 23) pix3 = pixel; //low
				if(x == 24) pix4 = pixel; //high
			}
			int width = getWidthHeight( pixlow, pixhigh);
			int height = getWidthHeight( pix3, pix4);
			//cout << "\nx width = " << xw << "y height = " << yh << endl;
			unsigned char* data = new unsigned char[4*width*height];
			//xpack = ww % 4; // multiples of 4 bytes/x line
			for (j = 0; j < height; j++) {
				for (i = 0; i < width; i++) {
					data[k+3] = 255;
					data[k+2] = inpaint.get( );
					data[k+1] = inpaint.get( );
					data[k+0] = inpaint.get( );
					//cout<<data[k+3]<<" "<<data[k+2]<<" "<<data[k+1]<<" "<<data[k]<<endl;
					k += 4;
				}
				//if (xpack != 0) for (i=1; i<=xpack; i++) inpaint.get(pixel);
			}

			m_Image = new Image(width , height , EIF_RGBA);
			memcpy(m_Image->getData() , data , width*height*4);
			delete[] data;
		}

		void TextureLoader::loadTga()
		{
			unsigned char   TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
			unsigned char	TGAcompare[12];								// Used To Compare TGA Header
			unsigned char	header[6];									// First 6 Useful Bytes From The Header
			unsigned		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
			unsigned		imageSize;									// Used To Store The Image Size When Setting Aside Ram
			unsigned		temp;										// Temporary Variable

			FILE *file = fopen(imageFileName.c_str(), "rb");						// Open The TGA File

			if(	file==NULL ||										// Does File Even Exist?
				fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
				memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
				fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
			{
				if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
					return;									// Return False
				else
				{
					fclose(file);									// If Anything Failed, Close The File
					return;									// Return False
				}
			}

			float width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
			float height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

			if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
				height	<=0	||								// Is The Height Less Than Or Equal To Zero
				(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
			{
				fclose(file);										// If Anything Failed, Close The File
				return;										// Return False
			}

			unsigned char bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
			bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
			imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

			//texture->imageData=(osmUByte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data
			unsigned char* data = new unsigned char[imageSize];

			if(	data ==0 ||							// Does The Storage Memory Exist?
				fread(data, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
			{
				if(data!=0)							// Was Image Data Loaded
					free(data);						// If So, Release The Image Data

				fclose(file);										// Close The File
				return;										// Return False
			}

			for(unsigned i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
			{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
				temp = data[i];							// Temporarily Store The Value At Image Data 'i'
				data[i] = data[i + 2];				// Set The 1st Byte To The Value Of The 3rd Byte
				data[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
			}

			fclose (file);											// Close The Fil
		    
			m_Image = new Image(width , height , EIF_RGBA);
			memcpy(m_Image->getData() , data , imageSize);
			delete[] data;
		}


		Image* TextureLoader::createSpotlightTexture()
		{
			int texWidth = 128, texHeight = 128;
			unsigned char *texData = 0;
			float borderColor[4] =
			{0.1f, 0.1f, 0.1f, 0.005f};

			if (!texData) {
				unsigned char *p;
				int i, j;

				texData = new unsigned char[texWidth * texHeight * 4 ];

				p = texData;
				for (j = 0; j < texHeight; ++j) {
					float dy = (texHeight * 0.5 - j + 0.5) / (texHeight * 0.5);

					for (i = 0; i < texWidth; ++i) {
						float dx = (texWidth * 0.5 - i + 0.5) / (texWidth * 0.5);
						float r = cos(3.14159265f / 2.0 * sqrt(dx * dx + dy * dy));
						float c;

						r = (r < 0) ? 0 : r * r;
						c = 0xff * (r + borderColor[0]);
						p[0] = (c <= 0xff) ? c : 0xff;
						c = 0xff * (r + borderColor[1]);
						p[1] = (c <= 0xff) ? c : 0xff;
						c = 0xff * (r + borderColor[2]);
						p[2] = (c <= 0xff) ? c : 0xff;
						c = 0xff * (r + borderColor[3]);
						p[3] = (c <= 0xff) ? c : 0xff;
						//printf("Alpha=%d%s",p[3]," ");
						p += 4;
					}
				}
			}

			Image *image = new Image(texWidth , texHeight , EIF_RGBA );
			memcpy(image->getData() , texData , texWidth*texHeight*4);
			return image;
		}
	}
}