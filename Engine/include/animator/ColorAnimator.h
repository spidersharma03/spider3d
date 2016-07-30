#ifndef _COLOR_ANIMATOR_H

#define _COLOR_ANIMATOR_H


#include "NodeAnimator.h"
#include "Appearance.h"

using namespace spider3d::appearance;

namespace spider3d
{

  namespace scene
  {


  class ColorAnimator:public NodeAnimator
	{
	public:

		void animate(sf32 time);

		ColorAnimator(Appearance* target);

		ColorAnimator(Appearance* target , Color3 startColor , Color3 finalColor );

		void setStartColor(Color3& color);

		void setFinalColor(Color3& color);

	private:
    	Color3 startColor;
		Color3 finalColor;
        Appearance* target;
 	};

  }


}

#endif 