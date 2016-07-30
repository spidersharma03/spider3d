#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "NodeAnimator.h"


namespace spider3d
{
	namespace scene
	{
		class MeshNode;
		class CameraNode;

		enum BILLBOARD_TYPE { EBT_CYLINDRICAL, EBT_SPHERICAL };

		class BillBoard: public NodeAnimator
		{

		public:

			BillBoard(CameraNode*, MeshNode* , BILLBOARD_TYPE);

			virtual void animate(sf32 time);

			// inline Methods
			inline void setType(BILLBOARD_TYPE type)
			{
				m_Type = type;
			}

			inline void setTarget(MeshNode* target)
			{
				m_Target = target;
			}

			inline MeshNode* getTarget()
			{
				return m_Target;
			}

		private:

			BILLBOARD_TYPE m_Type;
			MeshNode*    m_Target;
			CameraNode*  m_CameraNode;
		};

	}
}
#endif