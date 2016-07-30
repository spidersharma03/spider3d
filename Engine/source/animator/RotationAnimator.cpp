#include "animator/RotationAnimator.h"
#include<stdio.h>

namespace spider3d
{

  namespace scene
  {


	  void RotationAnimator::animate(sf32 time)
	  {
		  sf32 angle = startAngle * ( 1 - time) + finalAngle * time;
		  rot.rotY(angle);
		  currentTransform.multiply( axis , rot );
		  currentTransform.multiply( axisInv );
		  target->setTransform(currentTransform); 
	  }


	  RotationAnimator::RotationAnimator(Node* target , Matrix4f& axis , sf32 startAngle , sf32 finalAngle)
		  :TransformAnimator(target , axis),startAngle(startAngle),finalAngle(finalAngle)
	  {
	  }

	  RotationAnimator::RotationAnimator(Node* target, Matrix4f& axis ):TransformAnimator(target , axis)
	  {
		  startAngle = 0.0;
		  finalAngle = TWO_M_PI;
	  }


  }


}