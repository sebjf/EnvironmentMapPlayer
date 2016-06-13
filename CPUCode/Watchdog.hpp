/*
 * Watchdog.hpp
 *
 *  Created on: Jun 13, 2016
 *      Author: demo
 */

#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

#include "ts_util.h"
#include <time.h>

class Watchdog
{
public:
	Watchdog(float warningThreshold)
	{
		threshold = warningThreshold;
		checking = false;
		clock_gettime(CLOCK_REALTIME, &lastTime);
	}

	void Update()
	{
		clock_gettime(CLOCK_REALTIME, &currentTime);

		timespec d = tsSubtract(currentTime, lastTime);
		lastTime = currentTime;

		if(checking)
		{
			if(tsFloat(d) > threshold)
			{
				error = true;
			}
		}

		checking = true;
	}

	bool GetError()
	{
		return error;
	}

	void ClearError()
	{
		error = false;
	}

private:

	bool error;
	float threshold;
	bool checking;
	timespec lastTime;
	timespec currentTime;

};

#endif /* WATCHDOG_HPP_ */
