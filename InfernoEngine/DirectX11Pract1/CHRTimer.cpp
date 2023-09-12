#include "CHRTimer.h"
#include "windows.h"	// For high freq


float mTimeSinceStart = 0;
float mFixedTimeStep = 0.01f;
float mGlobalDeltaTime = 0;

CHRTimer::CHRTimer()
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;

	LARGE_INTEGER tempTime;
	
	QueryPerformanceFrequency( &tempTime ); 

	this->m_frequency = static_cast<uint64_t>( tempTime.QuadPart );
}

CHRTimer::~CHRTimer()
{
}

void CHRTimer::Start(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_startTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
}

void CHRTimer::Stop(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
}

void CHRTimer::Reset(void)
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;
}

float CHRTimer::GetElapsedSeconds(void)
{
	this->m_elapsedTime = this->m_stopTime - this->m_startTime;
	float elapsedTimeSec = static_cast<float>( this->m_elapsedTime ) / this->m_frequency;
	return elapsedTimeSec;
}

unsigned long long CHRTimer::GetFrequency(void)
{
	return this->m_frequency;
}