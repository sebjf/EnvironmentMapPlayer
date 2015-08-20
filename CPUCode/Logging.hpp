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
#include <OVR.h>
#include "Camera.hpp"
#include "Oculus.hpp"

using namespace std;

class Logging
{
private:
	struct Record
	{
		double timestamp; //time in ms from first record
		float x;
		float y;
		float z;
		float w;
		float yaw;
		float pitch;
		float roll;
	};

	vector<Record> m_records;
	double m_starttime;

public:

	void Add(Oculus& oculus)
	{
		/* first read is invalid */

		if(oculus.GetTimeInSeconds() <= 0)
		{
			return;
		}

		/* get the starting time of this capture set */

		if(m_records.size() <= 0)
		{
			m_starttime = oculus.GetTimeInSeconds();
		}

		/* get the offset of the new reading */

		double timestamp = oculus.GetTimeInSeconds() - m_starttime;

		/* if this is not a new reading skip it */

		if(m_records.size() > 0)
		{
			float dt = timestamp - m_records.back().timestamp;

			/* filter invalid reads */

			if(dt <= 0)
			{
				return;
			}

			if(dt < 0.0001)
			{
				//the sensor cannot be this fast!
				//it is not clear how ovr sets the TimeInSeconds member, but it is possible it may be slightly different even when
				//no new values have been set, due to floating point error
				//in either case, we know these values aren't real sensor readings, so discard them
				printf("\nRead strange sensor dt of %f\n",dt);
				return;
			}
			if(dt > 10)
			{
				//the sensor cannot be this slow! (i.e. somewhere we have stored a starting value)
				printf("\nRead strange sensor dt of %f\n",dt);
				return;
			}
		}

		/* otherwise store the record */

		Record r;
		r.timestamp = timestamp;
		r.x = oculus.GetOrientation().x;
		r.y = oculus.GetOrientation().y;
		r.z = oculus.GetOrientation().z;
		r.w = oculus.GetOrientation().w;
		oculus.GetOrientation().GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&(r.yaw),&(r.pitch),&(r.roll));

		m_records.push_back(r);

	}

	void Save()
	{
		//open the file - we will write in csv plaintext. It wont be the fastest thing
		//to read in with the number of records we have, but we will only do it once
		FILE* fd = fopen("/home/sfriston/Dropbox/Investigations/Rendering Experiment/Head Tracking Logs/HeadLogs.csv","w");

		for(int i = 0; i < m_records.size(); i++){
			Record r = m_records[i];
			fprintf(fd, "%f,%f,%f,%f,%f,%f,%f,%f\n", (float)(r.timestamp), r.x, r.y, r.z, r.w, r.yaw, r.pitch, r.roll);
		}

		fclose(fd);

		Reset();
	}

	void Reset()
	{
		m_records = vector<Record>();
	}

	void Instrument()
	{
		/* print some properties of the log */

		float avg_delta = 0;
		float max_delta = 0;
		float min_delta = 0;

		float avg_delta_acc = 0;
		float avg_delta_cnt = 0;

		for(int i = 1; i < m_records.size(); i++)
		{
			float dt = m_records[i].timestamp - m_records[i-1].timestamp;

			if(dt > max_delta){
				max_delta = dt;
			}

			if(dt < min_delta){
				min_delta = dt;
			}

			avg_delta_acc += dt;
			avg_delta_cnt ++;
		}

		avg_delta = avg_delta_acc / avg_delta_cnt;

		printf("\nTime Deltas: Avg: %f, Min %f, Max %f, Total Records: %i\n", avg_delta, min_delta, max_delta, (int)avg_delta_cnt);
	}


};

#endif /* LOGGING_H_ */
