#include "SkyBoxNode.h"
#include "SceneManager.h"
#include "OglRenderer.h"
#include "Appearance.h"
#include "Texture2D.h"


namespace spider3d
{
	namespace scene
	{

		SkyBoxNode::SkyBoxNode( Nodeptr nodePtr, SceneManagerptr sceneManagerptr, 
			Image* left, Image* right,
			Image* front, Image* back,
			Image* top  , Image* bottom):Node(nodePtr , sceneManagerptr),
			left(left),right(right),front(front),back(back),top(top),bottom(bottom)
		{
			setCullOperation(CULL_OFF);

			createGeometry();

			globalTransform.setIdentity();
			Texture2D* front_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , front->getWidth()  , front->getHeight() ); 
		    front_texture->setImage(0,front);
			TextureUnitState* front_texUnit = new TextureUnitState(front_texture);
			//front_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;

			Texture2D* back_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , back->getWidth()  , back->getHeight() ); 
		    back_texture->setImage(0,back);
			TextureUnitState* back_texUnit = new TextureUnitState(back_texture);
			//back_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;

			Texture2D* left_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , left->getWidth()  , left->getHeight() ); 
		    left_texture->setImage(0,left);
			TextureUnitState* left_texUnit = new TextureUnitState(left_texture);
			//left_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;

			Texture2D* right_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , right->getWidth()  , right->getHeight() ); 
		    right_texture->setImage(0,right);
			TextureUnitState* right_texUnit = new TextureUnitState(right_texture);
			//right_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;

			Texture2D* top_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , top->getWidth()  , top->getHeight() ); 
		    top_texture->setImage(0,top);
			TextureUnitState* top_texUnit = new TextureUnitState(top_texture);
			//top_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
			
			Texture2D* bottom_texture = new Texture2D( ETEX_BM_CLAMP , ETF_RGBA , bottom->getWidth()  , bottom->getHeight() ); 
		    bottom_texture->setImage(0,bottom);
			TextureUnitState* bottom_texUnit = new TextureUnitState(bottom_texture);
			//bottom_texUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
			
			frontApp = new Appearance();
			frontApp->setTextureUnitState(0,front_texUnit);
			frontApp->getMaterial()._mLightingEnabled = false;
			frontApp->getPolygonAttributes().cullFace = ECF_NONE;
			frontApp->getDepthAttributes().DepthTest = false;
			frontApp->getDepthAttributes().DepthMask = false;
			//frontApp.getDepthAttributes().CompareFunc = ECFN_NEVER;
			
			backApp = new Appearance();
			backApp->setTextureUnitState(0,back_texUnit);
			backApp->getMaterial()._mLightingEnabled = false;
			backApp->getPolygonAttributes().cullFace = ECF_NONE;
			backApp->getDepthAttributes().DepthTest = false;
			backApp->getDepthAttributes().DepthMask = false;
			//backApp.getDepthAttributes().CompareFunc = ECFN_NEVER;

			leftApp = new Appearance();
			leftApp->setTextureUnitState(0,left_texUnit);
			leftApp->getMaterial()._mLightingEnabled = false;
			leftApp->getPolygonAttributes().cullFace = ECF_NONE;
			leftApp->getDepthAttributes().DepthTest = false;
			leftApp->getDepthAttributes().DepthMask = false;
			//leftApp.getDepthAttributes().CompareFunc = ECFN_NEVER;

			rightApp = new Appearance();
			rightApp->setTextureUnitState(0,right_texUnit);
			rightApp->getMaterial()._mLightingEnabled = false;
			rightApp->getPolygonAttributes().cullFace = ECF_NONE;
			rightApp->getDepthAttributes().DepthTest = false;
			rightApp->getDepthAttributes().DepthMask = false;
			//rightApp.getDepthAttributes().CompareFunc = ECFN_NEVER;

			topApp = new Appearance();
			topApp->setTextureUnitState(0,top_texUnit);
			topApp->getMaterial()._mLightingEnabled = false;
			topApp->getPolygonAttributes().cullFace = ECF_NONE;
			topApp->getDepthAttributes().DepthTest = false;
			topApp->getDepthAttributes().DepthMask = false;
			//topApp.getDepthAttributes().CompareFunc = ECFN_NEVER;

			bottomApp = new Appearance();
			bottomApp->setTextureUnitState(0,bottom_texUnit);
			bottomApp->getMaterial()._mLightingEnabled = false;
			bottomApp->getPolygonAttributes().cullFace = ECF_NONE;
			//bottomApp.getDepthAttributes().CompareFunc = ECFN_NEVER;
			bottomApp->getDepthAttributes().DepthTest = false;
			bottomApp->getDepthAttributes().DepthMask = false;
		}

		SkyBoxNode::SkyBoxNode( Nodeptr nodePtr , SceneManagerptr sceneManagerptr, Image* images):Node(nodePtr , sceneManagerptr)
		{
			left   = images;
			right  = images + 1;
			front  = images + 2;
			back   = images + 3;
			top    = images + 4;
			bottom = images + 5;

			setCullOperation(CULL_OFF);

			createGeometry();

			globalTransform.setIdentity();
			frontApp->getMaterial()._mLightingEnabled = false;
			frontApp->getPolygonAttributes().cullFace = ECF_NONE;
		}

		void SkyBoxNode::render()
		{
			Matrix4f& viewMat = sceneManager->getActiveCamera()->getViewMatrix();
			Vector3 v;
			v.set(sceneManager->getActiveCamera()->getEyePosition());
			globalTransform.setTranslation( v );
			Matrix4f temp;
			temp.multiply(viewMat , globalTransform);
			sceneManager->getRenderer()->setTransform(temp , WORLD);
			
			sceneManager->getRenderer()->setAppearance(frontApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , frontCoords , 3 , 0 , 0,0, texCoordsFront, 2, 0 , QUADS);
			
			sceneManager->getRenderer()->setAppearance(backApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , backCoords , 3 , 0 , 0 , 0 , texCoordsFront,2 , 0, QUADS);
			
			sceneManager->getRenderer()->setAppearance(leftApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , leftCoords ,3, 0 , 0,0, texCoordsFront ,2 , 0, QUADS);
			
			sceneManager->getRenderer()->setAppearance(rightApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , rightCoords,3 , 0 , 0 , 0 , texCoordsFront , 2 , 0, QUADS);
			
			sceneManager->getRenderer()->setAppearance(topApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , topCoords ,3, 0 , 0, 0,texCoordsFront ,2,0, QUADS);
			
			sceneManager->getRenderer()->setAppearance(bottomApp);
			((OglRenderer*)sceneManager->getRenderer())->drawVertexPrimitiveList( 4 , bottomCoords ,3, 0 , 0,0, texCoordsFront,2, 0 , QUADS);

		}

		void SkyBoxNode::OnRegister()
		{
			if( _mVisible )
			{
				sceneManager->registerNode(this , SceneManager::TARGET_SKY_BOX );
				Node::OnRegister();
			}
		}

		void SkyBoxNode::print()
		{
		}

		void SkyBoxNode::createGeometry()
		{
			float width = 30.5f;

			// front face
			texCoordsFront[0] =  1.0f ; texCoordsFront[1]  =  1.0f;
			texCoordsFront[2] =  0.0f ; texCoordsFront[3]  =  1.0f;
			texCoordsFront[4] =  0.0f ; texCoordsFront[5]  =  0.0f;
			texCoordsFront[6] =  1.0f ; texCoordsFront[7]  =  0.0f;

			frontCoords[0] =  width/2 ; frontCoords[1]  =  width/2 ; frontCoords[2] = width/2;
			frontCoords[3] = -width/2 ; frontCoords[4]  =  width/2 ; frontCoords[5] = width/2;
			frontCoords[6] = -width/2 ; frontCoords[7]  = -width/2 ; frontCoords[8] = width/2;
			frontCoords[9] =  width/2 ; frontCoords[10] = -width/2 ; frontCoords[11] = width/2;
			
			// back face
			texCoordsBack[0] =  0.0f ; texCoordsBack[1]  =  1.0f;
			texCoordsBack[2] =  1.0f ; texCoordsBack[3]  =  1.0f;
			texCoordsBack[4] =  1.0f ; texCoordsBack[5]  =  0.0f;
			texCoordsBack[6] =  0.0f ; texCoordsBack[7]  =  0.0f;

			backCoords[0] = -width/2 ; backCoords[1] =  width/2 ; backCoords[2] = -width/2;
			backCoords[3] =  width/2 ; backCoords[4] =  width/2 ; backCoords[5] = -width/2;
			backCoords[6] =  width/2 ; backCoords[7] = -width/2 ; backCoords[8] = -width/2;
			backCoords[9] = -width/2 ; backCoords[10] = -width/2 ; backCoords[11] = -width/2;

			// left face
			texCoordsLeft[0] =  1.0f ; texCoordsLeft[1]  =  1.0f;
			texCoordsLeft[2] =  1.0f ; texCoordsLeft[3]  =  0.0f;
			texCoordsLeft[4] =  0.0f ; texCoordsLeft[5]  =  0.0f;
			texCoordsLeft[6] =  0.0f ; texCoordsLeft[7]  =  1.0f;

			leftCoords[0] = -width/2 ; leftCoords[1] = width/2 ; leftCoords[2] =  width/2;
			leftCoords[3] = -width/2 ; leftCoords[4] = width/2 ; leftCoords[5] =  -width/2;
			leftCoords[6] = -width/2 ; leftCoords[7] = -width/2 ; leftCoords[8] = -width/2;
			leftCoords[9] = -width/2 ; leftCoords[10] = -width/2 ; leftCoords[11] = width/2;

			// right face
			texCoordsRight[0] =  1.0f ; texCoordsRight[1]  =  0.0f;
			texCoordsRight[2] =  1.0f ; texCoordsRight[3]  =  1.0f;
			texCoordsRight[4] =  0.0f ; texCoordsRight[5]  =  1.0f;
			texCoordsRight[6] =  0.0f ; texCoordsRight[7]  =  0.0f;

			rightCoords[0] =  width/2 ; rightCoords[1] = width/2 ; rightCoords[2] =  -width/2;
			rightCoords[3] =  width/2 ; rightCoords[4] = width/2 ; rightCoords[5] =  width/2;
			rightCoords[6] =  width/2 ; rightCoords[7] = -width/2 ; rightCoords[8] = width/2;
			rightCoords[9] =  width/2 ; rightCoords[10] = -width/2 ; rightCoords[11] = -width/2;


			// top face
			texCoordsTop[0] =  1.0f ; texCoordsTop[1]  =  0.0f;
			texCoordsTop[2] =  1.0f ; texCoordsTop[3]  =  1.0f;
			texCoordsTop[4] =  0.0f ; texCoordsTop[5]  =  1.0f;
			texCoordsTop[6] =  0.0f ; texCoordsTop[7]  =  0.0f;

			// top face
			topCoords[0] =  width/2 ; topCoords[1] = width/2 ; topCoords[2] =  -width/2;
			topCoords[3] =  -width/2 ; topCoords[4] = width/2 ; topCoords[5] =  -width/2;
			topCoords[6] =  -width/2 ;  topCoords[7] = width/2 ; topCoords[8] = width/2;
			topCoords[9] =  width/2 ;  topCoords[10] = width/2 ; topCoords[11] = width/2;

			// bottom face
			bottomCoords[0] =  width/2 ; bottomCoords[1] = -width/2 ; bottomCoords[2] =  width/2;
			bottomCoords[3] =  -width/2 ; bottomCoords[4] = -width/2 ; bottomCoords[5] =  width/2;
			bottomCoords[6] =  -width/2 ;  bottomCoords[7] = -width/2 ; bottomCoords[8] = -width/2;
			bottomCoords[9] =  width/2 ;  bottomCoords[10] = -width/2 ; bottomCoords[11] = -width/2;

		}
	}
}