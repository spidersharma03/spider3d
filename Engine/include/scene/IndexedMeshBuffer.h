#ifndef _INDEXEDMESHBUFFER_H

#define _INDEXEDMESHBUFFER_H

#include "MeshBuffer.h"

namespace spider3d
{
	namespace scene
	{
		class IndexedMeshBuffer:public MeshBuffer
		{
		public:

			virtual void setIndices(su16*) = 0;

			virtual su16* getIndices() const = 0;

			virtual su32 getIndexCount() const = 0;

			virtual su32 getPrimitiveCount() const = 0;

			sbool isIndexed() const
		    {
			  return true;
		    }

			IndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 indexCount ):MeshBuffer(vertexCount , vertexFormat),indexCount(indexCount)
			{
			}

			IndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet, su32 indexCount ):MeshBuffer(vertexCount , vertexFormat , numTexCoordSet),indexCount(indexCount)
			{
			}
		
			IndexedMeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet, si32 vertexAttributeCount, si32* vertexAttributeSizes, su32 indexCount ):MeshBuffer(vertexCount , vertexFormat , numTexCoordSet, vertexAttributeCount, vertexAttributeSizes),indexCount(indexCount)
			{
			}

			virtual ~IndexedMeshBuffer()
			{
			}

		protected:
   			su32 indexCount;

		};


	}

}

#endif