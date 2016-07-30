#ifndef _LIGHT_MAP_H
#define _LIGHT_MAP_H

#include "coreMath.h"
#include "TextureCubeMap.h"

using namespace spider3d::appearance;

namespace spider3d
{
	namespace effects
	{
        struct LightMapParams
		{
			Color3 color;
			Vector3 direction;
			float shininess;
		};

		class LightMap
		{
		public:

			LightMap()
			{
			}

			void addLightMapParams(LightMapParams* lightMapParams)
			{
				m_vecLightMapParams.push_back(lightMapParams);
			}

			TextureCubeMap* createCubeLightMap(bool bDiffuseMap, int size, Color3& lightColor, float shininess)
			{
				lightMapSize = size;
				lightDir.set(1.0,2.0,3.0);
				lightDir.normalize();
				TextureCubeMap *specularCubeMap = new TextureCubeMap(ETEX_BM_WRAP,ETF_RGBA,size,size);
				images = new Image*[6];
				for( int i=0; i<6; i++)
				{
					images[i] = new Image(size,size,EIF_RGBA);
				}
				specularCubeMap->setImage(0, ECMF_POSX , images[0] );
				specularCubeMap->setImage(0, ECMF_NEGX , images[1] );	
				specularCubeMap->setImage(0, ECMF_POSY , images[2] );	
				specularCubeMap->setImage(0, ECMF_NEGY , images[3] );	
				specularCubeMap->setImage(0, ECMF_POSZ , images[4] );	
				specularCubeMap->setImage(0, ECMF_NEGZ , images[5] );

				int texSize = lightMapSize;
				float dst = 2.0f/(texSize-1);

				float t = -1;
				for (int i=0; i<texSize; ++i)
				{
					float s = -1;
					for (int j=0; j<texSize; ++j)
					{
						setPixelInFaceMap(bDiffuseMap, 0, j, i, computeColorAtPixel(Vector3(1, -t, -s), bDiffuseMap ) );
						setPixelInFaceMap(bDiffuseMap, 1, j, i, computeColorAtPixel(Vector3(-1, -t, s), bDiffuseMap  ));
						setPixelInFaceMap(bDiffuseMap, 2, j, i, computeColorAtPixel(Vector3(s, 1, t), bDiffuseMap    ));
						setPixelInFaceMap(bDiffuseMap, 3, j, i, computeColorAtPixel(Vector3(s, -1, -t), bDiffuseMap  ));
						setPixelInFaceMap(bDiffuseMap, 4, j, i, computeColorAtPixel(Vector3(s, -t, 1), bDiffuseMap  ));
						setPixelInFaceMap(bDiffuseMap, 5, j, i, computeColorAtPixel(Vector3(-s, -t, -1), bDiffuseMap ));
						s += dst;
					}
					t += dst;
				}
				return specularCubeMap;
			}

		private:

			void setPixelInFaceMap(bool bDiffuseMap, int faceIndex, int c, int r, Color3 color)
			{
				int R = (int)  (color.x * 255);
				int G = (int)  (color.y * 255);
				int B = (int)  (color.z * 255);

				int RGBval = 0;

				int alpha = 0;
				unsigned char* data = (unsigned char*)images[faceIndex]->getData();

				if( bDiffuseMap)
				{
					alpha = 0xff;
				}
				else // For specular map
				{
					alpha = R;
					alpha = 255;
					if ( R < 255)
					{
						// R = G = B = 255;
					}

				}
				//RGBval = alpha<<24 | R<<16 | G<<8 | B ;  //Format is  A R G B
				data[ 4*r*lightMapSize + 4*c + 0] = R;
				data[ 4*r*lightMapSize + 4*c + 1] = G;
				data[ 4*r*lightMapSize + 4*c + 2] = B;
				data[ 4*r*lightMapSize + 4*c + 3] = alpha;
			}

			Color3 computeColorAtPixel( Vector3 reflectionVec, bool diffuseMap)
			{
				reflectionVec.normalize();
				// Compute the final color at pixel for all lights
				Color3 finalColor( 0.0f, 0.0f, 0.0f);

				for( su32 l=0; l<m_vecLightMapParams.size(); l++)
				{
					sf32 shininess = m_vecLightMapParams[l]->shininess;
					if( diffuseMap == true)  // Diffuse map is True flag
					{
						shininess = 1;
					}
					Color3 lightCol = m_vecLightMapParams[l]->color;
					Vector3 lightDir = m_vecLightMapParams[l]->direction;
					lightDir.normalize();
					sf32 v = lightDir.dot(reflectionVec);

					if (v < 0)
					{
						v = 0;
					}
					float specComponent = (float) pow(v, shininess);
					lightCol.x *= specComponent;
					lightCol.y *= specComponent;
					lightCol.z *= specComponent;
					if (diffuseMap)
					{
						lightCol.x *= 0.5f;
						lightCol.y *= 0.5f;
						lightCol.z *= 0.5f;
					}
					// Accumulate and Clamp Color
					finalColor.x += lightCol.x;
					finalColor.y += lightCol.y;
					finalColor.z += lightCol.z;
				}
				// Clamp max color
				if( finalColor.x > 1.0f) finalColor.x = 1.0f;
				if( finalColor.y > 1.0f) finalColor.y = 1.0f;
				if( finalColor.z > 1.0f) finalColor.z = 1.0f;

				return finalColor;
			}

			Image** images;
			int lightMapSize;
			Vector3 lightDir;
			vector<LightMapParams*> m_vecLightMapParams;
		};

	}
}

#endif
