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

	bool offsetSet;

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

		offsetSet = false;
	}

	Quatf toQuatf(quaternion quat)
	{
		return Quatf::Quat(quat.x, quat.y, quat.z, quat.s);
	}

	void Update()
	{
		Quatf oculus = oculusTracker->GetOrientation();
		Quatf phasespace = toQuatf(phasespaceTracker->GetOrientation());

		if(!offsetSet)
		{
			/* The offset is basically the difference between the phasespace (which is always absolute) and the oculus (which starts
			 * at Identity no matter which way the actual device is facing) before the oculus begins to drift */

			offset = phasespace; //if the oculus starts at I, then the offset is just the rotation that takes I to the phasespace

			offsetSet = phasespaceTracker->GetValid();
		}

		orientation = offset * oculus;
	}

	vector<float> GetHeadPosition()
	{
		return phasespaceTracker->GetHeadPosition();
	}

	vector<float> GetHeadLookat()
	{
		Vector3f lookat = orientation.Rotate(forward);
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
