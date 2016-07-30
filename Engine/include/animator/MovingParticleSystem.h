#pragma once

#include "Appearance.h"
#include "FileLoader.h"
#include "Image.h"

#include "ParticleBase.h"
#include "NodeAnimator.h"
#include "ParticleNode.h"
#include "EventListener.h"

using namespace spider3d::particlesystem;
using namespace spider3d::io;

namespace spider3d
{
	namespace scene
	{
		class SceneManager;
	}
}

class Rocket;

class MovingParticleSystem : public NodeAnimator
{
public:
	MovingParticleSystem(SceneManager* sceneManager ,Node* parent)
	{
		parentNode = parent;
		m_SceneManager = sceneManager;
		//parentNode->addAnimator(this);
		m_Velocity.x = 200.0f;
		m_Velocity.y = 0.0f;
		m_Velocity.z = 200.0f;
		m_Acceleration.x = 0.0f;
		m_Acceleration.y = -500.0f;
		m_LifeTime = 1.0f;
	    m_Decay = 0.0008f;
		createParticleNode();
	}
public:

	friend class Rocket;

	virtual ~MovingParticleSystem(void)
	{
	}

#define dt 0.001
#define m_Drag 10.0f

	void animate(sf32 time)
	{
		m_LifeTime -= m_Decay;
		m_ParticleNode->getParticleEmitter()->setEmissionRate(m_LifeTime*m_LifeTime*m_LifeTime*m_LifeTime*100.0f);
		if( m_LifeTime <= 0.0f )
		{
			m_ParticleNode->getParticleEmitter()->setEmissionRate(0);
			return;
		}
		m_Velocity.x += (m_Acceleration.x  - m_Drag * m_Velocity.x)* dt;
		m_Velocity.y += (m_Acceleration.y  - m_Drag * m_Velocity.y)* dt;
		m_Velocity.z += (m_Acceleration.z  - m_Drag * m_Velocity.z)* dt;

		m_Position.x += m_Velocity.x * dt;
		m_Position.y += m_Velocity.y * dt;
		m_Position.z += m_Velocity.z * dt;

		Matrix4f xf, rot;
		xf.setTranslation(Vector3(m_Position.x,m_Position.y,m_Position.z));
		m_ParticleNode->getParticleEmitter()->setTransform(xf);
	}

private:
	void createParticleNode()
	{
		m_ParticleNode = new ParticleNode(parentNode, m_SceneManager, 0, 160 );
		m_ParticleNode->setParticleShape(ParticleNode::EPS_POINT);
		//ParticleEmitter *emitter = new ParticleEmitterCylinder(2.0f , 0.2f, 1.0f);
		ParticleEmitter *emitter = new ParticleEmitterSphere(0.3f);
		//ParticleEmitter *emitter = new ParticleEmitterPoint(Point3(0.0f,0.0f,0.0f));
		emitter->setVelocity(Vector3(0.0f,0.0f,0.0f));
		emitter->setVelocityVariance(Vector3(0.8f,0.7f,0.7f));
		emitter->setColor(Color3(200,170,0));
		emitter->setColorVariance(Color3(50,10,0));
		emitter->setLifeTime(1.0f);
		emitter->setLifeTimeVariance(1.0f);
		emitter->setEmissionRate(100);
		//emitter->registerOnBirthCallBack(OnParticleBirth);
		//ParticleAnimator *particleAnimator = new ParticleGravityAnimator(m_ParticleNode,0.0f);
		ParticleAnimator *particleAnimator = new ParticleDiffuseAnimator(m_ParticleNode,2.0f);
		//ParticleAnimator *particleAnimator = new ParticleOrbitCentreAnimator(m_ParticleNode,1000.0f);
		m_ParticleNode->addParticleAnimator(particleAnimator);
		m_ParticleNode->setParticleEmitter(emitter);
		m_ParticleNode->getParticleEmitter()->setEnable(false);
		m_ParticleNode->setSortingEnable(true);

		Appearance *spriteApp = new Appearance();
		////spriteApp->setTextureUnitState(0,spriteTU);
		m_ParticleNode->setAppearance(spriteApp);
		spriteApp->getMaterial()._mLightingEnabled = false;
		spriteApp->getPointAttributes().size = 5.0f + (float)rand()/RAND_MAX - 0.5f;
		//spriteApp->getPointAttributes().antiAliased = true;

		spriteApp->getTransparencyAttributes().transparency = 0.1f;
		spriteApp->getTransparencyAttributes().srcBlendFactor = EBF_BLEND_SRC_ALPHA;
		spriteApp->getTransparencyAttributes().dstBlendFactor = EBF_BLEND_ONE;
		spriteApp->getTransparencyAttributes().mode = ETM_BLEND;

		TextureLoader *imageLoader = new 	TextureLoader("portal1.bmp",0);
		Image* sprite = imageLoader->getImage();
		TexturePointSprite *pointSpriteTexture = new TexturePointSprite( ETF_RGB , sprite->getWidth()  , sprite->getHeight() );
		Point3 attenuation(0.0001f,0.01f,0.01f);
		spriteApp->getPointAttributes().m_DistanceAttenuation = attenuation;
		spriteApp->getPointAttributes().m_MaximumSize = 10.0f;
		spriteApp->getPointAttributes().m_MinimumSize = 5.0f;
		pointSpriteTexture->setImage(sprite);
		TextureUnitState *spriteTU = new TextureUnitState(pointSpriteTexture);
		spriteApp->setTextureUnitState(0,spriteTU);
	}


private:
	Vector3 m_Velocity, m_Acceleration;
	Point3 m_Position;
	bool m_bEnabled;
	ParticleNode *m_ParticleNode;
	float m_EmissionRate;
    float m_LifeTime;
	float m_Decay;
	Node* parentNode;
	SceneManager* m_SceneManager;
};