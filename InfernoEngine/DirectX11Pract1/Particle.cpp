
#include "Particle.h"
#include "GameObjectManager.h"
#include "Helper.h"


Particle::Particle(const Particle& other)
{
	this->transform.position = other.transform.position;
	this->mVelocity = other.mVelocity;
	this->mAcceleration = other.mAcceleration;
	this->mAge = other.mAge;
	this->mBoundingSphere = other.mBoundingSphere;
	
}

void Particle::Update()
{
	this->mBoundingSphere.mCenter = this->transform.position;
}


ParticleEmitter::ParticleEmitter(){}
ParticleEmitter::~ParticleEmitter(){}

void ParticleEmitter::InitParticles( int numParticles, Vector3 minInitVelocity,Vector3 maxInitVelocity, float maxAgeInSeconds )
{

	this->mAge = maxAgeInSeconds;
	this->mMaxInitVelocity = maxInitVelocity;
	this->mMinInitVelocity = minInitVelocity;

	this->mParticlesList.clear();	// Clear all the old particles
	for ( int count = 0; count != numParticles; count++ )
	{
		this->mParticlesList.push_back(this->CreateParticle());
	}
	return;

}

void ParticleEmitter::GetParticleListToRender( std::vector<Particle*> &vecParticlesToRender )
{
	for (std::vector<Particle*>::iterator particleIt = mParticlesList.begin();particleIt != mParticlesList.end();particleIt++)
	{
		if ( (*particleIt)->mAge > 0.0f )
		{	
			// It's 'alive' so add it to the vector to render
			vecParticlesToRender.push_back( (*particleIt) );
		}
	}
}

void ParticleEmitter::SetDirectionOfForce(Vector3 direction)
{
	this->directionForce = direction;
}

void ParticleEmitter::Update( float deltaTime, bool bKeepRecylingParticles)
{
	// If the particle is 'alive' then update it with whatever (Euler, RK4, etc.)
	for ( std::vector< Particle* >::iterator itPart = this->mParticlesList.begin(); 
		itPart != this->mParticlesList.end(); itPart++ )
	{
		if ( (*itPart)->mAge > 0.0f )
		{	// Update it... 
			(*itPart)->mAcceleration = (*itPart)->mAcceleration + this->directionForce * deltaTime ;
			(*itPart)->mVelocity = (*itPart)->mVelocity + ( (*itPart)->mAcceleration * deltaTime );
			(*itPart)->transform.position = (*itPart)->transform.position + ( (*itPart)->mVelocity * deltaTime );
			(*itPart)->Update();
			// "Age" the particle
			(*itPart)->mAge -= deltaTime;
		}
		else
		{	// He's dead, Jim.
			 this->ResetParticle((*itPart));   //= this->CreateParticle();
		}
	}

	return;
}

Particle* ParticleEmitter::CreateParticle()
{
	Particle* tempParticle = new Particle;
	tempParticle->acclerationValue = 5;
	// Start particle at the same place as the emitter...
	tempParticle->transform.position = this->transform.position;
	// Pick a ra->dom age (from 50% of maxAge to maxAge);
	tempParticle->mAge = Helper::RandomRange( this->mAge/2.0f, this->mAge);
	// Pick a ra->dom acceleration
	tempParticle->mVelocity.x = Helper::RandomRange( this->mMinInitVelocity.x, this->mMaxInitVelocity.x );
	tempParticle->mVelocity.y = Helper::RandomRange( this->mMinInitVelocity.y, this->mMaxInitVelocity.y );
	tempParticle->mVelocity.z = Helper::RandomRange( this->mMinInitVelocity.z, this->mMaxInitVelocity.z );
	tempParticle->mBoundingSphere.mRadius = 7.0f;
	tempParticle->mBoundingSphere.mCenter = tempParticle->transform.position;
	tempParticle->mBoundingSphere.mNormal = Vector3::Zero;
	tempParticle->mAcceleration = Vector3::Up * tempParticle->acclerationValue;

	return tempParticle;

}

void ParticleEmitter::ResetParticle(Particle* particle)
{
	particle->transform.position = this->transform.position;
	// Pick -> random age (from 50% of maxAge to maxAge);
	particle->mAge = Helper::RandomRange( this->mAge/2.0f, this->mAge);
	// Pick -> random acceleration
	particle->mVelocity.x = Helper::RandomRange( this->mMinInitVelocity.x, this->mMaxInitVelocity.x );
	particle->mVelocity.y = Helper::RandomRange( this->mMinInitVelocity.y, this->mMaxInitVelocity.y );
	particle->mVelocity.z = Helper::RandomRange( this->mMinInitVelocity.z, this->mMaxInitVelocity.z );
	particle->mBoundingSphere.mRadius = 7.0f;
	particle->mBoundingSphere.mCenter = particle->transform.position;
	particle->mBoundingSphere.mNormal = Vector3::Zero;
	particle->mAcceleration = this->directionForce*particle->acclerationValue;
}


