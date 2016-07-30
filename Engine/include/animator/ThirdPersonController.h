#ifndef _THIRD_PERSON_CONTROLLER_H_
#define _THIRD_PERSON_CONTROLLER_H_

#include "NodeAnimator.h"
#include "coreMath.h"
#include "EventListener.h"

namespace spider3d
{
	namespace scene
	{
		class Node;

		class ThirdPersonController : public EventListener, NodeAnimator
		{
		public:

			ThirdPersonController();

			ThirdPersonController(Node*);

			void setTarget(Node*);

			Node* getTarget();

			virtual sbool OnEvent(Event& sEvent);

			virtual void animate(sf32 time);

		private:
			Node *m_Target;
			Matrix4f m_TargetTransform;
			Vector3 m_Velocity;
			Vector3 m_Position;

			sf32 m_CurrentTime;
			sf32 m_PrevTime;

			Event inputEvent;
			sbool mouseEvent[3];
			sbool keyCodes[256];
		};
	}
}
#endif