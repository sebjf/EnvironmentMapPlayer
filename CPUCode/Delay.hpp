/*
 * Delay.hpp
 *
 *  Created on: 27 Jun 2016
 *      Author: sfriston
 */

#ifndef DELAY_HPP_
#define DELAY_HPP_

#include <memory.h>
#include <time.h>
#include "ts_util.h"

class Delay
{
public:

	struct Record
	{
		vector<float> head;
		vector<float> lookat;
		vector<float> up;
		float timestamp;
	};

private:

	timespec startTime;

	float sampleRate;
	float max;
	int length;

	Record* buffer;
	Record* current;

public:
	Delay(float SampleRate, float Max)
	{
		sampleRate = SampleRate;
		max = Max;
		length = max/sampleRate;

		buffer = (Record*)malloc(length * sizeof(struct Record));
		current = buffer;

		clock_gettime(CLOCK_REALTIME, &startTime);
	}

	float GetCurrentTimeInSeconds()
	{
		timespec currentTime;
		clock_gettime(CLOCK_REALTIME, &currentTime);
		return tsFloat(tsSubtract(currentTime, startTime));
	}

	void Update(vector<float> head, vector<float> lookat, vector<float> up)
	{
		Record r;
		r.head = head;
		r.lookat = lookat;
		r.up = up;
		r.timestamp = GetCurrentTimeInSeconds();

		if(r.timestamp - current->timestamp > sampleRate)
		{
			current++;
			if(current >= buffer + length){
				current = buffer;
			}

			*current = r;
		}
	}

	Record GetRecord(float latency) // must be zero or negative
	{
		Record* search = current;

		for(int i = 0; i < length; i++)
		{
			if(current->timestamp - search->timestamp >= latency)
			{
				return *search;
			}
			search--;
			if(search < buffer)
			{
				search = buffer + length - 1;
			}
		}

		return *current;
	}

};


#endif /* DELAY_HPP_ */
