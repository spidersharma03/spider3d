#ifndef _SCALEANIMATOR_H

#define _SCALEANIMATOR_H


#include "TransformAnimator.h"

namespace spider3d
{

  namespace scene
  {


  class ScaleAnimator:public TransformAnimator
	{
	public:

		void animate(sf32 time);

		ScaleAnimator(Node* target, Matrix4f& axis);

		ScaleAnimator(Node* target , Matrix4f& axis , sf32 startScale , sf32 finalScale);

	private:
    	sf32 startScale;
		sf32 finalScale;

		Matrix4f scale;
        Matrix4f currentTransform;
	};

  }


}

#endif 