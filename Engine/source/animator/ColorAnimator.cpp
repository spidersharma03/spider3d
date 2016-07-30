#include "animator/ColorAnimator.h"

#include<stdio.h>


namespace spider3d
{

	namespace scene
	{


		void ColorAnimator::animate(sf32 time)
		{
			Color3 color = startColor.interpolate( finalColor , startColor ,  time );
			if ( !target )
				return;

			Material& material = target->getMaterial();
			switch ( material.colorTarget )
			{
			case ECT_AMBIENT:
				material.ambient.set(color);
				break;
			case ECT_DIFFUSE_AND_AMBIENT:
				material.ambient.set(color);
			case ECT_DIFFUSE:
				material.diffuse.set(color);
				break;
			case ECT_EMMISSIVE:
				material.emissive.set(color);
				break;
			case ECT_SPECULAR:
				material.specular.set(color);
				break;
			default:
				break;
			}
		}


		ColorAnimator::ColorAnimator(Appearance* target, Color3 startColor , Color3 finalColor )
			:target(target),startColor(startColor),finalColor(finalColor)
		{
		}

		ColorAnimator::ColorAnimator(Appearance* target ):target(target)
		{
			startColor.set(1.0f,0.0f,0.0f );
			finalColor.set(1.0f,1.0f,0.0f);
		}

		void ColorAnimator::setStartColor(Color3& color)
		{
			startColor.set(color);
		}

		void ColorAnimator::setFinalColor(Color3& color)
		{
			finalColor.set(color);
		}

		
	}


}