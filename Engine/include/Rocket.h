#pragma once

#include "ParticleBase.h"
#include "NodeAnimator.h"
#include "ParticleNode.h"
#include "EventListener.h"
#include "MovingParticleSystem.h"

using namespace spider3d::scene;
using namespace spider3d::particlesystem;

namespace spider3d
{
	namespace scene
	{
		class SceneManager;
	}
}

class Rocket : public NodeAnimator, public EventListener
{
public:
	Rocket(SceneManager* sceneManager ,Node* parent);
public:
	virtual ~Rocket(void);
    
	virtual void animate(sf32 time);

	virtual sbool OnEvent(Event& sEvent);

private:
	
	void reset();
	void createRocketTrailParticleNode();
	void createRocketExplosionTrailParticleNode();

private:

	Vector3 m_Velocity, m_Acceleration;
	Point3 m_Position;
	bool m_bEnabled;
	ParticleNode *m_RocketTrailNode;
	float m_EmissionRate;

	Node* parentNode;
    SceneManager* m_SceneManager;
	int m_NumMovingTrails;
	bool bOnce;
	//ParticleNode *m_RocketTrailNode;
	vector<MovingParticleSystem*> m_vecExplosionTrailParticleNodes;
};
