/*
 * PhaseSpaceTracker.hpp
 *
 *  Created on: Jun 9, 2016
 *      Author: demo
 */

#ifndef PHASESPACETRACKER_HPP_
#define PHASESPACETRACKER_HPP_

#include <vector>
#include <iostream>
#include <cstring>
#include "PhaseSpace/owl.h"
#include "PhaseSpace/owl_math.h"

#define X 0
#define Y 1
#define Z 2

class PhaseSpaceTracker
{
public:
	PhaseSpaceTracker(string Address)
	{
		headPosition.assign(3, 0);
		headLookat.assign(3,0);
		leftFoot.assign(3, 0);
		rightFoot.assign(3, 0);
		address = Address;

		flags = 0;

		headTrackerId = 0;
		feetTrackerId = 1;

	}
	~PhaseSpaceTracker()
	{

	}

	void Connect()
	{
		if(owlInit(address.c_str(), flags) < 0)
		{
			printf("Could not connect to PhaseSpace\n");
		}

	//	owlSetInteger(OWL_FRAME_BUFFER_SIZE, 0);

		// create trackers. There will be two trackers, one for the head, and one for the feet. The head tracker will be a rigid body, with the feet a collection of point trackers

	//	owlTrackeri(headTrackerId, OWL_CREATE, OWL_RIGID_TRACKER);



	}

	bool isConnected()
	{
		return false;
	}

	void Update()
	{

	}

private:

	string	address;
	size_t flags;

	int headTrackerId;
	int feetTrackerId;

	OWLRigid rigids[1];
	OWLMarker markers[2];

	float pose[7];

	vector<float> headPosition;
	vector<float> headLookat;
	vector<float> leftFoot;
	vector<float> rightFoot;

public:

	vector<float> GetHeadPosition()
	{
		return headPosition;
	}

	vector<float> GetHeadLookat()
	{
		float v[3] = { 0.0f,0.0f,1.0f };
		float lookat[3];
		mult_qvq(&pose[3], v, lookat );
		headLookat[0] = lookat[0];
		headLookat[1] = lookat[1];
		headLookat[2] = lookat[2];
		return headLookat;
	}

	vector<float> GetLeftFoot()
	{
		return leftFoot;
	}

	vector<float> GetRightFoot()
	{
		return rightFoot;
	}

private:

	void owl_print_error(const char *s, int n)
	{
	  if(n < 0) printf("%s: %d\n", s, n);
	  else if(n == OWL_NO_ERROR) printf("%s: No Error\n", s);
	  else if(n == OWL_INVALID_VALUE) printf("%s: Invalid Value\n", s);
	  else if(n == OWL_INVALID_ENUM) printf("%s: Invalid Enum\n", s);
	  else if(n == OWL_INVALID_OPERATION) printf("%s: Invalid Operation\n", s);
	  else printf("%s: 0x%x\n", s, n);
	}

};

#endif /* PHASESPACETRACKER_HPP_ */
