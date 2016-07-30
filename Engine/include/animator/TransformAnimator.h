#ifndef _TRANSFORM_ANIMATOR_H

#define _TRANSFORM_ANIMATOR_H

#include "NodeAnimator.h"


namespace spider3d
{

  namespace scene
  {

   
  class TransformAnimator:public NodeAnimator
   {

   public:

	   virtual void animate(sf32 time) = 0;

	   void setTarget(Node* target)
	   {
		   this->target = target;
	   }

	   Node* getTarget()
	   {
		   return target;
	   }

	   void setTransformAxis(Matrix4f& axis)
	   {
		   this->axis.set(axis);
	   }

	   void setTransformLocal(sbool isLocal)
	   {
		   this->isLocal = isLocal;
	   }

	   Matrix4f& getTransformAxis()
	   {
		   return axis;
	   }

	   TransformAnimator(Node* target , Matrix4f& axis):target(target),axis(axis)
	   {
		   axisInv = axis.getInverse();
	   };

   protected:

  	   Node* target;
	   sbool isLocal;
       Matrix4f axis;
	   Matrix4f axisInv;
   };

  }

}

#endif