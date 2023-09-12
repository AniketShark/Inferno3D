#ifndef _CHRTimer_HG_
#define _CHRTimer_HG_
#include <cstdint>

// Used to do high resulution timing
// Used QueryPerformanceTimer() 
// http://msdn.microsoft.com/en-us/library/ms644905(VS.85).aspx

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
extern float mTimeSinceStart;
extern float mFixedTimeStep;
extern float mGlobalDeltaTime;

class CHRTimer
{
public:
	CHRTimer();
	virtual ~CHRTimer();
	void Start(void);
	void Stop(void);
	void Reset(void);
	float GetElapsedSeconds(void);	// Can also use __int64 (win32 specific)
	unsigned long long GetFrequency(void);	
private:
	uint64_t m_elapsedTime;	   
	uint64_t m_startTime;	
	uint64_t m_stopTime;	
	uint64_t m_frequency;	
};

#endif // _CHRTimer_HG_