#ifndef _ROTATIONANIMATOR_H

#define _ROTATIONANIMATOR_H


#include "TransformAnimator.h"

namespace spider3d
{

  namespace scene
  {


  class RotationAnimator:public TransformAnimator
	{
	public:

		void animate(sf32 time);

		RotationAnimator(Node* target, Matrix4f& axis);

		RotationAnimator(Node* target , Matrix4f& axis , sf32 startAngle , sf32 finalAngle);

	private:
    	sf32 startAngle;
		sf32 finalAngle;

		Matrix4f rot;
        Matrix4f currentTransform;
	};

  }


}

#endif 