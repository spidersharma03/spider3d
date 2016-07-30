#ifndef _ENVIRONMENT_MAP_GENERATOR_H
#define _ENVIRONMENT_MAP_GENERATOR_H

#include "coreMath.h"
#include "Image.h"

using namespace spider3d::core;
using namespace spider3d::appearance;

namespace spider3d
{
	namespace effects
	{
		class EnvironmentMapGenerator
		{
		public:
			EnvironmentMapGenerator(Image** cubeMapImages)
			{
				m_cubeMapImages = cubeMapImages;
				imageWidth  = m_cubeMapImages[0]->getWidth();
				imageHeight = m_cubeMapImages[0]->getHeight();
			}

			Image* generateSkyDomeImage(int numSamples)
			{
				float s , t;
				Vector3 normal;
				Vector3 ray;
				Color3 color;
				float shift = 0.001f;
				float sample1[] = {shift,0.0f,-shift,0.0f};
				float sample2[] = {0.0f,shift,0.0f,-shift};
				numSamples = 4;
				int numChannelPerPixel = 3;
				numChannelPerPixel = m_cubeMapImages[0]->isTransparent() ? 4 : 3;
				IMAGE_FORMAT imageFormat = EIF_RGB;
				imageFormat = m_cubeMapImages[0]->isTransparent() ? EIF_RGBA : EIF_RGB;
				Image *sphereMap = new Image(imageWidth, imageHeight, imageFormat);
				unsigned char* sphereMapData = (unsigned char*)sphereMap->getData();
				for (int i = 0; i < imageHeight; i++) 
				{
					t = -1 + 2.0f * ( (float)i / (imageHeight-1) - 0.0f );
					for (int j = 0; j < imageWidth; j++)
					{
						s = -1 + 2.0f * ( (float)j / (imageWidth-1) - 0.0f );
						if ( s*s + t*t > 1.0)continue;

						int red = 0;
						int green = 0;
						int blue = 0;
						for ( int k = 0 ; k < numSamples ; k ++ )
						{
							normal.x = s + sample1[k];
							normal.y = t + sample2[k];
							float d = 1.0f - (s+sample1[k])*(s+sample1[k]) - (t + sample2[k])*(t + sample2[k]);
							d = d < 0.0f ? 0.0f : d;
							normal.z = (float) sqrt(d);
							ray.x = normal.x;
							ray.y = normal.y;
							ray.z = normal.z;
							color = cast_ray_SPH( ray , i , j);
							red += (si32)color.x;
							green += (si32)color.y;
							blue += (si32)color.z;
						}
						red /= numSamples;
						green /= numSamples;
						blue /= numSamples;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 0] = red;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 1] = green;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 2] = blue;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 3] = 255;
					}
				}

				return sphereMap;
			}

			Image* generateSphereMap(int numSamples)
			{
				float s , t;
				Vector3 normal;
				Vector3 ray;
				Color3 color;
				float shift = 0.001f;
				float sample1[] = {shift,0.0f,-shift,0.0f};
				float sample2[] = {0.0f,shift,0.0f,-shift};
				numSamples = 4;

				int numChannelPerPixel = 3;
				numChannelPerPixel = m_cubeMapImages[0]->isTransparent() ? 4 : 3;
				IMAGE_FORMAT imageFormat = EIF_RGB;
				imageFormat = m_cubeMapImages[0]->isTransparent() ? EIF_RGBA : EIF_RGB;

				Image *sphereMap = new Image(imageWidth, imageHeight, imageFormat);
				unsigned char* sphereMapData = (unsigned char*)sphereMap->getData();
				for (int i = 0; i < imageHeight; i++) 
				{
					t = -1 + 2.0f * ( (float)i / (imageHeight-1) - 0.0f );
					for (int j = 0; j < imageWidth; j++)
					{
						s = -1 + 2.0f * ( (float)j / (imageWidth-1) - 0.0f );
						if ( s*s + t*t > 1.0)continue;

						int red = 0;
						int green = 0;
						int blue = 0;
						for ( int k = 0 ; k < numSamples ; k ++ )
						{
							normal.x = s + sample1[k];
							normal.y = t + sample2[k];
							float d = 1.0 -  (s+sample1[k])*(s+sample1[k]) - (t + sample2[k])*(t + sample2[k]);
							d = d < 0.0f ? 0.0f : d;
							normal.z = (float) sqrt(d);

							ray.x = 2*normal.x * normal.z;
							ray.y = 2*normal.y * normal.z;
							ray.z = 2*normal.z * normal.z - 1.0f;
							color = cast_ray_SPH( ray , i , j);
							red += (si32)color.x;
							green += (si32)color.y;
							blue += (si32)color.z;
						}
						red /= numSamples;
						green /= numSamples;
						blue /= numSamples;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 0] = red;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 1] = green;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 2] = blue;
						sphereMapData[numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 3] = 255;
					}
				}

				return sphereMap;
			}

			Image** generateDualParaboloidMap(int numSamples)
			{
				float s , t;
				Vector3 normal;
				Vector3 ray;
				Color3 color;
				float shift = 0.001f;
				float sample1[] = {shift,0.0f,-shift,0.0f};
				float sample2[] = {0.0f,shift,0.0f,-shift};
				numSamples = 4;
				Image **dualParaboloidMap = new Image*[2];

				int numChannelPerPixel = 3;
				numChannelPerPixel = m_cubeMapImages[0]->isTransparent() ? 4 : 3;
				IMAGE_FORMAT imageFormat = EIF_RGB;
				imageFormat = m_cubeMapImages[0]->isTransparent() ? EIF_RGBA : EIF_RGB;

				dualParaboloidMap[0] = new Image(imageWidth, imageHeight, imageFormat);
				dualParaboloidMap[1] = new Image(imageWidth, imageHeight, imageFormat);
				
				unsigned char** dualParaboloidMapData = new unsigned char*[2];
				dualParaboloidMapData[0] = (unsigned char*)dualParaboloidMap[0]->getData();
				dualParaboloidMapData[1] = (unsigned char*)dualParaboloidMap[1]->getData();

				for ( int i=0; i< imageWidth*imageHeight*numChannelPerPixel; i++ )
				{
					dualParaboloidMapData[0][i] = 0;
					dualParaboloidMapData[1][i] = 0;
				}

				for (int i = 0; i < imageHeight; i++)
				{
					t = -1 + 2.0f * ( (float)i / (imageHeight-1) - 0.0f );
					for (int j = 0; j < imageWidth; j++)
					{
						s = -1 + 2.0f * ( (float)j / (imageWidth-1) - 0.0f );
						if ( s*s + t*t > 1.0)continue;
						normal.z = 1.0f;
						ray.z = 1.0f;
						for (int m = 0; m < 2; m++) {
							int red = 0;
							int green = 0;
							int blue = 0;

							for (int k = 0; k < numSamples; k++) {

								normal.x = s + sample1[k];
								normal.y = t + sample2[k];

								ray.x = 2 * normal.x * normal.z;
								ray.y = 2 * normal.y * normal.z;

								color = cast_ray_DP(ray, i, j , m);
								red   += color.x;
								green += color.y;
								blue  += color.z;
							}
							//normal[2] = -normal[2];
							ray.z = -ray.z;
							red /= 4;
							green /= 4;
							blue /= 4;
							
							dualParaboloidMapData[m][numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 0] = red;
							dualParaboloidMapData[m][numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 1] = green;
							dualParaboloidMapData[m][numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 2] = blue;
							dualParaboloidMapData[m][numChannelPerPixel*i*imageWidth + numChannelPerPixel*j + 3] = 255;
						}

					}
				}

				return dualParaboloidMap;
			}
		private:

			Color3 cast_ray_SPH(Vector3 ray , int i , int j)
			{
				int face = chooseFaceofCubeMap(ray);
				int S, T;
				chooseST(face , ray, &S, &T);
				Color3 color = getColorfromImage(face , S , T );
				return color;
			}

			Color3 cast_ray_DP(Vector3 ray , int i , int j , int m)
			{
				int face = chooseFaceofCubeMap(ray);
				if ( m == 1 )
				{
					if ( face == POS_Z)
					{
						face = NEG_Z;
					}

				}
				int S, T;
				chooseST(face , ray, &S, &T);
				Color3 color = getColorfromImage(face , S , T );
				return color;
			}
			Color3 getColorfromImage(int face , int x , int y)
			{
				Color3  color;
				unsigned char* imageData = (unsigned char*)m_cubeMapImages[face]->getData();
				int numChannelPerPixel = 3;
				numChannelPerPixel = m_cubeMapImages[face]->isTransparent() ? 4 : 3;
				int  red   = imageData[numChannelPerPixel*x*imageWidth + numChannelPerPixel*y + 0];
				int  green = imageData[numChannelPerPixel*x*imageWidth + numChannelPerPixel*y + 1];
				int  blue  = imageData[numChannelPerPixel*x*imageWidth + numChannelPerPixel*y + 2];
				color.set(red,green,blue);
				return color;
			}

			void chooseST(int face , Vector3 ray, int* S, int* T)
			{
				float data[] = {0.0f, 0.0f};
				//int[] val = new int[2];
				float s = 0 , t  = 0, r = 0;
				if ( face == POS_X)
				{
					r = ray.x ; s = ray.z; t = ray.y;
				}
				if ( face == NEG_X)
				{
					r = ray.x ; s =  ray.z; t = -ray.y;
				}
				if ( face == POS_Y)
				{
					r = ray.y ; s =  ray.z; t = -ray.x;
				}
				if ( face == NEG_Y)
				{
					r = ray.y ; s =  ray.z; t = ray.x;
				}
				if ( face == POS_Z)
				{
					r = ray.z ; s =  -ray.y; t = -ray.x;
				}
				if ( face == NEG_Z)
				{
					r = ray.z ; s =  ray.y; t = -ray.x;
				}
				data[0] = ( s / abs(r) + 1.0f) / 2.0f;
				data[1] = ( t / abs(r) + 1.0f) / 2.0f;
				*S = (int) (data[0] * imageWidth);
				*T = (int) (data[1] *imageHeight);
				if(*S > (imageWidth-1) || *T > (imageHeight-1))
				{
					*S = (imageWidth-1);
					*T = (imageHeight-1);
				}
			}



			int chooseFaceofCubeMap(Vector3 ray)
			{
				float x = ray.x ; float y = ray.y ; float z = ray.z ;
				if ( abs(x) > abs(y) )
				{
					if ( abs(x) > abs(z) )
					{
						if ( x > 0.0f)
						{
							//posxSamples++;
							return POS_X;
						}
						else
						{
							//negxSamples++;
							return NEG_X;
						}
					}
					else
					{
						if ( z > 0.0f)
						{
							//poszSamples++;
							return POS_Z;
						}
						else
						{
							//negzSamples++;
							return NEG_Z;
						}
					}
				}
				else
				{
					if ( abs(y) > abs(z) )
					{
						if ( y > 0.0f)
						{
							//posySamples++;
							return POS_Y;
						}
						else
						{
							//negySamples++;
							return NEG_Y;
						}
					}
					else
					{
						if ( z > 0.0f)
						{
							//poszSamples++;
							return POS_Z;
						}
						else
						{
							//negzSamples++;
							return NEG_Z;
						}
					}

				}

			}

			Image** m_cubeMapImages;
			int imageWidth, imageHeight;
			static const int POS_X = 0;
			static const int NEG_X = 1;
			static const int POS_Y = 2;
			static const int NEG_Y = 3;
			static const int POS_Z = 4;
			static const int NEG_Z = 5;
		};
	}
}

#endif