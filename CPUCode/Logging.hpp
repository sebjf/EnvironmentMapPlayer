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
#include <OVR.h>
#include <OVR_CAPI.h>
#include "Camera.hpp"
#include "Oculus.hpp"

using namespace std;
using namespace OVR;

class Logging
{
public:

	struct Record
	{
		double timestamp;
		float x;
		float y;
		float z;
		float w;
		float yaw;
		float pitch;
		float roll;
	};

	Logging(ovrHmd hmd)
	{
		m_HMD = hmd;
		m_locked = false;
	}

	void Update()
	{
		if(m_locked)
		{
			return;
		}

		ovrSensorState state = ovrHmd_GetSensorState(m_HMD, 0.0);

		static double lasttime = 0;

		if(state.Recorded.TimeInSeconds == lasttime)
		{
			return;
		}

		lasttime = state.Recorded.TimeInSeconds;

		Record r;
		r.timestamp = state.Recorded.TimeInSeconds;
		r.x = state.Recorded.Pose.Orientation.x;
		r.y = state.Recorded.Pose.Orientation.y;
		r.z = state.Recorded.Pose.Orientation.z;
		r.w = state.Recorded.Pose.Orientation.w;

		((OVR::Quatf)state.Recorded.Pose.Orientation).GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&(r.yaw), &(r.pitch), &(r.roll));

		log.push_back(r);
	}

	OVR::Quatf GetState(double timeInSeconds)
	{
		for(unsigned int i = 0; i < log.size(); i++)
		{
			double offset = log[i].timestamp - log.front().timestamp;
			if(offset > timeInSeconds)
			{
				Record r = log[i-1];
				return OVR::Quatf(r.x,r.y,r.z,r.w);
			}
		}

		return OVR::Quatf(log.back().x,log.back().y,log.back().z,log.back().w);
	}

	double GetLastTime()
	{
		return log.back().timestamp - log.front().timestamp;
	}

	void Reset()
	{
		if(m_locked)
		{
			return;
		}

		log = std::vector<Logging::Record>();
	}

	void Save()
	{
		if(m_locked)
		{
			return;
		}

		std::ofstream file;
		file.open("C:\\HeadLogs\\Log.csv",std::ios::trunc);

		for(unsigned int i = 0; i < log.size(); i++)
		{
			Record r = log[i];
			file << std::fixed << r.timestamp << "," << r.x << "," << r.y << "," << r.z << "," << r.w << "," << r.roll << "," << r.pitch << "," << r.yaw <<  "\n";
		}

		file.close();
		Reset();
	}

	void Load(std::string filename)
	{
		Reset();
		ifstream file;
		file.open(filename.c_str(),ios::in);
		string item;
		while(file.good())
		{
			Record r;
			std::getline(file, item, ','); r.timestamp = strtod(item.c_str(), NULL);

			if(item.length() <= 0)
				break;

			std::getline(file, item, ','); r.x = strtod(item.c_str(), NULL);
			std::getline(file, item, ','); r.y = strtod(item.c_str(), NULL);
			std::getline(file, item, ','); r.z = strtod(item.c_str(), NULL);
			std::getline(file, item, ','); r.w = strtod(item.c_str(), NULL);
			std::getline(file, item, ','); r.yaw = strtod(item.c_str(), NULL);
			std::getline(file, item, ','); r.pitch = strtod(item.c_str(), NULL);
			std::getline(file, item); r.roll = strtod(item.c_str(), NULL);
			log.push_back(r);
		}
		m_locked = true;
		file.close();
	}

	std::vector<Logging::Record> log;
	bool m_locked;

private:
	ovrHmd m_HMD;

};

#endif /* LOGGING_H_ */
