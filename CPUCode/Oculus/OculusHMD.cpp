/*
 * OculusHMD.cpp
 *
 *  Created on: Apr 7, 2015
 *      Author: demo
 */

#include "OculusHMD.hpp"
#include <LibOVR/Src/CAPI/CAPI_HMDState.h>
#include <iostream>

class MyBodyFrameHandler : public OVR::MessageHandler
{
public:
	virtual void OnMessage(const OVR::Message& msg)
	{
		if (msg.Type == OVR::Message_BodyFrame)
		{
			OVR::MessageBodyFrame frame = static_cast<const OVR::MessageBodyFrame&>(msg);
			std::cout << "Captured: " << "\r";
		}
	}

};

OculusHMD::OculusHMD()
{
	/* Find and connect to an HMD */

	ovr_Initialize();

	int i = ovrHmd_Detect();

	m_HMD = ovrHmd_Create(0);

	if(!m_HMD){
		std::cout << "Unable to find an HMD." << std::endl;
		return;
	}

	ovrHmd_GetDesc(m_HMD, &m_HMDDesc);


	/* Attach the sensor handler which will read the tracking data */

	/* Get the Sensor object, to which we will add our own message handler */

	OVR::CAPI::HMDState* p = (OVR::CAPI::HMDState*)m_HMD;
	m_pSensor = p->pHMD->GetSensor();
	m_pSensor->SetReportRate(1000);

	/* Create and add our message handler - the sensor will call into this with new data */
	m_pHandler = new MyBodyFrameHandler();
	m_pSensor->AddMessageHandler(m_pHandler);

}

OculusHMD::~OculusHMD()
{
	m_pHandler->RemoveHandlerFromDevices();
	m_pSensor->Release();
	ovrHmd_Destroy(m_HMD);
    ovr_Shutdown();
}

Orientation OculusHMD::GetOrientation()
{
	Orientation t;

	return t;
}
