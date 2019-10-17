#include "Euler.h"


void EulerIntegrate(EulerState& state,float t,float dt)
{
	state.mPosition = state.mPosition + (state.mVelocity * dt);
	state.mVelocity = state.mVelocity + ((state.mForce *(1/ state.mMass)) * dt);
	t = t + dt;
}