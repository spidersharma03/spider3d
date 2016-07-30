#include "animator/ThirdPersonController.h"
#include "scene/sceneNodes/Node.h"
#include "scene/SceneManager.h"
#include "util/STimer.h"
#include "device/SpiderDisplay.h"

#define MOVE 4.0f

namespace spider3d
{
	namespace scene
	{

		ThirdPersonController::ThirdPersonController():m_Target(0)
		{
			for(int i=0; i<256; i++ )
				keyCodes[i] = false;
			mouseEvent[0] = mouseEvent[1] = mouseEvent[2] = false;
		}

		ThirdPersonController::ThirdPersonController(Node* target):m_Target(target)
		{
			for(int i=0; i<256; i++ )
				keyCodes[i] = false;
			mouseEvent[0] = mouseEvent[1] = mouseEvent[2] = false;

			target->getSceneManager()->getDisplay()->addEventListener(this);

			m_Target->addAnimator(this);
		}

		void ThirdPersonController::setTarget(Node* target)
		{
			m_Target = target;
		}

		Node* ThirdPersonController::getTarget()
		{
			return m_Target;
		}


		void ThirdPersonController::animate(sf32 time)
		{
			m_CurrentTime = m_Target->getSceneManager()->getTimer()->getCurrentTime();

			static sbool bFirst = true;
			sf32 deltaT = (m_CurrentTime - m_PrevTime)/1000.0f;
			if(bFirst)
			{
				bFirst = false;
				m_PrevTime = m_CurrentTime;
				return;
			}
			
			if( !keyCodes[KEY_KEY_W] ||
				!keyCodes[KEY_KEY_A] || 
				!keyCodes[KEY_KEY_S] || keyCodes[KEY_KEY_D])
			{
				m_Velocity.x = m_Velocity.z = 0.0f;
			}

			//if( inputEvent.eventType == SKEY_EVENT )
			{
				if( keyCodes[KEY_KEY_W] )
				{
					m_Velocity.z = -MOVE;
				}
				if( keyCodes[KEY_KEY_A] )
				{
					m_Velocity.x = -MOVE;
				}
				if( keyCodes[KEY_KEY_S] )
				{
					m_Velocity.z = MOVE;
				}
				if( keyCodes[KEY_KEY_D] )
				{
					m_Velocity.x = MOVE;
				}

				//printf("deltaT %f \t %f\t %f\n", deltaT, m_PrevTime, m_CurrentTime);
				m_Position.x += m_Velocity.x * deltaT;
				m_Position.z += m_Velocity.z * deltaT;


				m_TargetTransform.setTranslation(Vector3(m_Position));
				m_Target->setTransform(m_TargetTransform);
			}

			m_PrevTime = m_CurrentTime;
		}

		sbool ThirdPersonController::OnEvent(Event& sEvent)
		{
			inputEvent = sEvent;
			
			if ( inputEvent.eventType == SKEY_EVENT )
			{
				for( unsigned i = 0; i<256; i++ ) 
					keyCodes[inputEvent.keyInput.Key] = inputEvent.keyInput.PressedDown;
			
				//printf("%d\n", keyCodes[KEY_KEY_D]);
			}
			return true;
		}
	}
}