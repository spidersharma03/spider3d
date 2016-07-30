#include "NodeAnimator.h"
#include "CameraNode.h"
#include "Appearance.h"

using namespace spider3d::appearance;

namespace spider3d
{
	namespace scene
	{ 
		class TextureMatrixUpdater:public NodeAnimator
		{
		public:
			TextureMatrixUpdater(CameraNode* camera, Appearance* appearance)
			{
				cameraNode = camera;
				texUnit1 = appearance->getTextureUnitState(0);
				texUnit2 = appearance->getTextureUnitState(1);
				ScaleBias.m00 = 0.5f; ScaleBias.m01 = 0.0f; ScaleBias.m02 = 0.0f; ScaleBias.m03 = 0.5f;
				ScaleBias.m10 = 0.0f; ScaleBias.m11 = 0.5f; ScaleBias.m12 = 0.0f; ScaleBias.m13 = 0.5f;
				ScaleBias.m20 = 0.0f; ScaleBias.m21 = 0.0f; ScaleBias.m22 = 1.0f; ScaleBias.m23 = 0.0f;
				ScaleBias.m30 = 0.0f; ScaleBias.m31 = 0.0f; ScaleBias.m32 = 0.0f; ScaleBias.m33 = 1.0f;

				D1.m00 = -1.0f; D1.m01 = 0.0f; D1.m02 = 0.0f; D1.m03 = 0.0f;
				D1.m10 = 0.0f; D1.m11 = -1.0f; D1.m12 = 0.0f; D1.m13 = 0.0f;
				D1.m20 = 0.0f; D1.m21 = 0.0f; D1.m22 = 1.0f; D1.m23 = -1.0f;
				D1.m30 = 0.0f; D1.m31 = 0.0f; D1.m32 = 0.0f; D1.m33 = 1.0f;

				D2.m00 = -1.0f; D2.m01 = 0.0f; D2.m02 = 0.0f; D2.m03 = 0.0f;
				D2.m10 = 0.0f; D2.m11 = -1.0f; D2.m12 = 0.0f; D2.m13 = 0.0f;
				D2.m20 = 0.0f; D2.m21 = 0.0f; D2.m22 = 1.0f; D2.m23 = 1.0f;
				D2.m30 = 0.0f; D2.m31 = 0.0f; D2.m32 = 0.0f; D2.m33 = 1.0f;

				P.m00 = 1.0f; P.m01 = 0.0f; P.m02 = 0.0f; P.m03 = 0.0f;
				P.m10 = 0.0f; P.m11 = 1.0f; P.m12 = 0.0f; P.m13 = 0.0f;
				P.m20 = 0.0f; P.m21 = 0.0f; P.m22 = 1.0f; P.m23 = 0.0f;
				P.m30 = 0.0f; P.m31 = 0.0f; P.m32 = 1.0f; P.m33 = 0.0f;

				ScaleBias_P_D1 = ScaleBias;
				ScaleBias_P_D1.multiply( P );
				ScaleBias_P_D1.multiply( D1 );

				ScaleBias_P_D2 = ScaleBias;
				ScaleBias_P_D2.multiply( P );
				ScaleBias_P_D2.multiply( D2 );
			}//40835842

			void animate(sf32 time)
			{
				Matrix4f viewMatrix = cameraNode->getViewMatrix();
				viewMatrix.m03 = viewMatrix.m13 = viewMatrix.m23 = 0.0f;
				viewMatrix.m33 = 1.0f;
				Matrix4f invViewMatrix;
				invViewMatrix = viewMatrix.getInverse();

				Matrix4f R1 = ScaleBias_P_D1;
				R1.multiply(invViewMatrix);

				Matrix4f R2 = ScaleBias_P_D2;
				R2.multiply(invViewMatrix);

				texUnit1->getTextureAttributes()->texture_transform = R1;
				texUnit2->getTextureAttributes()->texture_transform = R2;
			}

		private:
			TextureUnitState *texUnit1;
			TextureUnitState *texUnit2;
			CameraNode* cameraNode;

			Matrix4f ScaleBias;
			Matrix4f D1, D2;
			Matrix4f P;
			Matrix4f ScaleBias_P_D1;
			Matrix4f ScaleBias_P_D2;
		};

	}
}