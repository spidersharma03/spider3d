#include "ParticleBase.h"
#include "ParticleNode.h"
#include <vector>
#include <math.h>

using namespace std;

namespace spider3d 
{
	namespace particlesystem
	{
		// This Emitter emits particle from a PointSource
		su32 ParticleEmitterPoint::emit(sf32 currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			su32 numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = pNode->m_ParticlePool;

			int nDeadParticles = pNode->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(su32 p=0; p<numParticles; p++)
			{
				Particle particle;

				initializeParticle(particle);

				particlePool.push_back(particle);
			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}


		void ParticleEmitterPoint::modulateEmissionRate(sf32 currentTime)
		{
			return;
			static int i = 0;
			int n1 = 150;
			int n2 = 200;
			i++;

			if ( i > n2 )
				i = 0;

			if ( i > n1 && i < n2 )
			{
				m_EmissionRate = 0.0f;
			}
			else
			{
				m_EmissionRate = 1500.0f;
			}

			//m_EmissionRate = 500 + ( 500.0f * sin(currentTime*0.0001f) );
		}

		// LINE SOURCE

		su32 ParticleEmitterLine::emit(sf32 currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			su32 numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = pNode->m_ParticlePool;

			int nDeadParticles = pNode->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(su32 p=0; p<numParticles; p++)
			{
				Particle particle;

				sf32 y  = (float)rand()/RAND_MAX;

				particle.m_Y = m_Length * y;

				sf32 vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				sf32 vy = 0.0f;//(float)rand()/RAND_MAX ;
				sf32 vz = 2.0f*(float)rand()/RAND_MAX - 1.0f;

				particle.m_isDead = false;
				particle.m_Life = 1.0f;
				particle.m_Decay = 0.0002f;
				particle.m_Vx = 0.001*vx;
				particle.m_Vy = 0.001*vy;
				particle.m_Vz = 0.001*vz;

				particle.m_X = 0.0f;
				particle.m_Z = 0.0f;

				initializeParticle(particle);

				particlePool.push_back(particle);

			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterLine::modulateEmissionRate(sf32 currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}

		// CYLINDRICAL SOURCE
		// LINE SOURCE

		su32 ParticleEmitterCylinder::emit(sf32 currentTime)
		{
			if( !m_bEnable ) return 0; 

			m_CurrentTime = currentTime;

			su32 numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = pNode->m_ParticlePool;

			int nDeadParticles = pNode->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(su32 p=0; p<numParticles; p++)
			{
				Particle particle;

				sf32 y		= (float)rand()/RAND_MAX;
				sf32 theta  = (float)rand()/RAND_MAX * M_PI * 2.0f;


				//sf32 vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				//sf32 vy = 0.0f;//(float)rand()/RAND_MAX ;
				//sf32 vz = 2.0f*(float)rand()/RAND_MAX - 1.0f;

				sf32 vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				sf32 vy = 0.0f;//(float)rand()/RAND_MAX ;
				sf32 vz = 2.0f*(float)rand()/RAND_MAX - 1.0f;

				particle.m_isDead = false;
				particle.m_Life = 1.0f;
				particle.m_Decay = 0.003f;

				sf32 radius = m_Radius1*(1-y) + y*m_Radius2;

				particle.m_Y = m_Length * y;
				particle.m_X = radius * cos(theta);
				particle.m_Z = radius * sin(theta);

				Vector3 R(particle.m_X,0.0,particle.m_Z);
				Vector3 m_TornadoAxis(0,1,0);
				Vector3 V = m_TornadoAxis.cross(R);

				particle.m_Vx = V.x;
				particle.m_Vy = 0.0f;
				particle.m_Vz = V.z;

				initializeParticle(particle);

				particlePool.push_back(particle);
			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterCylinder::modulateEmissionRate(sf32 currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}

		// SPHERICAL SOURCE

		su32 ParticleEmitterSphere::emit(sf32 currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			su32 numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = pNode->m_ParticlePool;

			int nDeadParticles = pNode->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(su32 p=0; p<numParticles; p++)
			{
				Particle particle;

				// Choose Random Spawn location
				sf32 theta	= (float)rand()/RAND_MAX * M_PI;
				sf32 phi    = (float)rand()/RAND_MAX * M_PI * 2.0f;

				// Choose Random Axis
				sf32 ax = (float)rand()/RAND_MAX ;
				sf32 ay = (float)rand()/RAND_MAX ;
				sf32 az = (float)rand()/RAND_MAX ;
				sf32 length = sqrt(ax*ax+ay*ay+az*az);

				if( length > 1e-15)
				{
					ax /= length;
					ay /= length;
					az /= length;
				}
				particle.m_isDead = false;
				particle.m_Life = 1.0f;
				particle.m_Decay = 0.000f;

				particle.m_Y = m_Radius * cos(theta);
				particle.m_X = m_Radius * sin(theta) * sin(phi);
				particle.m_Z = m_Radius * sin(theta) * cos(phi);

				Vector3 R(particle.m_X ,particle.m_Y,particle.m_Z );
				Vector3 m_TornadoAxis(ax,ay,az);
				Vector3 V = m_TornadoAxis.cross(R);

				particle.m_Vx = V.x;
				particle.m_Vy = V.y;
				particle.m_Vz = V.z;

				initializeParticle(particle);

				particlePool.push_back(particle);
			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterSphere::modulateEmissionRate(sf32 currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}
	}
}