#ifndef CUSTOM_SHAPE_H
#define CUSTOM_SHAPE_H

#include "GeneralShape2D.h"

namespace spider3d
{
	namespace cad
	{

		class CustomShape:public GeneralShape2D
		{
		public:
			CustomShape(float stepHeight, float stepWidth, int numSteps, int flags);
		private:
			float m_stepHeight, m_stepWidth;
			int m_numSteps;
		protected:
			void createCoord();
		};

	}
}

#endif

