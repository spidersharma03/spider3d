#ifndef _TBN_GENERATOR_H
#define _TBN_GENERATOR_H

#include "IndexedMeshBuffer.h"
#include "Mesh.h"

namespace spider3d
{
	namespace effects
	{

		class TBNGenerator
		{
		public:

			static sbool generateTBN( Mesh* mesh )
			{
				for( int i=0; i<mesh->getNumMeshBuffers(); i++ )
					return generateTBN( &mesh->getMeshBuffer(i) );

				return true;
			}

			static sbool generateTBN( MeshBuffer* meshBuffer )
			{
				sbool bIndexed = meshBuffer->isIndexed();
				PRIMITIVE_TYPE pType = meshBuffer->getPrimitiveType();
				if ( bIndexed )
				{
					if(  pType == TRIANGLES )
					{
						return generateTBNforIndexedTriangles(meshBuffer);
					}
					if(  pType == QUADS )
					{
						return generateTBNforIndexedQuads(meshBuffer);
					}
				}
				else
				{
					if(  pType == TRIANGLES )
					{
						return generateTBNforTriangles(meshBuffer);
					}
					if(  pType == QUADS )
					{
						return generateTBNforQuads(meshBuffer);
					}
				}
				return false;
			}

		private:
			static sbool generateTBNforTriangles(MeshBuffer* meshBuffer)
			{

				return false;
			}
			static sbool generateTBNforIndexedTriangles(MeshBuffer* meshBuffer)
			{
				SIndexedMeshBuffer *indexedBuffer = (SIndexedMeshBuffer*)meshBuffer;
				su32 indexCount = indexedBuffer->getIndexCount();
				sf32* coords = indexedBuffer->getCoordinates();
				su16* indices = indexedBuffer->getIndices();
				si32 vaCount  = indexedBuffer->getVertexAttributeCount();
				si32* vaSizes = indexedBuffer->getVertexAttributeSizes();
				if( vaCount < 3 )
					return false;
				if( vaSizes[0] < 3 || vaSizes[1] < 3 || vaSizes[2] < 3 )
					return false;

				vector<ShaderVertexAttributeSet*>& vecShaderVertexAttributes = indexedBuffer->getAllShaderVertexAttributes();
				sf32* TangentData = vecShaderVertexAttributes[0]->m_ShaderVertexAttributePointer;
				sf32* NormalData = vecShaderVertexAttributes[1]->m_ShaderVertexAttributePointer;
				sf32* BiNormalData = vecShaderVertexAttributes[2]->m_ShaderVertexAttributePointer;

				for ( int i = 0 ; i < indexCount; i += 3 )
				{
					int index0 = 3*indices[i];
					int index1 = 3*indices[i+1];
					int index2 = 3*indices[i+2];

					Vector3 A;
					Vector3 B;
					Vector3 C;
					Vector3 AB, AC;
					Vector3 N;

					A.set(coords[index0], coords[index0+1], coords[index0+2]);
					B.set(coords[index1], coords[index1+1], coords[index1+2]);
					C.set(coords[index2], coords[index2+1], coords[index2+2]);

					AB.sub(A,B);
					AC.sub(A,C); // Tangent
					AC.normalize();

					N = AB.cross(AC); // Normal
					N.normalize();
					AB = AC.cross(N); // BiNormal

					TangentData[index0] = AC.x; TangentData[index0+1] = AC.y; TangentData[index0+2] = AC.z; 
					TangentData[index1] = AC.x; TangentData[index1+1] = AC.y; TangentData[index1+2] = AC.z; 
					TangentData[index2] = AC.x; TangentData[index2+1] = AC.y; TangentData[index2+2] = AC.z; 

					NormalData[index0] = N.x; NormalData[index0+1] = N.y; NormalData[index0+2] = N.z; 
					NormalData[index1] = N.x; NormalData[index1+1] = N.y; NormalData[index1+2] = N.z; 
					NormalData[index2] = N.x; NormalData[index2+1] = N.y; NormalData[index2+2] = N.z; 

					BiNormalData[index0] = AB.x; BiNormalData[index0+1] = AB.y; BiNormalData[index0+2] = AB.z; 
					BiNormalData[index1] = AB.x; BiNormalData[index1+1] = AB.y; BiNormalData[index1+2] = AB.z; 
					BiNormalData[index2] = AB.x; BiNormalData[index2+1] = AB.y; BiNormalData[index2+2] = AB.z; 
				}
				return true;
			}

			static sbool generateTBNforQuads(MeshBuffer* meshBuffer)
			{
				sf32* coords = meshBuffer->getCoordinates();
				si32 vaCount  = meshBuffer->getVertexAttributeCount();
				si32* vaSizes = meshBuffer->getVertexAttributeSizes();
				if( vaCount < 3 )
					return false;
				if( vaSizes[0] < 3 || vaSizes[1] < 3 || vaSizes[2] < 3 )
					return false;

				vector<ShaderVertexAttributeSet*>& vecShaderVertexAttributes = meshBuffer->getAllShaderVertexAttributes();
				sf32* TangentData = vecShaderVertexAttributes[0]->m_ShaderVertexAttributePointer;
				sf32* NormalData = vecShaderVertexAttributes[1]->m_ShaderVertexAttributePointer;
				sf32* BiNormalData = vecShaderVertexAttributes[2]->m_ShaderVertexAttributePointer;

				for ( int i = 0 ; i < meshBuffer->getVertexCount(); i += 4 )
				{
					int index0 = 3*i;
					int index1 = 3*(i+1);
					int index2 = 3*(i+2);
					int index3 = 3*(i+3);

					Vector3 A;
					Vector3 B;
					Vector3 C;
					Vector3 AB, AC;
					Vector3 N;

					A.set(coords[index0], coords[index0+1], coords[index0+2]);
					B.set(coords[index1], coords[index1+1], coords[index1+2]);
					C.set(coords[index2], coords[index2+1], coords[index2+2]);

					AB.sub(A,B);
					AC.sub(A,C); // Tangent
					AC.normalize();

					N = AB.cross(AC); // Normal
					N.normalize();
					AB = AC.cross(N); // BiNormal
					
					/*AB.scale(-1.0f);
					N.scale(-1.0f);
					AC.scale(-1.0f);*/

					TangentData[index0] = AC.x; TangentData[index0+1] = AC.y; TangentData[index0+2] = AC.z; 
					TangentData[index1] = AC.x; TangentData[index1+1] = AC.y; TangentData[index1+2] = AC.z; 
					TangentData[index2] = AC.x; TangentData[index2+1] = AC.y; TangentData[index2+2] = AC.z; 
					TangentData[index3] = AC.x; TangentData[index3+1] = AC.y; TangentData[index3+2] = AC.z; 

					NormalData[index0] = N.x; NormalData[index0+1] = N.y; NormalData[index0+2] = N.z; 
					NormalData[index1] = N.x; NormalData[index1+1] = N.y; NormalData[index1+2] = N.z; 
					NormalData[index2] = N.x; NormalData[index2+1] = N.y; NormalData[index2+2] = N.z; 
					NormalData[index3] = N.x; NormalData[index3+1] = N.y; NormalData[index3+2] = N.z; 

					BiNormalData[index0] = AB.x; BiNormalData[index0+1] = AB.y; BiNormalData[index0+2] = AB.z; 
					BiNormalData[index1] = AB.x; BiNormalData[index1+1] = AB.y; BiNormalData[index1+2] = AB.z; 
					BiNormalData[index2] = AB.x; BiNormalData[index2+1] = AB.y; BiNormalData[index2+2] = AB.z; 
					BiNormalData[index3] = AB.x; BiNormalData[index3+1] = AB.y; BiNormalData[index3+2] = AB.z; 
				}
				return true;
			}

			static sbool generateTBNforIndexedQuads(MeshBuffer* meshBuffer)
			{
				SIndexedMeshBuffer *indexedBuffer = (SIndexedMeshBuffer*)meshBuffer;
				su32 indexCount = indexedBuffer->getIndexCount();
				sf32* coords = indexedBuffer->getCoordinates();
				su16* indices = indexedBuffer->getIndices();
				si32 vaCount  = indexedBuffer->getVertexAttributeCount();
				si32* vaSizes = indexedBuffer->getVertexAttributeSizes();
				if( vaCount < 3 )
					return false;
				if( vaSizes[0] < 3 || vaSizes[1] < 3 || vaSizes[2] < 3 )
					return false;

				vector<ShaderVertexAttributeSet*>& vecShaderVertexAttributes = indexedBuffer->getAllShaderVertexAttributes();
				sf32* TangentData = vecShaderVertexAttributes[0]->m_ShaderVertexAttributePointer;
				sf32* NormalData = vecShaderVertexAttributes[1]->m_ShaderVertexAttributePointer;
				sf32* BiNormalData = vecShaderVertexAttributes[2]->m_ShaderVertexAttributePointer;

				for ( int i = 0 ; i < indexCount; i += 4 )
				{
					int index0 = 3*indices[i];
					int index1 = 3*indices[i+1];
					int index2 = 3*indices[i+2];
					int index3 = 3*indices[i+3];

					Vector3 A;
					Vector3 B;
					Vector3 C;
					Vector3 AB, AC;
					Vector3 N;

					A.set(coords[index0], coords[index0+1], coords[index0+2]);
					B.set(coords[index1], coords[index1+1], coords[index1+2]);
					C.set(coords[index2], coords[index2+1], coords[index2+2]);

					AB.sub(A,B);
					AC.sub(A,C); // Tangent
					AC.normalize();

					N = AB.cross(AC); // Normal
					N.normalize();
					AB = AC.cross(N); // BiNormal

					TangentData[index0] = AC.x; TangentData[index0+1] = AC.y; TangentData[index0+2] = AC.z; 
					TangentData[index1] = AC.x; TangentData[index1+1] = AC.y; TangentData[index1+2] = AC.z; 
					TangentData[index2] = AC.x; TangentData[index2+1] = AC.y; TangentData[index2+2] = AC.z; 
					TangentData[index3] = AC.x; TangentData[index3+1] = AC.y; TangentData[index3+2] = AC.z; 

					NormalData[index0] = N.x; NormalData[index0+1] = N.y; NormalData[index0+2] = N.z; 
					NormalData[index1] = N.x; NormalData[index1+1] = N.y; NormalData[index1+2] = N.z; 
					NormalData[index2] = N.x; NormalData[index2+1] = N.y; NormalData[index2+2] = N.z; 
					NormalData[index3] = N.x; NormalData[index3+1] = N.y; NormalData[index3+2] = N.z;

					BiNormalData[index0] = AB.x; BiNormalData[index0+1] = AB.y; BiNormalData[index0+2] = AB.z; 
					BiNormalData[index1] = AB.x; BiNormalData[index1+1] = AB.y; BiNormalData[index1+2] = AB.z; 
					BiNormalData[index2] = AB.x; BiNormalData[index2+1] = AB.y; BiNormalData[index2+2] = AB.z; 
					BiNormalData[index3] = AB.x; BiNormalData[index3+1] = AB.y; BiNormalData[index3+2] = AB.z; 
				}
				return true;
			}

			/*static bool checkForValidTBNSupport()
			{
			}*/
		};

	}
}
#endif