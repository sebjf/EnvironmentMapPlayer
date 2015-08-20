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
#include "Camera.hpp"

using namespace OVR;

class Oculus
{
	ovrHmdDesc HmdDesc;
	ovrHmd Hmd;
	bool initialised;



public:
	OVR::Vector3<float> m_up;
	OVR::Vector3<float> m_forward;

	OVR::Quatf m_orientation;
	double m_timeInSeconds;

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

		m_orientation = state.Recorded.Pose.Orientation;
		m_timeInSeconds = state.Recorded.TimeInSeconds;

		//because the y axis is inverted (i.e. the screen y counter starts at 0, but really its drawing down from the top) we must do a 180 degree turn around z
		//to flip the world
		OVR::Quat<float> oneEighty = OVR::Quat<float>(OVR::Vector3<float>(0,0,1),3.14f);

		m_up      = (oneEighty * m_orientation).Rotate(up);
		m_forward = (oneEighty * m_orientation).Rotate(forward);

	}

	Quatf GetOrientation()
	{
		return m_orientation;
	}

	double GetTimeInSeconds()
	{
		return m_timeInSeconds;
	}

	Camera::vector3 GetCameraForward()
	{
		return Camera::vector3(m_forward.x, m_forward.y, m_forward.z);
	}

	Camera::vector3 GetCameraUp()
	{
		return Camera::vector3(m_up.x, m_up.y, m_up.z);
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
