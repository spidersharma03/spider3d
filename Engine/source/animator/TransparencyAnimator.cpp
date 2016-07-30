#include "animator/TransparencyAnimator.h"


namespace spider3d
{

	namespace scene
	{


		void TransparencyAnimator::animate(sf32 time)
		{
			sf32 transparency = startTransparency * ( 1 - time) + finalTransparency * time;;
			if ( !target )
				return;
			target->getTransparencyAttributes().transparency = transparency;
		}


		TransparencyAnimator::TransparencyAnimator(Appearance* target, sf32 startTransparency , sf32 finalTransparency )
			:target(target),startTransparency(startTransparency),finalTransparency(finalTransparency)
		{
		}

		TransparencyAnimator::TransparencyAnimator(Appearance* target ):target(target)
		{
			startTransparency = 0.0f;
			finalTransparency = 1.0f;
		}

		void TransparencyAnimator::setStartTransparency(sf32 startTransparency)
		{
			this->startTransparency = startTransparency;
		}

		void TransparencyAnimator::setFinalTransparency(sf32 finalTransparency)
		{
			this->finalTransparency = finalTransparency;
		}

		
	}


}