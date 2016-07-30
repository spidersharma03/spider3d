#ifndef _PARTICLE_NODE_H
#define _PARTICLE_NODE_H

#include "Node.h"
#include <stack>

namespace spider3d 
{
	namespace particlesystem 
	{
		class Particle;
		class ParticleEmitter;
		class ParticleEmitterPoint;
		class ParticleEmitterLine;
		class ParticleEmitterCylinder;
		class ParticleEmitterSphere;
		class ParticleAnimator;
		class ParticleGravityAnimator;
		class ParticleTornadoAnimator;
		class ParticleDiffuseAnimator;
		class ParticleFlockAnimator;
		class ParticleOrbitAnimator;
		class ParticleTargetFollowAnimator;
	}
}
namespace spider3d 
{
	namespace appearance
	{
		class Appearance;
	}
}

using namespace spider3d::particlesystem;
using namespace spider3d::appearance;

namespace spider3d 
{
		
	namespace scene
	{
		enum PARTICLE_SHAPE{ EPS_POINT, EPS_LINE, EPS_QUAD};

		class MeshBuffer;

class ParticleNode:public Node
		{
		public:

			ParticleNode(Nodeptr , SceneManagerptr, su32 numActiveParticles, su32 maxParticles);

			ParticleNode(Nodeptr , SceneManagerptr, su32 numActiveParticles, su32 maxParticles, PARTICLE_SHAPE shape);

			virtual void render();  	

			virtual void OnRegister();

			virtual void print();

			void setAppearance(Appearance* appearance);

			Appearance* getAppearance();

			void setParticleEmitter( ParticleEmitter* particleEmitter);
			
			void addParticleAnimator(ParticleAnimator* particleAnimator);
			
			inline void setEnable(sbool bEnable)
			{
				m_bEnable = bEnable;
			}

			inline ParticleEmitter* getParticleEmitter()
			{
				return m_ParticleEmitter;
			}

			inline void setSortingEnable(bool bSort)
			{
				m_bSort = bSort;
			}

			void setParticleShape(PARTICLE_SHAPE shape)
			{
				m_Shape = shape;
			}

			PARTICLE_SHAPE getParticleShape()
			{
				return m_Shape;
			}

			friend class spider3d::particlesystem::ParticleEmitterPoint;
			friend class spider3d::particlesystem::ParticleEmitterLine;
			friend class spider3d::particlesystem::ParticleEmitterCylinder;
			friend class spider3d::particlesystem::ParticleEmitterSphere;
			friend class spider3d::particlesystem::ParticleAnimator;
			friend class spider3d::particlesystem::ParticleGravityAnimator;
			friend class spider3d::particlesystem::ParticleTornadoAnimator;
            friend class spider3d::particlesystem::ParticleDiffuseAnimator; 
			friend class spider3d::particlesystem::ParticleFlockAnimator;
			friend class spider3d::particlesystem::ParticleOrbitCentreAnimator;
			friend class spider3d::particlesystem::ParticleTargetFollowAnimator;
		private:

			ParticleEmitter* m_ParticleEmitter;
			vector<ParticleAnimator*> m_VecParticleAnimators;
			vector<Particle> m_ParticlePool;
			vector<Particle> particleLinePool;
			su32 m_MaxParticles;
			su32 m_NumActiveParticles;
			Appearance* m_pAppearance;
			MeshBuffer* m_ParticleBuffer;
			PARTICLE_SHAPE m_Shape;
			sbool m_bSort;
			sbool m_bEnable;
		};

	}
}

#endif