#ifndef _TEXTURE_PROJECTOR_H
#define _TEXTURE_PROJECTOR_H

#include "coreMath.h"
#include "CameraNode.h"

using namespace spider3d::core;

namespace spider3d
{
	namespace effects
	{
		class TextureProjector
		{
		public:
			TextureProjector( CameraNode* camera , Point3& projectorPosition, Point3& projectorTarget , Vector3& projectorUp )
			{
				this->camera = camera;
				ScaleBias.m00 = 0.5f; ScaleBias.m01 = 0.0f; ScaleBias.m02 = 0.0f; ScaleBias.m03 = 0.5f;
				ScaleBias.m10 = 0.0f; ScaleBias.m11 = 0.5f; ScaleBias.m12 = 0.0f; ScaleBias.m13 = 0.5f;
				ScaleBias.m20 = 0.0f; ScaleBias.m21 = 0.0f; ScaleBias.m22 = 0.5f; ScaleBias.m23 = 0.5f;
				ScaleBias.m30 = 0.0f; ScaleBias.m31 = 0.0f; ScaleBias.m32 = 0.0f; ScaleBias.m33 = 1.0f;

				View.lookAt(projectorPosition, projectorTarget, projectorUp);

				//Projection.set( camera->getProjectionMatrix() );
				Projection.perspective( camera->getFOV()/2 , camera->getAspectRatio() , camera->getNearPlane() , camera->getFarPlane() );
	
				InvCameraView.set( camera->getViewMatrix() );

				InvCameraView = InvCameraView.getInverse();

			}
			
			void lookAt( Point3& projectorPosition, Point3& projectorTarget , Vector3& projectorUp )
			{
				View.lookAt(projectorPosition, projectorTarget, projectorUp);
			}

			void lookAt( Matrix4f& viewMatrix )
			{
				View.set(viewMatrix);
			}

			void getProjectorMatrix(Matrix4f& outMatrix)
			{
				outMatrix = ScaleBias;

				outMatrix.multiply( Projection );
				
				outMatrix.multiply( View );

				InvCameraView.set( camera->getViewMatrix() );

				InvCameraView = InvCameraView.getInverse();

				outMatrix.multiply( InvCameraView );
				
			}

			void getProjectorMatrix3D(Matrix4f& outMatrix)
			{
				outMatrix = ScaleBias;

				outMatrix.multiply( Projection );
				
				InvCameraView.set( camera->getViewMatrix() );

				outMatrix.multiply( InvCameraView );
				
			}

			void getCameraProjectorMatrix(Matrix4f& outMatrix)
			{
				outMatrix = ScaleBias;

				outMatrix.multiply( Projection );
			}

			Matrix4f& getViewMatrix()
			{
				return View;
			}

		private:
			Matrix4f ScaleBias;
			Matrix4f Projection;
			Matrix4f View;
			Matrix4f InvCameraView;	
			Matrix4f projectorMatrix;
			CameraNode* camera;
		};
	}
}
#endif