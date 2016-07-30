#ifndef _ROT_POS_SCALE_PATH_ANIMATOR

#define _ROT_POS_SCALE_PATH_ANIMATOR

#include "spiderDataTypes.h"
#include "TCBKeyFrame.h"
#include "NodeAnimator.h"

using namespace spider3d;
using namespace spider3d::scene;


class RotPosScalePathAnimator : public NodeAnimator
	{
	public:

		//! constructor
		RotPosScalePathAnimator(Node* target, Matrix4f& axis , TCBKeyFrame* keyFrames , int numKeys );

		//! destructor
		virtual ~RotPosScalePathAnimator();

		//! animates a scene node
		virtual void animate( sf32 timeMs);
	
		
	private:

		TCBKeyFrame* keyFrames;
		Matrix4f axis , axisInv;
		Node* m_Target;
		int numKeys;
		su32 StartTime;
	};



#endif


