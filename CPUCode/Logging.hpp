/*
 * Logging.hpp
 *
 *	Requires OVR Math
 *
 *  Created on: 14 Aug 2015
 *      Author: sfriston
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <sys/time.h>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include "Camera.hpp"
#include "ts_util.h"
#include <time.h>

using namespace std;

class Logging
{
public:

	struct __attribute__((__packed__)) Record
	{
		float timestamp;
		float headposition[3];
		float headlookat[3];
		float leftfootposition[3];
		float rightfootposition[3];
		float participantid;
		float trialid;
		float flag;
	};

	Logging()
	{
		enable = false;
		locked = false;
		participantid = -1;
		trialid = -1;
		flag = -1;
		sampleRate = 0.002f;
		memset(&lastR,0,sizeof(struct Record));
		clock_gettime(CLOCK_REALTIME, &startTime);
	}

	void SetEnable(bool b)
	{
		enable = b;
	}

	void SetParticipantId(int id)
	{
		participantid = id;
	}

	void SetTrialId(int id)
	{
		trialid = id;
	}

	void SetFlag(float f)
	{
		flag = f;
	}

	float GetCurrentTimeInSeconds()
	{
		timespec currentTime;
		clock_gettime(CLOCK_REALTIME, &currentTime);
		return tsFloat(tsSubtract(currentTime, startTime));
	}

	Record GetRecord(double timeInSeconds)
	{
		vector<Logging::Record>::reverse_iterator rit = log.rbegin();
		for(; rit != log.rend(); ++rit)
		{
			if((*rit).timestamp <= timeInSeconds)
			{
				return (*rit);
			}
		}

		return *(log.begin());
	}

	void Update(vector<float> head, vector<float> lookat, vector<float> lfoot, vector<float> rfoot)
	{
		if(enable && !locked)
		{
			Record r;

			memcpy(r.headposition, head.data(), sizeof(float) * 3);
			memcpy(r.headlookat, lookat.data(), sizeof(float) * 3);
			memcpy(r.leftfootposition, lfoot.data(), sizeof(float) * 3);
			memcpy(r.rightfootposition, rfoot.data(), sizeof(float) * 3);

			r.participantid = participantid;
			r.trialid = trialid;
			r.flag = flag;

			r.timestamp = GetCurrentTimeInSeconds();

			// log at a rate of 1KHz
			if(r.timestamp - lastR.timestamp > sampleRate)
			{
				lastR = r;
				log.push_back(r);
			}
		}
	}

	void Clear()
	{
		if(!locked)
		{
			log.clear();
			memset(&lastR,0,sizeof(struct Record));
			clock_gettime(CLOCK_REALTIME, &startTime);
		}
	}

	/* Warning this is an expensive call and should not be called when a participant is active! */
	void GetLogPtr(char** data, int* length)
	{
		*data = (char*)log.data();
		*length = log.size() * sizeof(struct Logging::Record);
		locked = true;
	}

	void ReleaseLogPtr()
	{
		locked = false;
	}

	int GetParticipantId()
	{
		return participantid;
	}

	int GetTrialId()
	{
		return trialid;
	}

	int GetRecordCount()
	{
		return log.size();
	}

	bool GetState()
	{
		return enable & !locked;
	}

	bool GetLocked()
	{
		return locked;
	}

private:

	std::vector<Logging::Record> log;
	bool enable;
	bool locked;

	volatile float sampleRate;

	int participantid;
	int trialid;
	float flag;

	Record lastR;

	timespec startTime;
};

#endif /* LOGGING_H_ */
