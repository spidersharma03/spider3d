#include "animator/RotPosScalePathAnimator.h"

//! constructor
RotPosScalePathAnimator::RotPosScalePathAnimator(Node* target, Matrix4f& axis , TCBKeyFrame* keyFrames , int numKeys )
{
	this->numKeys = numKeys;
	this->keyFrames = new TCBKeyFrame[numKeys];
	for ( int i = 0 ; i < numKeys ; i ++ )
	{
		this->keyFrames[i] = keyFrames[i];
	}
	this->axis = axis;
	axisInv = axis.getInverse();
	m_Target = target;
}


//! destructor
RotPosScalePathAnimator::~RotPosScalePathAnimator()
{
}



//! animates a scene node
void RotPosScalePathAnimator::animate(sf32 timeMs)
{
	static float cnt = 0;
	
	if ( cnt > numKeys )cnt = 0;
	if (m_Target) 
	{
		su32 diffTime = timeMs - StartTime;

		if (diffTime != 0)
		{
			Matrix4f& t3d = axis;
			TCBKeyFrame& keyframe = keyFrames[(int)cnt];

			Vector3& point = keyframe.position;
			Vector3& scaleFactors = keyframe.scale;
			Quaternion& quat = keyframe.quat;

			Matrix4f tMat = quat.getMatrix();
			tMat = tMat.getTranspose();

			tMat.setTranslation(point);

			Matrix4f keyFrameTransform(tMat);

			axis = t3d;
			t3d = t3d.getInverse();
			axisInv = t3d;

			Matrix4f transform , scaleTransform;
			transform.multiply( axis , keyFrameTransform);

			scaleTransform.setScale(scaleFactors);

			transform.multiply( scaleTransform );
			transform.multiply( axisInv );

			Node* dummyNode = m_Target;
			m_Target->setTransform(transform);
		}
	}
	cnt += 0.5;
}



