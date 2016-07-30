#ifndef _POSITIONANIMATOR_H

#define _POSITIONANIMATOR_H


#include "TransformAnimator.h"

namespace spider3d
{

  namespace scene
  {


  class PositionAnimator:public TransformAnimator
	{
	public:

		void animate(sf32 time);

		PositionAnimator(Node* target, Matrix4f& axis);

		PositionAnimator(Node* target , Matrix4f& axis , sf32 startPosition , sf32 finalPosition);

	private:
    	sf32 startPosition;
		sf32 finalPosition;

		Matrix4f trans;
        Matrix4f currentTransform;
	};

  }


}

#endif 