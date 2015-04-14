/*
 * OculusHMD.hpp
 *
 *  Created on: Apr 7, 2015
 *      Author: demo
 */

#ifndef OCULUSHMD_HPP_
#define OCULUSHMD_HPP_

#include <LibOVR/Include/OVR.h>

/*
 * This was designed for the Oculus Linux SDK 0.3.2, as this does not require the HMD to be connected in order to use the tracker.
 *
 * 1. Download and copy the libovr.a library to /usr/lib and the Src and Include folders to /usr/include/LibOVR to install.
 * 2. Add the flags   -lovr -lXrandr -lGL -ludev    to the linker options in the makefile, and remember to install the development
 * versions of the libraries where necessary! (e.g. udev)
 * 3. Run the ConfigurePermissionsAndPackages - if only to install the udev rules.
 */

class Orientation
{
	float x;
	float y;
};

class MyBodyFrameHandler;

class OculusHMD
{
public:
	OculusHMD();
	~OculusHMD();

	Orientation GetOrientation();

private:
	ovrHmd 				m_HMD;
	ovrHmdDesc 			m_HMDDesc;
	OVR::SensorDevice* 	m_pSensor;
	MyBodyFrameHandler* m_pHandler;
};


#endif /* OCULUSHMD_HPP_ */
