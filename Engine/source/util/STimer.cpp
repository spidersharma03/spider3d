#include "STimer.h"
#include <stdio.h>

namespace spider3d
{

//sf32 STimer::currentTime = 0.0f;
//sf32 STimer::startTime = 0.0f;
//sf32 STimer::diffTime = 0.0f;
//sf32 STimer::increasingDuration = 0.0f;
//sf32 STimer::decreasingDuration = 0.0f;
#ifdef WIN32
static LARGE_INTEGER HighPerformanceFreq;
static BOOL HighPerformanceTimerSupport = FALSE;
#endif;
	
STimer::STimer()
{
	initTimer();
}

STimer::STimer(su32 loopCount , sf32 increasingDuration)
{
	initTimer();
	STimer::increasingDuration = increasingDuration;
}

void STimer::initTimer()
{
	diffTime = 0.0f;
	decreasingDuration = 0.0f;

	increasingDuration = 5000;
	
#ifdef WIN32
	startTime = GetTickCount();
	HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
#endif
	currentTime = startTime;
}

void STimer::tick()
{
#ifdef WIN32
	LARGE_INTEGER nTime;
	
	if ( HighPerformanceTimerSupport )
	{
	  BOOL queriedOK = QueryPerformanceCounter(&nTime);
	  
	  currentTime = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

	}
	else
	{
	currentTime = GetTickCount();
	}
#endif
	
	static int count = 0;
	count++;
	if ( diffTime > increasingDuration )
	{
		fps = count*1000.0f/diffTime;
		printf("\nFPS = %f" , fps);
        count = 0;
		startTime = currentTime;
	}
	diffTime = ( currentTime - startTime );
}

void STimer::setCurrentTime(sf32 time)
{
	startTime = time;
}

sf32 STimer::getCurrentTime()
{
	sf32 time;
#ifdef WIN32
	LARGE_INTEGER nTime;

	if ( HighPerformanceTimerSupport )
	{
	  BOOL queriedOK = QueryPerformanceCounter(&nTime);
	  
	  time = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

	}
	else
	{
		time = GetTickCount();
	}
#endif
	return time;
}

void STimer::stop()
{
  
}

sf32 STimer::value()
{
  return diffTime / increasingDuration;
}

void STimer::setIncreasingDuration(sf32 increasingDuration)
{
	STimer::increasingDuration = increasingDuration;
}

sf32 STimer::getIncreasingDuration()
{
	return increasingDuration;
}

void STimer::setDecreasingDuration(sf32 decreasingDuration)
{
	STimer::decreasingDuration = decreasingDuration;
}

sf32 STimer::getDecreasingDuration()
{
	return decreasingDuration;
}

}