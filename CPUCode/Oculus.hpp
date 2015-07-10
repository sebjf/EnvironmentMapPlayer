/*
 * Oculus.hpp
 *
 *  Created on: 2 Jul 2015
 *      Author: sfriston
 */

#ifndef OCULUS_HPP_
#define OCULUS_HPP_

#include <OVR.h>
#include <Kernel/OVR_Math.h>

using namespace OVR;

class Oculus
{
	ovrHmdDesc HmdDesc;
	ovrHmd Hmd;
	bool initialised;



public:
	float roll, pitch, yaw;

	OVR::Vector3<float> m_up;
	OVR::Vector3<float> m_forward;

public:
	Oculus()
	{
		initialised = false;

		ovr_Initialize();

		Hmd = ovrHmd_Create(0);

		if (!Hmd)
		{
			return;
		}

		// Get more details about the HMD.
		ovrHmd_GetDesc(Hmd, &HmdDesc);
		ovrHmd_StartSensor(Hmd, ovrSensorCap_Orientation | ovrSensorCap_YawCorrection, 0);

		// Resets sensor orientation.
		ovrHmd_ResetSensor(Hmd);

		initialised = true;
	}

	void Update()
	{
		ovrSensorState state = ovrHmd_GetSensorState(Hmd, 0.0);

		OVR::Vector3<float> up = OVR::Vector3<float>(0,1,0);
		OVR::Vector3<float> forward = OVR::Vector3<float>(0,0,1);

		//because the y axis is inverted (i.e. the screen y counter starts at 0, but really its drawing down from the top) we must do a 180 degree turn around z
		//to flip the world
		OVR::Quat<float> oneEighty = OVR::Quat<float>(OVR::Vector3<float>(0,0,1),3.14f);

		m_up      = (oneEighty * OVR::Quat<float>(state.Recorded.Pose.Orientation)).Rotate(up);
		m_forward = (oneEighty * OVR::Quat<float>(state.Recorded.Pose.Orientation)).Rotate(forward);

	}

	void Reset()
	{
		ovrHmd_ResetSensor(Hmd);
	}

	~Oculus()
	{
		ovrHmd_StopSensor(Hmd);
	}
};

#endif /* OCULUS_HPP_ */
