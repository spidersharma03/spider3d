#include "animator/PositionAnimator.h"
#include<stdio.h>

namespace spider3d
{

  namespace scene
  {


	  void PositionAnimator::animate(sf32 time)
	  {
		  sf32 pos = startPosition * ( 1 - time) + finalPosition * time;
		  trans.m03 = pos;
		  currentTransform.multiply( axis , trans );
		  currentTransform.multiply( axisInv );
		  target->setTransform(currentTransform); 
	  }


	  PositionAnimator::PositionAnimator(Node* target , Matrix4f& axis , sf32 startPosition , sf32 finalPosition)
		  :TransformAnimator(target , axis),startPosition(startPosition),finalPosition(finalPosition)
	  {
	  }

	  PositionAnimator::PositionAnimator(Node* target, Matrix4f& axis ):TransformAnimator(target , axis)
	  {
		  startPosition = 0.0f;
		  finalPosition = 1.0f;
	  }


  }


}