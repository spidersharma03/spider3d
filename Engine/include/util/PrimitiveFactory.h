#ifndef _PRIMITIVEFACTORY_H

#define _PRIMITIVEFACTORY_H

class SMesh;

#include "coreMath.h"
#include "SIndexedMeshBuffer.h"
#include "Mesh.h"
#include <math.h>

using namespace spider3d::core;

namespace spider3d
{

	namespace scene
	{

		class PrimitiveFactory
		{
		public:
			static Mesh* createTorus(sf32 ringRadius , sf32 innerMinorRadius , sf32 innerMajorRadius , su32 numInnerSeg , su32 numOuterSeg , bool va = false)
			{
				sf32 theta = (sf32)(2*M_PI/numInnerSeg);//m
				sf32 angle = (sf32)(2*M_PI/numOuterSeg);//n
				sf32 alpha = 0.0;
				sf32 beta = 0.0;
				
				Point3* vertices  = new Point3[ numInnerSeg * numOuterSeg ];
				Point3* normals   = new Point3[ numInnerSeg * numOuterSeg ];

				sf32 x,y,z;
				su32 l=0;
				for(su32 i=0 ; i < numInnerSeg ; i++ )
				{
					x = ringRadius  +  innerMajorRadius * cos (beta) ;
					y = innerMinorRadius * sin(beta) ;
					z = sqrt( x * x + y * y );
					l = 0;

					for ( su32 j=0 ; j < numOuterSeg ; j++ )
					{
						vertices[l + i].x     = x * cos( alpha );
						vertices[l + i].y = y;
						vertices[l + i].z = z * sin( alpha );
						l = numInnerSeg *( j + 1 );
						alpha += angle;
					}

					beta += theta;
				}


				su16* indices = new su16[ 4 * numOuterSeg * numInnerSeg ];

				l = 0;
				su32 k = 0;

				for(su32 j=0 ; j < numOuterSeg ; j++ ) 
				{
					for( su32 i = 0 ; i < numInnerSeg ; i++)
					{
						if ( (i == numInnerSeg - 1) & (j != numOuterSeg - 1) )
						{
							indices[l++] = i + k;
							indices[l++] = i + k + 1 - numInnerSeg;
							indices[l++] = i + k + 1;
							indices[l++] = 2*i + k + 1;
						}
						else if ( (i != numInnerSeg -1 ) & ( j != numOuterSeg-1 ) )
						{
							indices[l++] = i + k;
							indices[l++] = i + k + 1;
							indices[l++] = i + k + 1 + numInnerSeg;
							indices[l++] = i + k + numInnerSeg;
						}
						else if( ( i == numInnerSeg-1 )  & ( j == numOuterSeg-1 ) )
						{
							indices[l++] = i + k;
							indices[l++] = i + k + 1 - numInnerSeg;
							indices[l++] = i - numInnerSeg + 1;
							indices[l++] = i;
						}
						else if( (i != numInnerSeg-1) & (j == numOuterSeg-1) )
						{
							indices[l++] = i+k;
							indices[l++] = i+k+1;
							indices[l++] = i+1;
							indices[l++] = i;
						}
					}
					k = k + numInnerSeg;
				}



					for ( su32 i = 0 ; i < 4 * numInnerSeg * numOuterSeg ; i +=4 )
					{
						su32 one   = indices[i];
						su32 two   = indices[i+1];
						su32 three = indices[i+2];
						su32 four  = indices[i+3];

						Vector3 A;
						Vector3 B;
						Vector3 N;

						A.x = vertices[three].x - vertices[one].x;
						A.y = vertices[three].y - vertices[one].y;
						A.z = vertices[three].z - vertices[one].z;

						B.x = vertices[two].x - vertices[one].x;
						B.y = vertices[two].y - vertices[one].y;
						B.z = vertices[two].z - vertices[one].z;

						N = B.cross(A);


						N.normalize();

						normals[one].x = N.x;
						normals[one].y = N.y;
						normals[one].z = N.z;

						normals[two].x = N.x;
						normals[two].y = N.y;
						normals[two].z = N.z;

						normals[three].x = N.x;
						normals[three].y = N.y;
						normals[three].z = N.z;

						normals[four].x = N.x;
						normals[four].y = N.y;
						normals[four].z = N.z;
					}

                    sf32* coords = new sf32[3*numInnerSeg * numOuterSeg];
					k = 0;
                    for ( su32 i = 0 ; i < numInnerSeg * numOuterSeg ; i++)
					{
						coords[k] = vertices[i].x;
						coords[k + 1] = vertices[i].y;
						coords[k + 2] = vertices[i].z;
						k += 3;
					}
					delete vertices;

					sf32* norms = new sf32[3*numInnerSeg * numOuterSeg];
					k = 0;
                    for ( su32 i = 0 ; i < numInnerSeg * numOuterSeg ; i++)
					{
						norms[k] = normals[i].x;
						norms[k + 1] = normals[i].y;
						norms[k + 2] = normals[i].z;
						k += 3;
					}
					delete[] normals;

					//SIndexedMeshBuffer* buffer = new SIndexedMeshBuffer( numInnerSeg * numOuterSeg , COORDINATES | NORMALS , 4 * numInnerSeg * numOuterSeg );
					
					SIndexedMeshBuffer* buffer;
					si32 vaSize[] = {3};
					if( !va )
					{
						buffer = new SIndexedMeshBuffer( numInnerSeg * numOuterSeg , COORDINATES | NORMALS, 1 ,4 * numInnerSeg * numOuterSeg );
					}
					else
					{
						buffer = new SIndexedMeshBuffer( numInnerSeg * numOuterSeg , COORDINATES | NORMALS | VERTEX_ATTRIBUTES, 1 , 1 , vaSize ,4 * numInnerSeg * numOuterSeg );
						sf32 *va = new sf32[3*numInnerSeg * numOuterSeg];
						for( int i=0; i<3*numInnerSeg * numOuterSeg; i+=3 )
						{
							va[i]   = norms[i];
							va[i+1] = norms[i+1];
							va[i+2] = norms[i+2];
						}
						buffer->setVertexAttributes(0,va);
						delete[] va;
					}

					buffer->setCoordinates(coords);
					buffer->setNormals(norms);
					buffer->setIndices(indices);
					buffer->setPrimitiveType(QUADS);
					buffer->reCalculateAABB();

					delete[] coords;
					delete[] norms;
					delete[] indices;

					Mesh* mesh = new Mesh();
					mesh->addMeshBuffer(buffer);
                    mesh->reCalculateAABB(); 
					return mesh;

			}

			static Mesh* createPolyPlane( sf32 width, sf32 length, su32 divX, su32 divZ, bool bTemp = true, bool bTemp1 = true)
			{
				sf32* coordinates = new sf32[(divX+1) * (divZ+1) * 3];
				sf32* texcoords = new sf32[(divX+1) * (divZ+1) * 2];
                sf32* normals     = new sf32[(divX+1) * (divZ+1) * 3];
				su16* indices = new su16[ 6 * divX * divZ ];
				
				int cnt1 = 0;
				int cnt2 = 0;
				float X = -width*0.5f;
				float Z = length*0.5f;
				float dx = width/divX;
				float dz = length/divZ;
				
				// Creating Geometry
				for( su32 i = 0 ; i < divZ+1 ; i++ )
				{
					X = -width*0.5f;
					for ( su32 j = 0 ; j < divX+1 ; j++ )
					{
						coordinates[cnt1]   = X;
						coordinates[cnt1+1] = 0.0f;
						coordinates[cnt1+2] = Z;
						if( bTemp )
						{
							texcoords[cnt2]   = 12*(X/width + 0.5f);
							texcoords[cnt2+1] = 12*(Z/length + 0.5f);
						}
						else
						{
							texcoords[cnt2]   = 1*(X/width + 0.5f);
							texcoords[cnt2+1] = 1*(Z/length + 0.5f);
						}
							//texcoords[cnt2+2] = Z;
						normals[cnt1]   = 0.0f;
						normals[cnt1+1] = 1.0f;
						normals[cnt1+2] = 0.0f;
						X += dx;
						cnt1 += 3;
						cnt2 += 2;
					}
					Z -= dz;
				}
				
				cnt1 = 0;
				// Creating Indices
				for( su32 i = 0 ; i < divZ ; i++ )
				{
					for ( su32 j = 0 ; j < divX ; j++ )
					{
						indices[cnt1]   = i*(divX+1) + j;
						indices[cnt1+1] = i*(divX+1) + j + 1;
						indices[cnt1+2] = i*(divX+1) + divX + j + 2;

						indices[cnt1+3] = i*(divX+1) + j;
						indices[cnt1+4] = i*(divX+1) + divX + j + 2;
						indices[cnt1+5] = i*(divX+1) + divX + j + 1;
						cnt1 += 6;
					}
				}

				SIndexedMeshBuffer* buffer = 0;
				si32 vaSize[] = {3,3,3};
				if( bTemp1 )
				    buffer = new SIndexedMeshBuffer( (divX+1) * (divZ+1) , COORDINATES | NORMALS | TEXTURE_COORDINATE_2, 1 ,6 * (divX) * (divZ) );
				
				if ( !bTemp1 )
				{
					buffer = new SIndexedMeshBuffer( (divX+1) * (divZ+1) , COORDINATES | NORMALS | TEXTURE_COORDINATE_2 | VERTEX_ATTRIBUTES, 2 , 3 , vaSize , 6 * (divX) * (divZ));
				}

				buffer->setCoordinates(coordinates);
				buffer->setTexCoords(0,texcoords);
				buffer->setTexCoords(1,texcoords);
				buffer->setNormals(normals);
				buffer->setIndices(indices);
				buffer->setPrimitiveType(TRIANGLES);
				buffer->reCalculateAABB();

				delete[] coordinates;
				delete[] texcoords;
				delete[] normals;
				delete[] indices;

				Mesh* mesh = new Mesh();
				mesh->addMeshBuffer(buffer);
                mesh->reCalculateAABB(); 
				return mesh;

			}

			static Mesh* createBox(sf32 length , sf32 width , sf32 height, bool bTemp = true)
			{
                sf32* coordinates = new sf32[6 * 4 * 3];
				sf32* texcoords   = new sf32[6 * 4 * 2];
				sf32* colors      = new sf32[6 * 4 * 3];
                sf32* normals     = new sf32[6 * 4 * 3];
                
				// front face
				coordinates[0] =  width/2 ; coordinates[1]  =  height/2 ; coordinates[2] = length/2;
                coordinates[3] = -width/2 ; coordinates[4]  =  height/2 ; coordinates[5] = length/2;
				coordinates[6] = -width/2 ; coordinates[7]  = -height/2 ; coordinates[8] = length/2;
				coordinates[9] =  width/2 ; coordinates[10] = -height/2 ; coordinates[11] = length/2;

				texcoords[0]   = 1.0f ; texcoords[1] = 1.0f;
				texcoords[2]   = 0.0f ; texcoords[3] = 1.0f;
				texcoords[4]   = 0.0f ; texcoords[5] = 0.0f;
				texcoords[6]   = 1.0f ; texcoords[7] = 0.0f;

				normals[0] =  0.0f ; normals[1]  =  0.0f; normals[2] = 1.0f;
                normals[3] =  0.0f ; normals[4]  =  0.0f ; normals[5] = 1.0f;
				normals[6] =  0.0f ; normals[7]  =  0.0f ; normals[8] = 1.0f;
				normals[9] =  0.0f ; normals[10] =  0.0f ; normals[11] = 1.0f;

				// back face
				coordinates[12] = -width/2 ; coordinates[13] =  height/2 ; coordinates[14] = -length/2;
                coordinates[15] =  width/2 ; coordinates[16] =  height/2 ; coordinates[17] = -length/2;
				coordinates[18] =  width/2 ; coordinates[19] = -height/2 ; coordinates[20] = -length/2;
				coordinates[21] = -width/2 ; coordinates[22] = -height/2 ; coordinates[23] = -length/2;

				texcoords[8]   = 0.0f ; texcoords[9] = 1.0f;
				texcoords[10]  = 1.0f ; texcoords[11] = 1.0f;
				texcoords[12]  = 1.0f ; texcoords[13] = 0.0f;
				texcoords[14]  = 0.0f ; texcoords[15] = 0.0f;

    			normals[12] =  0.0f ; normals[13] =  0.0f ; normals[14] = -1.0f;
                normals[15] =  0.0f ; normals[16] =  0.0f ; normals[17] = -1.0f;
				normals[18] =  0.0f ; normals[19] =  0.0f ; normals[20] = -1.0f;
				normals[21] =  0.0f ; normals[22] =  0.0f ; normals[23] = -1.0f;

				// left face
				coordinates[24] = -width/2 ; coordinates[25] = height/2 ; coordinates[26] =  length/2;
                coordinates[27] = -width/2 ; coordinates[28] = height/2 ; coordinates[29] =  -length/2;
				coordinates[30] = -width/2 ; coordinates[31] = -height/2 ; coordinates[32] = -length/2;
				coordinates[33] = -width/2 ; coordinates[34] = -height/2 ; coordinates[35] = length/2;

				texcoords[16]   = 1.0f ; texcoords[17] = 1.0f;
				texcoords[18]  = 1.0f ; texcoords[19] = 0.0f;
				texcoords[20]  = 0.0f ; texcoords[21] = 0.0f;
				texcoords[22]  = 0.0f ; texcoords[23] = 1.0f;

				normals[24] = -1.0f ; normals[25] = 0.0f ; normals[26] = 0.0f;
                normals[27] = -1.0f ; normals[28] = 0.0f ; normals[29] = 0.0f;
				normals[30] = -1.0f ; normals[31] = 0.0f ; normals[32] = 0.0f;
				normals[33] = -1.0f ; normals[34] = 0.0f ; normals[35] = 0.0f;

				// right face
				coordinates[36] =  width/2 ; coordinates[37] = height/2 ; coordinates[38] =  -length/2;
                coordinates[39] =  width/2 ; coordinates[40] = height/2 ; coordinates[41] =  length/2;
				coordinates[42] =  width/2 ; coordinates[43] = -height/2 ; coordinates[44] = length/2;
				coordinates[45] =  width/2 ; coordinates[46] = -height/2 ; coordinates[47] = -length/2;

				texcoords[24]   = 1.0f ; texcoords[25] = 0.0f;
				texcoords[26]  = 1.0f ; texcoords[27] = 1.0f;
				texcoords[28]  = 0.0f ; texcoords[29] = 1.0f;
				texcoords[30]  = 0.0f ; texcoords[31] = 0.0f;

				normals[36] =  1.0f ; normals[37] = 0.0f ; normals[38] = 0.0f;
                normals[39] =  1.0f ; normals[40] = 0.0f ; normals[41] = 0.0f;
				normals[42] =  1.0f ; normals[43] = 0.0f ; normals[44] = 0.0f;
				normals[45] =  1.0f ; normals[46] = 0.0f ; normals[47] = 0.0f;

				// top face
				coordinates[48] =  -width/2 ; coordinates[49] = height/2 ; coordinates[50] =  -length/2;
                coordinates[51] =  -width/2 ; coordinates[52] = height/2 ; coordinates[53] =  length/2;
				coordinates[54] =  width/2 ;  coordinates[55] = height/2 ; coordinates[56] = length/2;
				coordinates[57] =  width/2 ;  coordinates[58] = height/2 ; coordinates[59] = -length/2;

				texcoords[32]  = 0.0f ; texcoords[33] = 0.0f;
				texcoords[34]  = 0.0f ; texcoords[35] = 1.0f;
				texcoords[36]  = 1.0f ; texcoords[37] = 1.0f;
				texcoords[38]  = 1.0f ; texcoords[39] = 0.0f;

				normals[48] =  0.0f ; normals[49] = 1.0f ; normals[50] =  0.0f;
                normals[51] =  0.0f ; normals[52] = 1.0f ; normals[53] =  0.0f;
				normals[54] =  0.0f ; normals[55] = 1.0f ; normals[56] =  0.0f;
				normals[57] =  0.0f ; normals[58] = 1.0f ; normals[59] =  0.0f;

				// bottom face
				coordinates[60] =  -width/2 ; coordinates[61] = -height/2 ; coordinates[62] =  length/2;
                coordinates[63] =  -width/2 ; coordinates[64] = -height/2 ; coordinates[65] =  -length/2;
				coordinates[66] =  width/2 ;  coordinates[67] = -height/2 ; coordinates[68] = -length/2;
				coordinates[69] =  width/2 ;  coordinates[70] = -height/2 ; coordinates[71] = length/2;

				texcoords[40]  = 0.0f ; texcoords[41] = 1.0f;
				texcoords[42]  = 0.0f ; texcoords[43] = 0.0f;
				texcoords[44]  = 1.0f ; texcoords[45] = 0.0f;
				texcoords[46]  = 1.0f ; texcoords[47] = 1.0f;

				normals[60] =  0.0f ; normals[61] = -1.0f ; normals[62] =  0.0f;
                normals[63] =  0.0f ; normals[64] = -1.0f ; normals[65] =  0.0f;
				normals[66] =  0.0f ;  normals[67] = -1.0f ; normals[68] = 0.0f;
				normals[69] =  0.0f ;  normals[70] = -1.0f ; normals[71] = 0.0f;

				for ( si32 i = 0 ; i < 72 ; i+=3 )
				{
					colors[i] = 0.8f;
					colors[i+1] = 0.8f;
					colors[i+2] = 0.8f;
				}

				SMeshBuffer* buffer;
				int vaSize[] = {3,3,3};
				if( bTemp )
					buffer = new SMeshBuffer( 24 , COORDINATES | COLOR_3 | TEXTURE_COORDINATE_2 | NORMALS , 2);
				
				if( !bTemp )
					buffer = new SMeshBuffer( 24 , COORDINATES | COLOR_3 | TEXTURE_COORDINATE_2 | NORMALS | VERTEX_ATTRIBUTES, 2, 3,vaSize);

				buffer->setCoordinates(coordinates);
				buffer->setNormals(normals);
				buffer->setColors(colors);
				buffer->setTexCoords(0,texcoords);
				buffer->setTexCoords(1,texcoords);

				delete[] coordinates;
				delete[] normals;
				delete[] colors;
				delete[] texcoords;

				buffer->setPrimitiveType(QUADS);
				buffer->reCalculateAABB();

				Mesh* mesh = new Mesh();
				mesh->addMeshBuffer(buffer);
				mesh->reCalculateAABB();
				return mesh;

			}
			static Mesh* createCylinder( sf32 length, sf32 radius, su32 numVerticalSegments, su32 numCircularSegments)
			{
				return 0;
			}
		};

		
	}

}

#endif