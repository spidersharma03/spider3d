#include "NodeAnimator.h"
#include "ParticleBase.h"

using namespace spider3d::particlesystem;

namespace spider3d
{
	namespace scene
	{ 
		class EmitterTransformUpdater:public NodeAnimator
		{
		public:
			EmitterTransformUpdater(ParticleNode* particleNode, Node* target):m_ParticleNode(particleNode),m_Target(target)
			{
				m_EmitterTransform = m_ParticleNode->getParticleEmitter()->getTransform();
			}

			void animate(sf32 time)
			{
				//sf32 angle = M_PI * sin(8*time)*1.0f;
				//sf32 angle = (sf32)TWO_M_PI * ( 18*time );
				Matrix4f rot = m_Target->getGlobalTransform();
				//rot.rotY(angle);
				m_ParticleNode->getParticleEmitter()->getTransform() = rot; 
			}

		private:
			ParticleNode* m_ParticleNode;
			Node* m_Target;
			Matrix4f m_EmitterTransform;
		};

	}
}