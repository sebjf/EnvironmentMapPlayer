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
	};

	Logging()
	{
		enable = false;
		locked = false;
		participantid = -1;
		trialid = -1;
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

			memccpy(r.headposition, head.data(), sizeof(float), 3);
			memccpy(r.headlookat, lookat.data(), sizeof(float), 3);
			memccpy(r.leftfootposition, lfoot.data(), sizeof(float), 3);
			memccpy(r.rightfootposition, rfoot.data(), sizeof(float), 3);

			r.participantid = participantid;
			r.trialid = trialid;

			log.push_back(r);
		}
	}

	void Clear()
	{
		if(!locked)
		{
			log.clear();
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

private:

	std::vector<Logging::Record> log;
	bool enable;
	bool locked;

	int participantid;
	int trialid;
};

#endif /* LOGGING_H_ */
