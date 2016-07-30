#ifndef _STIMER_H

#define _STIMER_H

#include "spiderDataTypes.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace spider3d
{

class STimer
{
public:

	void start();

	void stop();

    void tick();

	sf32 value();
    
	void setCurrentTime(sf32 time);

	sf32 getCurrentTime();

	sf32 getIncreasingDuration();

	sf32 getDecreasingDuration();
	
	void setIncreasingDuration(sf32);

	void setDecreasingDuration(sf32);

	STimer();

	STimer(su32 , sf32);

	sf32 fps;

private:

	sf32 currentTime , startTime;
    sf32 diffTime;
	su32 loopCount;
	sf32 increasingDuration;
	sf32 decreasingDuration;

	void initTimer();


};

}

#endif