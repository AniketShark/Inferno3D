#ifndef Particle_HG_
#define Particle_HG_

#include <vector>
#include "GameObject.h"
#include "Vector.h"
#include "Model.h"
#include "Sphere.h"


class Particle : public GameObject
{
public:
	Particle(){

		AddComponent(RenderComp);
		mVelocity = Vector3::Zero;
		mAcceleration = Vector3::Zero;
		mAge = 4;
		renderer->LoadMesh("UVSphere.ply");
		transform.scale = Vector3::One*2;
		renderer->SetPixelShader(g_pPixelShaderDiscardTransperacy);
		renderer->SetRasterizerState(g_pRS_SolidCullBack);
	};
	~Particle() {};
	Particle(const Particle&);
	Vector3 mVelocity;
	Vector3 mAcceleration;
	float acclerationValue;
	float mAge;
	MySphere mBoundingSphere;

	void Update();
};

class ParticleEmitter : public GameObject
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	Vector3 directionForce;// aka 'gravity' or whatever
	void InitParticles( int numParticles, Vector3 minInitVelocity, 	Vector3 maxInitVelocity, float maxAgeInSeconds );
	void Update( float deltaTime, bool bKeepRecylingParticles = true );
	void SetDirectionOfForce(Vector3 direction);
	Particle* CreateParticle( void );
	// Draw them! 
	void GetParticleListToRender( std::vector<Particle*> &vecParticlesToRender );
	void ResetParticle(Particle* particle);
	std::vector< Particle* > mParticlesList;
	Vector3 mMinInitVelocity;
	Vector3 mMaxInitVelocity;
	float mAge;
private:
	
	
};

#endif
