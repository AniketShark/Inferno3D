
#include <stdio.h>
#include <math.h>

#include <iostream>

#include "RK4.h"

// F=ma -- This function simulates Hooke's law (aka springs)
//float acceleration(const State &state, float t)
//{
//	const float k = 10;
//	const float b = 1;
//	return - k*state.x - b*state.v;
//}

// We'll instead, change THIS function to simulate gravity.
Vector3 acceleration(State &state, float t)
{
	//const float k = 10;
	//const float b = 1;
	//return - k*state.x - b*state.v;

	return Vector3::Up *-9.81f;
}

// Straight up implicit or explicit Euler...
Derivative evaluate(State &initial, float t)
{
	Derivative output;
	output.mVelocity = initial.mVelocity;		// mVelocity = derive of location over time
	output.mForce = acceleration(initial, t);
	return output;
}

Derivative evaluate(State &initial, float t, float dt, Derivative &d)
{
	State state;
	state.mPosition = initial.mPosition + d.mVelocity * dt;
	state.mVelocity = initial.mVelocity + d.mForce * dt;
	Derivative output;
	output.mVelocity = state.mVelocity;
	output.mForce = acceleration(state, t + dt);
	return output;
}

// WE DON'T TOUCH THIS....
void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	Vector3 dxdt =  (a.mVelocity +  (b.mVelocity + c.mVelocity) * 2.0f  + d.mVelocity) * (1.0f/6.0f);
	Vector3 dvdt =  (a.mForce + (b.mForce + c.mForce) * 2.0f + d.mForce) * (1.0f/6.0f);

	state.mPosition = state.mPosition + dxdt * dt;
	state.mVelocity = state.mVelocity + dvdt * dt;
}