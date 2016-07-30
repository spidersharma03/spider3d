#ifndef _TRANSPARENCY_ANIMATOR_H

#define _TRANSPARENCY_ANIMATOR_H

#include "NodeAnimator.h"
#include "Appearance.h"

using namespace spider3d::appearance;

namespace spider3d
{

  namespace scene
  {


  class TransparencyAnimator:public NodeAnimator
	{
	public:

		void animate(sf32 time);

		TransparencyAnimator(Appearance* target);

		TransparencyAnimator(Appearance* target ,sf32 startTransparency , sf32 finalTransparency);

		void setStartTransparency(sf32 startTransparency);

		void setFinalTransparency(sf32 finalTransparency);

	private:
    	sf32 startTransparency;
		sf32 finalTransparency;
        Appearance* target;
 	};

  }


}

#endif