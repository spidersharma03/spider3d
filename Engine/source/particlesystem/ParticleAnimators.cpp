#include "ParticleBase.h"
#include "ParticleNode.h"
#include <math.h>

namespace spider3d 
{
	namespace particlesystem
	{

		void ParticleGravityAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;

			if ( n < 0 ) return;

			for( su32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				p.m_Vx += 0.0f * dt;
				p.m_Vy += m_Gravity * dt;
				p.m_Vz += 0.0f * dt;

				p.m_PrevX = p.m_X;
				p.m_PrevY = p.m_Y;
				p.m_PrevZ = p.m_Z;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				if ( p.m_Y < 0.0f ) 
				{
					//p.m_Vy *= -0.8f;
					//p.m_Y = 0.0f;
				}
				m_ParticleNode->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

	
		void ParticleTornadoAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;

			for( su32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Find radius vector from the axis of tornado
				//Vector3 R(p.m_Vx,p.m_Vy,p.m_Vz);
				Vector3 R(p.m_X,0.0,p.m_Z);
				Vector3 V = m_TornadoAxis.cross(R);
				//Vector3 A = m_TornadoAxis.cross(V);

				sf32 random = 1.0f;//(float)rand()/RAND_MAX;
				
				p.m_Vx = V.x * random;
				p.m_Vy = V.y * random;
				p.m_Vz = V.z * random;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				p.m_R = p.m_Life * 255;
				p.m_G = 0;
				p.m_B = (1-p.m_Life) * 255;

				m_ParticleNode->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleDiffuseAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			//printf("%d\n",vecParticlePool.size());
			if ( n <=0 )return;

			for( si32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Random Force Generation
				sf32 fx = (float)rand()/RAND_MAX - 0.5f;
				sf32 fy = (float)rand()/RAND_MAX - 0.5f;
				sf32 fz = (float)rand()/RAND_MAX - 0.5f;
				
				p.m_Vx += m_DiffuseFactor*fx * dt;
				p.m_Vy += m_DiffuseFactor*fy * dt;
				p.m_Vz += m_DiffuseFactor*fz * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				//p.m_R = p.m_Life * 255;
				//p.m_G = 0;//(0.5-p.m_Life) * 255;
				//p.m_B = (1-p.m_Life) * 255;

				m_ParticleNode->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleFlockAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;

			for( su32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Random Force Generation
				sf32 fx = (float)rand()/RAND_MAX - 0.5f;
				sf32 fy = (float)rand()/RAND_MAX - 0.5f;
				sf32 fz = (float)rand()/RAND_MAX - 0.5f;
				
				p.m_Vx += fx * dt;
				p.m_Vy += fy * dt;
				p.m_Vz += fz * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				p.m_R = p.m_Life * 255;
				p.m_G = (0.5-p.m_Life) * 255;
				p.m_B = (1-p.m_Life) * 255;

				m_ParticleNode->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleOrbitCentreAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			
			if ( n < 0 ) return;
			for( su32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Find radius vector from the Centre of Orbit
				Vector3 R(p.m_X,p.m_Y ,p.m_Z);
				//Vector3 V(p.m_Vx,p.m_Vy,p.m_Vz);
				//Vector3 V = m_TornadoAxis.cross(R);
				//Vector3 m_TornadoAxis = R.cross(V);
				float Rsq = R.lengthSquared();
				R.normalize();
				//Vector3 A = m_TornadoAxis.cross(V);
				Vector3 A(R);
				A.scale ( -m_Strength/(Rsq + 0.1f) );

				p.m_Vx += A.x * dt;
				p.m_Vy += A.y * dt;
				p.m_Vz += A.z * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				//p.m_R = p.m_Life * 255;
				//p.m_G = 127;
				//p.m_B = (1-p.m_Life) * 255;

				m_ParticleNode->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleTargetFollowAnimator::animate( sf32 currentTime )
		{
			sf32 dt = (currentTime - prevTime)/1000.0f;
			m_ParticleNode->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_ParticleNode->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			
			if ( n < 0 ) return;

			float maxV = 1e-20f;

			for( su32 i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_Life -= p.m_Decay;

				if ( p.m_Life <= 1e-10 && !p.m_isDead )
				{
					p.m_Life = 0.0f;
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Move target Vector 
				m_Target.x = 2.0f * cos(currentTime*0.001f);
				m_Target.z = 2.0f * sin(currentTime*0.001f);

				// Find vector from the target
				Vector3 T(m_Target.x - p.m_X, m_Target.y - p.m_Y , m_Target.z - p.m_Z);
				float dist = T.length();
				T.normalize();
				float maxSpeed = 10.0f;
				Vector3 V(-p.m_Vx + maxSpeed*T.x, -p.m_Vy + maxSpeed*T.y, -p.m_Vz + maxSpeed*T.z); 
				// Find distance between target and particle
				float R = 1.0f;
				dist = dist < R ? R : dist;
				dist -= R;
				
				//dist = dist > 1.0f ? 1.0f : dist;
				V.scale(dist);
				//Vector3 D(-p.m_Vx,-p.m_Vy,-p.m_Vz);
				//D.scale(0.1f);
				//V.add(D);

				/*V.x = V.x > 1000.0f ? 1000.0f : V.x;
				V.y = V.y > 1000.0f ? 1000.0f : V.y;
				V.z = V.z > 1000.0f ? 1000.0f : V.z;*/
				//maxV = maxV < V.length() ? V.length() : maxV;

				p.m_Vx += V.x*dt;// > 10.0f ? V.x : V.x;
				p.m_Vy += V.y*dt;// > 10.0f ? V.y : V.y;
				p.m_Vz += V.z*dt;// > 10.0f ? V.z : V.z;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				p.m_Z += p.m_Vz * dt;
				
				p.m_R = p.m_Life * 255;
				p.m_G = 127;
				p.m_B = (1-p.m_Life) * 255;

				m_ParticleNode->m_NumActiveParticles++;
			}
			//printf("Max Velocity=%f\n", maxV);
			prevTime = currentTime;
		}


	}
}