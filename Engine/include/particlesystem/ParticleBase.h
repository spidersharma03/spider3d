#ifndef _PARTICLE_BASE_H
#define _PARTICLE_BASE_H

#include "spiderDataTypes.h"
#include "coreMath.h"
#include <vector>

using namespace std;

using namespace spider3d;
using namespace spider3d::core;

namespace spider3d 
{
	namespace scene
	{
		class ParticleNode;
	}
}

using namespace spider3d::scene;

namespace spider3d 
{
	namespace particlesystem
	{
		struct Particle
		{
			sf32 m_X, m_Y, m_Z;
			sf32 m_PrevX, m_PrevY, m_PrevZ;
			su8 m_R, m_G, m_B, m_A;

			sf32 m_Vx, m_Vy, m_Vz;
			sf32 m_Ax, m_Ay, m_Az;
			sf32 size;

			sf32 m_Mass, m_InvMass;
			sf32 m_Life, m_Decay;
			sbool m_isDead;

			sf32 m_Restitution;
			sf32 distFromCameraSq;

			sf32 m_SizeX, m_SizeY;
			sf32 m_FrictionCoefficient;
			void* userData;

			Particle()
			{
				m_Mass = m_InvMass = 0.0f;
				m_X = m_Y = m_Z = 0.0f;
				m_Vx = m_Vy = m_Vz = 0.0f;
				m_Ax = m_Ay = m_Az = 0.0f;
				m_R = m_G = m_B = m_A = 125;
				m_A = 255;
				size = 1.0f;
				m_Life = m_Decay = 0.0f;
				m_isDead = true;
				distFromCameraSq = 0.0f;
				m_Restitution = 0.99f;
				m_SizeX = m_SizeY = 1.0f;
				m_FrictionCoefficient = 0.0f;
				userData = 0;
			}
			~Particle()
			{
			}

		};

		// Call back function prototype
		typedef void (*PARTICLE_CALL_BACK)(Particle&, void* userData);

		class ParticleEmitter
		{
		public:
			ParticleEmitter()
			{
				m_EmissionRate = 1.0f;
				particlesToSpawn = 0.0f;
				m_Velocity.set(0.0f,1.0f,0.0f);
				m_VelocityVariance.set(0.0f,0.0f,0.0f);
				m_Color.set(127,127,127);
				m_ColorVariance.set(0,0,0);
				m_LifeTime = 1.0f;
				m_LifeTimeVariance = 0.0f;
				m_Decay = 0.01f;
				m_DecayVariance = 0.0f;
				m_bEnable = true;
				onBirthCallBack = 0;
				onDeathCallBack = 0;
			}

			virtual void initializeParticle(Particle& p)
			{
				Point3 point;
				m_Transform.transform(point);

				p.m_X = point.x;
				p.m_Y = point.y;
				p.m_Z = point.z;

				p.m_PrevX = point.x;
				p.m_PrevY = point.y;
				p.m_PrevZ = point.z;

				Vector3 velocity(m_Velocity.x + m_VelocityVariance.x*((float)rand()/RAND_MAX - 0.5f),
					m_Velocity.y + m_VelocityVariance.y*((float)rand()/RAND_MAX- 0.5f),
					m_Velocity.z + m_VelocityVariance.z*((float)rand()/RAND_MAX- 0.5f));

				m_Transform.transform(velocity);

				p.m_Vx = velocity.x;
				p.m_Vy = velocity.y;
				p.m_Vz = velocity.z;

				p.m_R = m_Color.x + m_ColorVariance.x*(float)rand()/RAND_MAX;
				p.m_G = m_Color.y + m_ColorVariance.y*(float)rand()/RAND_MAX;
				p.m_B = m_Color.z + m_ColorVariance.z*(float)rand()/RAND_MAX;

				p.m_Life =	m_LifeTime + m_LifeTimeVariance*(float)rand()/RAND_MAX;

				p.m_Decay = m_Decay + m_DecayVariance*(float)rand()/RAND_MAX;
				p.m_isDead = false;

				if ( onBirthCallBack )
					onBirthCallBack(p,0);
			}



			virtual su32 emit(sf32 currentTime) = 0;

			virtual void modulateParticleProperties(sf32 currentTime)
			{
			}

			virtual void modulateEmissionRate(sf32 currentTime)
			{
			}
			// inline Methods
			inline void setTransform(Matrix4f& transform)
			{
				m_Transform = transform;
			}

			inline Matrix4f& getTransform()
			{
				return m_Transform;
			}

			inline void setEnable( sbool bEnable )
			{
				m_bEnable = bEnable;
			}

			inline void setEmissionRate(sf32 emissionRate)
			{
				m_EmissionRate = emissionRate;
			}

			inline sf32 getEmissionRate()
			{
				return m_EmissionRate;
			}

			inline void setVelocity(Vector3& V)
			{
				m_Velocity = V;
			}

			inline void setVelocityVariance(Vector3& var )
			{
				m_VelocityVariance = var;
			}

			inline void setColor( Color3& color )
			{
				m_Color = color;
			}

			inline void setColorVariance( Color3& var )
			{
				m_ColorVariance = var;
			}

			inline void setLifeTime( sf32 lifeTime)
			{
				m_LifeTime = lifeTime;
			}

			inline void setLifeTimeVariance( sf32 lifeTimeVar)
			{
				m_LifeTimeVariance = lifeTimeVar;
			}

			inline void setDecay( sf32 decay)
			{
				m_Decay = decay;
			}

			inline void setDecayVariance( sf32 decayVar)
			{
				m_DecayVariance = decayVar;
			}

			void registerOnBirthCallBack(PARTICLE_CALL_BACK callback)
			{
				onBirthCallBack = callback;
			}

			void registerOnDeathCallBack(PARTICLE_CALL_BACK callback)
			{
				onDeathCallBack = callback;
			}

			friend class ParticleNode;

		protected:

			inline su32 calculateSpawnParticles()
			{
				sf32 diffTime = (m_CurrentTime - m_PrevTime)/ 1000;

				modulateEmissionRate(m_CurrentTime);

				su32 numParticles;

				particlesToSpawn += diffTime * m_EmissionRate;

				if ( particlesToSpawn < 1 )
				{
					return 0;
				}
				else
				{
					numParticles = (si32)particlesToSpawn;
					particlesToSpawn -= numParticles;
				}
			}

			ParticleNode* pNode;
			sf32 m_EmissionRate;
			Matrix4f m_Transform;
			Vector3 m_Velocity;
			Vector3 m_VelocityVariance;
			Color3 m_Color;
			Color3 m_ColorVariance;
			sf32 m_LifeTime;
			sf32 m_LifeTimeVariance;
			sf32 m_Decay;
			sf32 m_DecayVariance;
			sf32 m_CurrentTime;
			sf32 m_PrevTime;
			sf32 particlesToSpawn;
			sbool m_bEnable;

			PARTICLE_CALL_BACK onBirthCallBack;
			PARTICLE_CALL_BACK onDeathCallBack;
		};

		class ParticleEmitterPoint: public ParticleEmitter
		{
		public:
			ParticleEmitterPoint()
			{
			}

			ParticleEmitterPoint(Point3& source):m_Source(source)
			{
			}

			virtual su32 emit(sf32 currentTime);

			virtual void modulateEmissionRate(sf32 currentTime);

		private:
			Point3 m_Source;
		};

		class ParticleEmitterLine: public ParticleEmitter
		{
		public:
			ParticleEmitterLine(sf32 length):m_Length(length)
			{
			}

			virtual su32 emit(sf32 currentTime);

			virtual void modulateEmissionRate(sf32 currentTime);

		private:
			sf32 m_Length;
		};

		class ParticleEmitterCylinder: public ParticleEmitter
		{
		public:
			ParticleEmitterCylinder(sf32 length, sf32 radius):m_Length(length),m_Radius1(radius),m_Radius2(radius)
			{
			}

			ParticleEmitterCylinder(sf32 length, sf32 radius1, sf32 radius2):m_Length(length),m_Radius1(radius1),m_Radius2(radius2)
			{
			}

			virtual su32 emit(sf32 currentTime);

			virtual void modulateEmissionRate(sf32 currentTime);

		private:
			sf32 m_Length, m_Radius1, m_Radius2;
		};

		class ParticleEmitterSphere: public ParticleEmitter
		{
		public:
			ParticleEmitterSphere(sf32 radius):m_Radius(radius)
			{
			}

			virtual su32 emit(sf32 currentTime);

			virtual void modulateEmissionRate(sf32 currentTime);

		private:
			sf32 m_Radius;
		};

		class ParticleAnimator
		{
		public:
			ParticleAnimator(ParticleNode* pNode):m_ParticleNode(pNode)
			{
			}

			virtual void animate( sf32 currentTime) = 0;

			friend class ParticleNode;

		protected:
			sf32 currTime, prevTime;
			ParticleNode* m_ParticleNode;
		};

		class ParticleGravityAnimator:public ParticleAnimator
		{
		public:

			ParticleGravityAnimator(ParticleNode* pNode):ParticleAnimator(pNode)
			{
				m_Gravity = -10.0f;
			}

			ParticleGravityAnimator(ParticleNode* pNode, sf32 gravity):ParticleAnimator(pNode),m_Gravity(gravity)
			{

			}

			inline void setGravity(sf32 gravity)
			{
				m_Gravity = gravity;
			}

			inline sf32 getGravity()
			{
				return m_Gravity;
			}

			virtual void animate( sf32 currentTime);

		private:
			sf32 m_Gravity;
		};

		class ParticleTornadoAnimator:public ParticleAnimator
		{
		public:

			ParticleTornadoAnimator(ParticleNode* pNode):ParticleAnimator(pNode)
			{
				m_AngularSpeed = 1.0f;
				m_TornadoAxis.set(0.0f,1.0f,0.0f);
			}

			ParticleTornadoAnimator(ParticleNode* pNode, sf32 angularSpeed):ParticleAnimator(pNode),m_AngularSpeed(angularSpeed)
			{
				m_TornadoAxis.set(0.0f,1.0f,0.0f);
				m_TornadoAxis.scale(m_AngularSpeed);
			}

			ParticleTornadoAnimator(ParticleNode* pNode, Vector3 tornadoAxis, sf32 angularSpeed):ParticleAnimator(pNode),m_AngularSpeed(angularSpeed),m_TornadoAxis(tornadoAxis)
			{
				m_TornadoAxis.normalize();
				m_TornadoAxis.scale(m_AngularSpeed);
			}
			inline void setAngularSpeed(sf32 angularVelocity)
			{
				m_AngularSpeed = angularVelocity;
			}

			inline sf32 getAngularSpeed()
			{
				return m_AngularSpeed;
			}

			inline void setTornadoAxis(Vector3& tornadoAxis)
			{
				m_TornadoAxis = tornadoAxis;
				m_TornadoAxis.normalize();
			}

			inline Vector3& getTornadoAxis()
			{
				return m_TornadoAxis;
			}

			virtual void animate( sf32 currentTime );

		private:

			sf32 m_AngularSpeed;
			Vector3 m_TornadoAxis;

		};


		class ParticleDiffuseAnimator:public ParticleAnimator
		{
		public:

			ParticleDiffuseAnimator(ParticleNode* pNode):ParticleAnimator(pNode),m_DiffuseFactor(1.0f)
			{
			}

			ParticleDiffuseAnimator(ParticleNode* pNode, sf32 diffuseFactor):ParticleAnimator(pNode),m_DiffuseFactor(diffuseFactor)
			{
			}

			void setDiffuseFactor(sf32 diffuseFactor)
			{
				m_DiffuseFactor = diffuseFactor;
			}

			sf32 getDiffuseFactor()
			{
				return m_DiffuseFactor;
			}

			virtual void animate( sf32 currentTime );

		private:
			sf32 m_DiffuseFactor;
		};

		class ParticleFlockAnimator:public ParticleAnimator
		{
		public:

			ParticleFlockAnimator(ParticleNode* pNode):ParticleAnimator(pNode)
			{
			}

			virtual void animate( sf32 currentTime );

		}; 

		class ParticleOrbitCentreAnimator:public ParticleAnimator
		{
		public:

			ParticleOrbitCentreAnimator(ParticleNode* pNode):ParticleAnimator(pNode),m_Strength(1.0f)
			{
			}

			ParticleOrbitCentreAnimator(ParticleNode* pNode, sf32 strength):ParticleAnimator(pNode),m_Strength(strength)
			{
			}

			inline void setCentre(Point3& centre)
			{
				m_Centre = centre;
			}

			inline Point3& getCentre()
			{
				return m_Centre;
			}

			virtual void animate( sf32 currentTime );
		private:
			sf32 m_Strength;
			Point3 m_Centre;
		}; 


		class ParticleTargetFollowAnimator:public ParticleAnimator
		{
		public:

			ParticleTargetFollowAnimator(ParticleNode* pNode):ParticleAnimator(pNode),m_Target(0.0f,0.0f,0.0f)
			{
			}

			ParticleTargetFollowAnimator(ParticleNode* pNode, Point3& target):ParticleAnimator(pNode),m_Target(target)
			{
			}

			inline void setTarget(Point3& target)
			{
				m_Target = target;
			}

			inline Point3& getTarget()
			{
				return m_Target;
			}

			virtual void animate( sf32 currentTime );
		private:
			Point3 m_Target;
		}; 
	}
}
#endif