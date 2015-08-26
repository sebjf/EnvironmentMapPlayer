/*
 * Stopwatch.hpp
 *
 *  Created on: 26 Aug 2015
 *      Author: sfriston
 */

#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

#include <time.h>

class Stopwatch
{
public:
	Stopwatch()
	{
	}

	timespec start;

	void Restart()
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}

	double getTimeInSeconds()
	{
		timespec current;
		clock_gettime(CLOCK_MONOTONIC,&current);
		return timespecToSeconds(diff(start,current));
	}
private:
	//http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
	timespec diff(timespec start, timespec end)
	{
		timespec temp;
		if ((end.tv_nsec-start.tv_nsec)<0) {
			temp.tv_sec = end.tv_sec-start.tv_sec-1;
			temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
		} else {
			temp.tv_sec = end.tv_sec-start.tv_sec;
			temp.tv_nsec = end.tv_nsec-start.tv_nsec;
		}
		return temp;
	}

	double timespecToSeconds(timespec t)
	{
		return t.tv_sec + ((double)t.tv_nsec / 1000000000.0f);
	}
};



#endif /* STOPWATCH_HPP_ */
