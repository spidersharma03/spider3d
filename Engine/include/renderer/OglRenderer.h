#ifndef _OGLRENDERER_H

#define _OGLRENDERER_H

#include "Renderer.h"
#include "spiderDisplayParams.h"

#ifdef WIN32
#include <windows.h>
#endif
//#include <GL/gl.h>
//#include <GL/glext.h>

namespace spider3d
{
	namespace scene
	{
		class CameraNode;
	}
}

namespace spider3d
{
	class DeviceContext;
}

namespace spider3d
{
	namespace render
	{		
		class OglRenderer:public Renderer
		{
		public:

			static Renderer* getSingleton(DisplayParams&) ;

			virtual void DrawPoint(Point3& , Color3&) ;

			void DrawLine(Point3& , Point3& , Color3&) ;

			void DrawRect(Rectangle& , Color3&);

			void DrawBox(AABB& , Color3&) ;

			void drawIndexedMeshBuffer(IndexedMeshBuffer&);

			void drawMeshBuffer(MeshBuffer&);

			void addLight(LightData& lightData);

			void setTransform(Matrix4f& , TANSFORMATION_STATE);

			void startScene(Color4&);

			void startScene(Color4&, int);

			void endScene();

			virtual void clearBuffers(su32);

			void drawIndexedVertexPrimitiveList
				(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize, sf32* texcoords, si32 texCoordSize, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type);

			void drawIndexedVertexPrimitiveList
				(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize,  vector<sf32*>& texcoords, si32 texCoordSize, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type);

			void drawIndexedVertexPrimitiveList
				(su32 vertexCount, sf32* coords, si32 vertexSize, sf32* normals , sf32* colors, si32 colorSize,  vector<sf32*>& texcoords, si32 texCoordSize, vector<ShaderVertexAttributeSet*>& shaderVertexAttributes, su16* indices, su32 primitiveCount ,  PRIMITIVE_TYPE type);

			void drawVertexPrimitiveList(su32 vertexCount, void* coords , si32 vertexSize , sf32* normals , void* colors, si32 colorSize, sf32* texcoords, si32 texCoordSize, si32 stride, PRIMITIVE_TYPE type);

			void drawVertexPrimitiveList(su32 vertexCount, sf32* coords , si32 vertexSize , sf32* normals , sf32* colors, si32 colorSize, vector<sf32*>& texcoords, si32 texCoordSize, si32 stride, PRIMITIVE_TYPE type);

			void drawVertexPrimitiveList(su32 vertexCount, sf32* coords , si32 vertexSize , sf32* normals , sf32* colors, si32 colorSize, vector<sf32*>& texcoords, si32 texCoordSize, vector<ShaderVertexAttributeSet*>& shaderVertexAttributes, PRIMITIVE_TYPE type);

			virtual void setViewPort(sf32 left , sf32 top , sf32 width , sf32 height) ;

			virtual Rectangle& getViewPort() ;

			virtual void resize(sf32 , sf32 ) ;

			virtual void setAppearance(Appearance*);

			virtual void setRenderState(Appearance*); 

			virtual su32 createTextureObject();

			virtual void setTextureObject(Texture*);

			virtual void setTextureObject(TextureUnitState*);

			virtual void updateTexture(Rectangle&, const void* data, Texture*);

			virtual void copyBufferToTexture(Rectangle& ,Texture*);

			virtual void copyBufferToTextureCubeMap(Rectangle& ,Texture*, CUBE_MAP_FACE);

			//virtual si32 createDisplayList();

			//virtual void drawDisplayList(si32);

			virtual void setRenderTargetEnable(FrameBufferObject*, sbool);

			virtual FrameBufferObject* createRenderTargetDepthTexture2D(su32 width, su32 height);

			virtual FrameBufferObject* createRenderTargetTexture2D(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer);

			virtual FrameBufferObject* createRenderTargetTextureCubeMap(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer);
			
			void setCamera( CameraNode* camera )
			{
				this->camera = camera;
			}
		protected:

			OglRenderer(DisplayParams&);

			void initRenderer(DisplayParams&);

			void setTextureCobineAttributes(TextureAttributes*,int);

			static OglRenderer* oglRenderer;

			ss16 LightCount;

			//HDC deviceContext;

			//HWND WindowID;

			//HGLRC OpenglContext;

			Rectangle ViewPort;

			Appearance* previousAppearance;

			Appearance* currentAppearance;

			CameraNode* camera;

			FrameBufferObject* fboObject;
			
			DeviceContext *m_pContext;
		};
	}

}

#endif