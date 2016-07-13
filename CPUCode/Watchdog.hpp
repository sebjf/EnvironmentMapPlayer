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
		working = false;
		lastPeriod = 0.0f;
		clock_gettime(CLOCK_REALTIME, &lastTime);
	}

	void Update()
	{
		clock_gettime(CLOCK_REALTIME, &currentTime);

		timespec d = tsSubtract(currentTime, lastTime);
		lastTime = currentTime;

		working = false;

		if(checking)
		{
			float lastPeriod = tsFloat(d);
			if(lastPeriod > threshold)
			{
				error = true;
			}
			if(lastPeriod > 0)
			{
				working = true;
			}
		}

		checking = true;
	}

	float GetStatus()
	{
		return working;
	}

	float GetPeriod()
	{
		return lastPeriod;
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

	bool working;
	bool error;
	float threshold;
	bool checking;
	timespec lastTime;
	timespec currentTime;

	volatile float lastPeriod;
};

#endif /* WATCHDOG_HPP_ */
