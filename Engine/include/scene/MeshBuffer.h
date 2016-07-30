#ifndef _MESHBUFFER_H

#define _MESHBUFFER_H

#include<stdio.h>
#include "spiderDataTypes.h"
#include "AABB.h"
#include "MeshBufferUpdater.h"
#include <vector>

using namespace std;
using namespace spider3d::core;

namespace spider3d
{
	enum PRIMITIVE_TYPE
	{
		EPT_POINTS = 0 , LINES , TRIANGLES , TRIANGLEFANS , QUADS , POLYGON
	};

	enum VERTEX_FORMAT
	{
		COORDINATES = 0x01 , NORMALS = 0x02 , COLOR_3 = 0x04 , COLOR_4 = 0x04 | 0x08 , TEXTURE_COORDINATE_2 = 0x20
		, TEXTURE_COORDINATE_3 = 0x40, TEXTURE_COORDINATE_4 = 0x400 , VERTEX_ATTRIBUTES = 0x1000
	};

	enum GEOMETRY_RENDERING_MODE
	{
		EGRM_VERTEX_ARRAYS, EGRM_DISPLAY_LIST, EGRM_VBO
	};

	namespace scene
	{
		struct ShaderVertexAttributeSet
		{
			// Attribute Location in the Shader Program
			int m_Location;
			// Pointer to the Attribute Data
			sf32 *m_ShaderVertexAttributePointer;
			// Attribute type
			int m_Type;
			// Attribute Size(1/2/3/4)
			int m_Size;

			ShaderVertexAttributeSet()
			{
				m_Location = -1;
				m_ShaderVertexAttributePointer = 0;
				m_Size = 1;
			}
		};

		class MeshBuffer
		{
		public:

			virtual sf32* getCoordinates() const = 0;

			virtual su32 getVertexCount() const = 0;

			virtual sf32* getNormals() const = 0;

			virtual sf32* getColors() const = 0;

			virtual sf32* getTexCoords() const = 0;

			virtual vector<sf32*>& getAllTexCoords() = 0;

			virtual sf32* getTexCoords(su32 texCoordSet) const = 0;

			virtual vector<sf32*>& getAllVertexAttributes() = 0;

			virtual vector<ShaderVertexAttributeSet*>& getAllShaderVertexAttributes() = 0;

			virtual sbool isIndexed() const = 0;

			virtual void setCoordinates(sf32*) = 0;

			virtual void setNormals(sf32*) = 0;

			virtual void setColors(sf32*) = 0;

			virtual void setTexCoords(sf32*) = 0;

			virtual void setTexCoords(su32 , sf32*) = 0;

			virtual void setVertexAttributes( su32, sf32*) = 0;

			virtual PRIMITIVE_TYPE getPrimitiveType() const = 0;

			virtual AABB& getAABB() = 0;

			virtual void setAABB(AABB&) = 0;

			virtual void reCalculateAABB() = 0;

			virtual void setPrimitiveType(PRIMITIVE_TYPE) = 0;

			MeshBuffer(su32 vertexCount , su32 vertexFormat):vertexCount(vertexCount),vertexFormat(vertexFormat)
			{
				numTexCoordSet = 0;
			}

			MeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet ):vertexCount(vertexCount),vertexFormat(vertexFormat),numTexCoordSet(numTexCoordSet)
			{
			}

			MeshBuffer(su32 vertexCount , su32 vertexFormat , su32 numTexCoordSet, si32 vertexAttributeCount, si32* vertexAttributeSizes ):vertexCount(vertexCount),vertexFormat(vertexFormat),numTexCoordSet(numTexCoordSet),
				m_vertexAttributeCount(vertexAttributeCount)
			{
				m_vertexAttributeSizes = new si32[m_vertexAttributeCount];
				for ( int i=0; i<vertexAttributeCount; i++)
					m_vertexAttributeSizes[i] = vertexAttributeSizes[i]; 
			}

			inline si32 getVertexSize()
			{
				return vertexSize;
			}

			inline si32 getColorSize()
			{
				return colorSize;
			}

			inline si32 getTexCoordSize()
			{
				return texCoordSize;
			}

			void updateMeshBuffer(MeshBufferUpdater* meshBufferUpdater)
			{
				if( meshBufferUpdater)
					meshBufferUpdater->updateMeshBuffer(this);
			}

			void setGeometryRenderingMode(GEOMETRY_RENDERING_MODE geometryRenderingMode)
			{
				m_GeometryRenderingMode = geometryRenderingMode;
			}
		
			GEOMETRY_RENDERING_MODE getGeometryRenderingMode()
			{
				return m_GeometryRenderingMode;
			}

			virtual ~MeshBuffer(){}

			si32 getVertexAttributeCount()
			{
				return m_vertexAttributeCount;
			}

			si32* getVertexAttributeSizes()
			{
				return m_vertexAttributeSizes;
			}

		protected:
			GEOMETRY_RENDERING_MODE m_GeometryRenderingMode;
			PRIMITIVE_TYPE type;
			su32 vertexCount;
			su32 vertexFormat;
			su32 numTexCoordSet;
			si32 vertexSize, colorSize, texCoordSize;
			si32 m_vertexAttributeCount;
			si32* m_vertexAttributeSizes;
		};

	}

}

#endif