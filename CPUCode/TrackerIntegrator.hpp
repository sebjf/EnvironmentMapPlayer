/*
 * TrackerIntegrator.hpp
 *
 *  Created on: 24 Jun 2016
 *      Author: sfriston
 */

#ifndef TRACKERINTEGRATOR_HPP_
#define TRACKERINTEGRATOR_HPP_

#include "PhaseSpaceTracker.hpp"
#include "Oculus.hpp"
#include "Stopwatch.hpp"

#define PI 3.14159265359

class TrackerIntegrator
{
	Vector3f up;
	Vector3f forward;
	vector<float> headUp;
	vector<float> headLookat;

	PhaseSpaceTracker* phasespaceTracker;
	Oculus* oculusTracker;

	/* We will use the oculus quaternion math library since that is the more complete one */
	Quatf offset;
	Quatf correction;
	Quatf orientation;

	Quatf previousphasespace;

	float phasespace_yaw;
	float current_yaw;
	float yaw_correction;

	Quatf phasespaceAveraged;

	int phasespaceSampleCounter;

	bool offsetSet;

	Stopwatch timer;
	double lastTime;

public:

	TrackerIntegrator(PhaseSpaceTracker* absolute, Oculus* relative)
	{
		phasespaceTracker = absolute;
		oculusTracker = relative;

		offset = Quatf::Quat();
		correction = Quatf::Quat();

		up = Vector3f(0,1,0);
		forward = Vector3f(0,0,-1);
		headUp.assign(3,0);
		headLookat.assign(3,0);

		phasespaceSampleCounter = false;

		offsetSet = false;

		yaw_correction = 0;
	}

	Quatf toQuatf(quaternion quat)
	{
		return Quatf::Quat(quat.x, quat.y, quat.z, quat.s);
	}

	void Update()
	{
		Quatf oculus = oculusTracker->GetOrientation();
		Quatf phasespace = toQuatf(phasespaceTracker->GetOrientation());

		float a;
		float b;
		float c;

		if(phasespaceTracker->GetValid() && phasespaceTracker->GetOrientationCond() > 12)	// 12 seems reasonably stable
		{
			if(phasespaceSampleCounter == 0){
				phasespaceAveraged = phasespace;
			}

			float x = 0.8;
			float y = 0.2;

			float angle = phasespace.Angle(phasespaceAveraged);

			phasespaceAveraged = (phasespaceAveraged * x) + (phasespace * y);
			phasespaceSampleCounter++;

		}

		if(!offsetSet)
		{
			Calibrate();
		}

		orientation = offset * oculus;

		//only update the yaw_correction when phasespaceAveraged has been updated (correctly), or else it may pull
		//an otherwise correct orientation to a very old value. 12 is high. there will not be too many valid samples.

		if(phasespaceTracker->GetValid() && phasespaceTracker->GetOrientationCond() > 12){

			orientation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&a, &b, &c);
			current_yaw = a;
			phasespaceAveraged.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&a, &b, &c);
			phasespace_yaw = a;

			float yaw_correction_n = (phasespace_yaw - current_yaw);

			if(fabs(yaw_correction_n) > PI)
			{
				yaw_correction_n = ((2.0f * PI) - fabs(yaw_correction_n)) * -1 * ((yaw_correction_n)/fabs(yaw_correction_n));
			}

			//integrate a max of one degree per second

			double delta = timer.getTimeInSeconds() - lastTime;
			if(delta > 0.01){
				float coef = 0.999;
				yaw_correction = (yaw_correction * coef) + (yaw_correction_n * (1 - coef));
				lastTime = timer.getTimeInSeconds();
			}
		}

		orientation = Quatf::Quat(up, yaw_correction) * orientation;

		//orientation = phasespaceAveraged;
	}

	void Calibrate()
	{
		/* The offset is basically the difference between the phasespace (which is always absolute) and the oculus (which starts
		 * at Identity no matter which way the actual device is facing) before the oculus begins to drift */

		if(phasespaceSampleCounter > 200)
		{
			timer.Restart();
			lastTime = timer.getTimeInSeconds();
			oculusTracker->Reset();
			yaw_correction = 0;
			offset = phasespaceAveraged;
			offsetSet = true;
			printf("Tracker Offset Calibrated\n");
		}
	}

	vector<float> GetHeadPosition()
	{
		return phasespaceTracker->GetHeadPosition();
	}

	vector<float> GetHeadLookat()
	{
		Vector3f lookat = orientation.Rotate(forward);
		lookat.Normalize();
		memcpy(headLookat.data(), &lookat, sizeof(float)* 3);
		return headLookat;
	}

	vector<float> GetHeadUp()
	{
		Vector3f u = orientation.Rotate(up);
		memcpy(headUp.data(), &u, sizeof(float)* 3);
		return headUp;
	}

};

#endif /* TRACKERINTEGRATOR_HPP_ */
