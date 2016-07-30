#ifndef _RENDERER_H

#define _RENDERER_H

#include "IndexedMeshBuffer.h"
#include "coreMath.h"
#include "Rectangle.h"
#include "AABB.h"
#include "LightData.h"
#include "Texture2D.h"
#include "TextureCubeMap.h"
#include "TextureUnitState.h"

namespace spider3d
{
	namespace appearance
	{
		class Appearance;
		class ShaderAppearance;
	}
}
using namespace spider3d::scene;
using namespace spider3d::appearance;

namespace spider3d
{
	enum RENDERER_TYPE { OPENGL = 0 , D3D = 1 };

	enum TANSFORMATION_STATE { WORLD = 0 , VIEW , PROJECTION , TEXTURE };

	enum BUFFER_BIT { COLOR_BUFFER_BIT = 0x01 , DEPTH_BUFFER_BIT = 0x02 , STENCIL_BUFFER_BIT = 0x04};

	namespace render
	{
		class FrameBufferObject;

		class Renderer
		{
		public:

			virtual void startScene(Color4&) = 0;

			virtual void startScene(Color4&, int) = 0;

			virtual void endScene() = 0;

			virtual void clearBuffers(su32) = 0;

			virtual void DrawPoint(Point3& , Color3&) = 0;

			virtual void DrawLine(Point3& , Point3& , Color3&) = 0;

			virtual void DrawRect(Rectangle& , Color3&) = 0;

			virtual void DrawBox(AABB& , Color3&) = 0;

			virtual void drawIndexedMeshBuffer( IndexedMeshBuffer& ) = 0;

			virtual void drawMeshBuffer( MeshBuffer& ) = 0;

			virtual void setTransform(Matrix4f& , TANSFORMATION_STATE ) = 0;

			virtual void addLight( LightData& ) = 0;

			virtual void setViewPort(sf32 left , sf32 top , sf32 width , sf32 height) = 0;

			virtual Rectangle& getViewPort() = 0;

			virtual void resize(sf32 , sf32 ) = 0;

			virtual void setAppearance(Appearance*) = 0;

			virtual su32 createTextureObject() = 0;

			virtual void setTextureObject(Texture*) = 0;

			virtual void updateTexture(Rectangle&, const void* data, Texture*) = 0;

			virtual void copyBufferToTexture(Rectangle& ,Texture*) = 0;

			virtual void copyBufferToTextureCubeMap(Rectangle& ,Texture*, CUBE_MAP_FACE) = 0;

			virtual void setRenderTargetEnable(FrameBufferObject*, sbool) = 0;

			virtual FrameBufferObject* createRenderTargetDepthTexture2D(su32 width, su32 height) = 0;

			virtual FrameBufferObject* createRenderTargetTexture2D(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer) = 0;
		
			virtual FrameBufferObject* createRenderTargetTextureCubeMap(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer) = 0;

		protected:
			virtual void setRenderState(Appearance*) = 0;
						
		};

		enum BUFFER_ATTACHMENT_TYPE { EBAT_COLOR, EBAT_DEPTH, EBAT_STENCIL };
		enum COLOR_ATTACHMENT_POINT {ECAP0, ECAP1, ECAP2, ECAP3 };
		
		class FrameBufferObject
		{			 
		public:
			FrameBufferObject(su32 width, su32 height);
			Texture* getFboTexture();
			TextureUnitState* getFboTextureUnitState();
			void attachTexture(Texture*, COLOR_ATTACHMENT_POINT colorAttchment = ECAP0);
			void setColorAttachmentPoint(COLOR_ATTACHMENT_POINT colorAttachment);
			void setBufferAttachmentType(BUFFER_ATTACHMENT_TYPE);
			void attachMRTArray(si32, COLOR_ATTACHMENT_POINT*);
			void getFrameBufferObjectName(unsigned*);
			virtual void onSet() = 0;
			virtual void unSet() = 0;
		protected:
			unsigned frameBufferName;
			unsigned depthBufferName;
			unsigned stencilBufferName;
			unsigned colorBuferName;
			unsigned fboTextureName;
			int      fboHeight, fboWidth;
			sbool    bEnable;
			Texture* fboTexture;
			TextureUnitState* fboTextureUnit;
			COLOR_ATTACHMENT_POINT m_ColorAttachmentPoint;
			sbool m_bMRT_Enabled;
			BUFFER_ATTACHMENT_TYPE m_BufferAttachmentType;
		};

		class FrameBufferObjectTexture_2D: public FrameBufferObject
		{
		public:
			FrameBufferObjectTexture_2D(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer);
			virtual void onSet();
			virtual void unSet();
		};

		class FrameBufferObjectTexture_CubeMap: public FrameBufferObject
		{
		public:
			FrameBufferObjectTexture_CubeMap(su32 width, su32 height, sbool depthBuffer, sbool stencilBuffer);
			void setCurrentFace(CUBE_MAP_FACE);
			virtual void onSet();
			virtual void unSet();
		private:
			CUBE_MAP_FACE currentFace;
		};

		class FrameBufferObjectTexture_Depth: public FrameBufferObject
		{
		public:
			FrameBufferObjectTexture_Depth(su32 width, su32 height);
			virtual void onSet();
			virtual void unSet();
		};

	}
}
#endif 