#include "LensFlareNode.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "FileLoader.h"
#include "Appearance.h"
#include "OglRenderer.h"

using namespace spider3d::io;

#define ADD_COORDS_TO_VECTOR(V,W,H) \
V.push_back(-W*0.5f);V.push_back(-H*0.5f); \
V.push_back(-W*0.5f);V.push_back(H*0.5f); \
V.push_back(W*0.5f);V.push_back(H*0.5f); \
V.push_back(W*0.5f);V.push_back(-H*0.5f); \

#define ADD_COLORS_TO_VECTOR(V,R,G,B,A) \
V.push_back(R);V.push_back(G);V.push_back(B);V.push_back(A); \
V.push_back(R);V.push_back(G);V.push_back(B);V.push_back(A); \
V.push_back(R);V.push_back(G);V.push_back(B);V.push_back(A); \
V.push_back(R);V.push_back(G);V.push_back(B);V.push_back(A); \

#define MODIFY_ALPHA(V,A) \
V[3] = A; \
V[7] = A; \
V[11] = A; \
V[15] = A; \

#define ADD_TEX_COORDS_TO_VECTOR(V) \
V.push_back(0.0f);V.push_back(0.0f); \
V.push_back(0.0f);V.push_back(1.0f); \
V.push_back(1.0f);V.push_back(1.0f); \
V.push_back(1.0f);V.push_back(0.0f); \


namespace spider3d
{
	namespace scene
	{
		LensFlareNode::LensFlareNode( Nodeptr nodePtr, SceneManagerptr smgr, Point3 lightPosition)
			:Node( nodePtr, smgr ),m_LightPosition(lightPosition)
		{
			//sceneManager->getActiveCamera()->addChild(this);
			TextureLoader *textureLoader1 = new TextureLoader("particlewhite.bmp",0);
			TextureLoader *textureLoader2 = new TextureLoader("flare1.png",0);
			TextureLoader *textureLoader3 = new TextureLoader("portal1.bmp",0);
			TextureLoader *textureLoader4 = new TextureLoader("HardGlow2.bmp",0);
			TextureLoader *textureLoader5 = new TextureLoader("BigGlow3.bmp",0);

			m_App1 = new Appearance();
			TextureUnitState *texUnit1 = new TextureUnitState(textureLoader1->getTexture());
			texUnit1->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
			m_App1->setTextureUnitState(0,texUnit1);
			m_App1->getDepthAttributes().DepthTest = false;
			m_App1->getMaterial()._mLightingEnabled = false;
			m_App1->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
			m_App1->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
			m_App1->getTransparencyAttributes().mode = ETM_BLEND;
			m_App1->getTransparencyAttributes().transparency = 0.01f;

			m_App2 = new Appearance();
			TextureUnitState *texUnit2 = new TextureUnitState(textureLoader2->getTexture());
			m_App2->setTextureUnitState(0,texUnit2);
			m_App2->getDepthAttributes().DepthTest = false;
			m_App2->getDepthAttributes().DepthTest = false;
			m_App2->getMaterial()._mLightingEnabled = false;
			m_App2->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
			m_App2->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
			m_App2->getTransparencyAttributes().mode = ETM_BLEND;
			m_App2->getTransparencyAttributes().transparency = 0.01f;

			m_App3 = new Appearance();
			TextureUnitState *texUnit3 = new TextureUnitState(textureLoader3->getTexture());
			m_App3->setTextureUnitState(0,texUnit3);
			m_App3->getDepthAttributes().DepthTest = false;
			m_App3->getMaterial()._mLightingEnabled = false;
			m_App3->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
			m_App3->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
			m_App3->getTransparencyAttributes().mode = ETM_BLEND;
			m_App3->getTransparencyAttributes().transparency = 0.01f;

			m_App4 = new Appearance();
			TextureUnitState *texUnit4 = new TextureUnitState(textureLoader4->getTexture());
			m_App4->setTextureUnitState(0,texUnit4);
			m_App4->getDepthAttributes().DepthTest = false;
			m_App4->getMaterial()._mLightingEnabled = false;
			m_App4->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
			m_App4->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
			m_App4->getTransparencyAttributes().mode = ETM_BLEND;
			m_App4->getTransparencyAttributes().transparency = 0.51f;

			m_App5 = new Appearance();
			TextureUnitState *texUnit5 = new TextureUnitState(textureLoader5->getTexture());
			m_App5->setTextureUnitState(0,texUnit5);
			m_App5->getDepthAttributes().DepthTest = false;
			m_App5->getMaterial()._mLightingEnabled = false;
			m_App5->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
			m_App5->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
			m_App5->getTransparencyAttributes().mode = ETM_BLEND;
			m_App5->getTransparencyAttributes().transparency = 0.51f;

			createGeometry();
		}

	
		void LensFlareNode::createGeometry()
		{
			int width = 60.0f;
			int height = 60.0f;
			ADD_COORDS_TO_VECTOR(m_vecCoords1,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors1,155,155,200,255)
			ADD_TEX_COORDS_TO_VECTOR(m_vecTexCoords1)

			width = 55.0f;
			height = 55.0f;
			ADD_COORDS_TO_VECTOR(m_vecCoords2,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors2,155,155,200,255)

			// All Geometry for Halo
			sf32 f = 10.0f;
			width = 3.0f *f;
			height = 3.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords4,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors4,5,55,255,255)

			width = 1.5f*f;
			height = 1.5f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords5,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors5,155,5,115,255)

			width = 3.0f*f;
			height = 3.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords6,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors6,155,155,5,255)

			width = 2.0f*f;
			height = 2.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords7,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors7,155,5,5,255)

			width = 2.0f*f;
			height = 2.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords8,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors8,5,155,5,255)

			width = 8.0f*f;
			height = 8.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords9,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors9,155,155,5,255)

			// All Geometry for Glow
			width = 5.0f*f;
			height = 5.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords10,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors10,200,200,255,255)

			width = 1.0f*f;
			height = 1.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords11,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors11,255,55,55,255)

			width = 1.3f*f;
			height = 1.3f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords12,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors12,55,55,255,255)

			width = 1.6f*f;
			height = 1.6f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords13,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors13,55,55,85,255)

			width = 3.0f*f;
			height = 3.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords14,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors14,155,255,55,255)

			width = 2.3f*f;
			height = 2.3f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords15,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors15,155,255,255,255)

			width = 1.0f*f;
			height = 1.0f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords16,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors16,155,155,255,255)

			width = 7.3f*f;
			height = 7.3f*f;
			ADD_COORDS_TO_VECTOR(m_vecCoords17,width,height)
			ADD_COLORS_TO_VECTOR(m_vecColors17,255,255,255,255)
		}

		void LensFlareNode::render()
		{
			Matrix4f billBoardTransform;
			
			Point3 currLightPos = m_LightPosition;
			globalTransform.transform(currLightPos);
			Matrix4f& camView = sceneManager->getActiveCamera()->getViewMatrix();
			computeBillBoard(currLightPos,billBoardTransform);
			sceneManager->getRenderer()->setAppearance(m_App1);
			renderFlare(billBoardTransform, camView, currLightPos, m_vecCoords1, m_vecTexCoords1, m_vecColors1, 255);
	
			sceneManager->getRenderer()->setAppearance(m_App2);
			renderFlare(billBoardTransform, camView, currLightPos, m_vecCoords2, m_vecTexCoords1, m_vecColors2, 255);

			//sceneManager->getRenderer()->setAppearance(m_App5);
			//renderFlare(billBoardTransform, camView, currLightPos, m_vecCoords2, m_vecTexCoords1, m_vecColors2, 255);

			//Render All the HardGlows
			sceneManager->getRenderer()->setAppearance(m_App4);
			renderFlare(billBoardTransform, camView, currLightPos, m_vecCoords10, m_vecTexCoords1, m_vecColors10, 255);

			Point3 point;
			sf32 t = 0.1f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords11, m_vecTexCoords1, m_vecColors11, 200);

			t = 0.2755f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords12, m_vecTexCoords1, m_vecColors12, 165);

			t = 0.4775f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords13, m_vecTexCoords1, m_vecColors13, 135);

			t = 0.65f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords14, m_vecTexCoords1, m_vecColors14, 95);

			t = 0.63f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords15, m_vecTexCoords1, m_vecColors15, 55);

			t = 0.7825f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords16, m_vecTexCoords1, m_vecColors16, 105);

			t = 0.975f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords17, m_vecTexCoords1, m_vecColors17, 80);

			//Render All the Halos
			sceneManager->getRenderer()->setAppearance(m_App3);
			
			point;
			t = 0.15f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords4, m_vecTexCoords1, m_vecColors4, 200);

			t = 0.175f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords5, m_vecTexCoords1, m_vecColors5, 180);

			t = 0.285f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords6, m_vecTexCoords1, m_vecColors6, 155);

			t = 0.49f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords7, m_vecTexCoords1, m_vecColors7, 80);

			t = 0.8f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords8, m_vecTexCoords1, m_vecColors8, 55);

			t = 1.0f * 2.0f;
			point.x = currLightPos.x + m_LightToPointOfIntersection.x * m_LightToPointOfIntersectionLength * t;
			point.y = currLightPos.y + m_LightToPointOfIntersection.y * m_LightToPointOfIntersectionLength * t;
			point.z = currLightPos.z + m_LightToPointOfIntersection.z * m_LightToPointOfIntersectionLength * t;

			renderFlare(billBoardTransform, camView, point, m_vecCoords9, m_vecTexCoords1, m_vecColors9, 70);

			//Render All HardGlow
		}

		void LensFlareNode::OnRegister()
		{
			SViewFrustum frust = *sceneManager->getActiveCamera()->getViewFrustum();

			Point3 currLightPos = m_LightPosition;
			globalTransform.transform(currLightPos);
			m_distLightSq = 0.0f;
			m_Fraction = frust.sphereFractionInside(currLightPos,10.0f);
			m_Fraction = m_Fraction > 1.0f ? 1.0f : m_Fraction;
			if (  m_Fraction >= 0.01f )
			{
				sceneManager->registerNode(this , SceneManager::TARGET_TRANSPARENT );
	
				Point3 eyePos = sceneManager->getActiveCamera()->getEyePosition();
				Matrix4f& viewMatrix = sceneManager->getActiveCamera()->getViewMatrix();
				Vector3 forward(viewMatrix.m20, viewMatrix.m21, viewMatrix.m22);
				m_distLightSq = currLightPos.dist(eyePos);
				forward.scale(m_distLightSq);
				m_PointOfIntersection.set(eyePos.x-forward.x, eyePos.y-forward.y, eyePos.z-forward.z);
				m_LightToPointOfIntersection.sub(m_PointOfIntersection, currLightPos);
				m_LightToPointOfIntersectionLength = m_LightToPointOfIntersection.length();
				m_LightToPointOfIntersection.scale(1.0f/m_LightToPointOfIntersectionLength);
			}
			
		}

		void LensFlareNode::print()
		{
		}

		void LensFlareNode::renderFlare(Matrix4f& billBoardTransform, Matrix4f& viewMat, Point3 position, vector<sf32>& vecCoords, vector<sf32>& vecTexCoords, vector<su8>& vecColors, su8 alpha)
		{
			Matrix4f temp;
			Matrix4f xform;
			xform.setTranslation(Vector3(position.x,position.y,position.z));
			temp.multiply(viewMat,xform);
			temp.multiply(billBoardTransform);

			sceneManager->getRenderer()->setTransform(temp , WORLD);

			su8 modifiedAlpha = m_Fraction*alpha;
			MODIFY_ALPHA(vecColors,modifiedAlpha)

			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( vecCoords.size()/2 , &vecCoords[0] ,2, 0 , &vecColors[0],4, &vecTexCoords[0],2, 0 , QUADS);
		}

		void LensFlareNode::computeBillBoard(Point3& p, Matrix4f& billBoardTransform)
		{
			Vector3 eyeToCentre;
			Vector3 up(0.0f , 1.0f ,0.0f);
			
			Matrix4f& camView = sceneManager->getActiveCamera()->getViewMatrix();
			up.x = camView.m10;
			up.y = camView.m11;
			up.z = camView.m12;

			eyeToCentre.x = -camView.m20;
			eyeToCentre.y = -camView.m21;
			eyeToCentre.z = -camView.m22;
			
			Vector3 right;
			right = Vector3::cross( up , eyeToCentre);

			up = eyeToCentre.cross(right);

			billBoardTransform.m00 = right.x; billBoardTransform.m01 = up.x; billBoardTransform.m02 = eyeToCentre.x;
			billBoardTransform.m10 = right.y; billBoardTransform.m11 = up.y; billBoardTransform.m12 = eyeToCentre.y;
			billBoardTransform.m20 = right.z; billBoardTransform.m21 = up.z; billBoardTransform.m22 = eyeToCentre.z;
		}
	}
}

		
