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

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "PhaseSpace/owl.hpp"
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
		headUp.assign(3,0);
		address = Address;

		flags = 0;

		head_id = 0;
		leftFoot_id = 6;
		rightFoot_id = 7;

		connected = false;

	}
	~PhaseSpaceTracker()
	{
		if(owl.isOpen())
		{
			owl.done();
			owl.close();
		}
	}

	void Connect()
	{
		int result = owl.open(address);

		if (result == 0)
		{
			printf("ERROR: Timeout connecting to PhaseSpace\n");
			return;
		}
		if (result < 0)
		{
			printf("ERROR: Error connecting to PhaseSpace");
			return;
		}

		result = owl.initialize();

		if (result < 0)
		{
			printf("ERROR: Error initialising PhaseSpace");
			return;
		}

		connected = true;

		/* create the trackers */

		owl.createTracker(head_id, "rigid", "head");

		// assign markers to the rigid and specify local coordinates in millimeters (taken from wand.json)
		owl.assignMarker(head_id, 0, "0", "pos=-148.573,77.3388,33.2397");
		owl.assignMarker(head_id, 1, "1", "pos=-95.0524,-72.3504,-3.47924");
		owl.assignMarker(head_id, 2, "2", "pos=10.2652,-85.5011,-32.9739");
		owl.assignMarker(head_id, 3, "3", "pos=-3.54247,74.6796,-69.2826");
		owl.assignMarker(head_id, 4, "4", "pos=99.6408,-79.5838,17.3713");
		owl.assignMarker(head_id, 5, "5", "pos=137.262,85.4163,55.1239");


		owl.streaming(1);
	}

	bool isConnected()
	{
		return connected;
	}

	void Update()
	{
		if(owl.isOpen() && owl.property<int>("initialized"))
		{
			const OWL::Event *event = owl.nextEvent(0);	//no timeout - poll
			if (!event)
			{
				return;
			}

			if(event->type_id() == OWL::Type::ERROR)
			{
				cerr << event->name() << ": " << event->str() << endl;
				return;
			}

			if(event->type_id() == OWL::Type::FRAME)
			{
				if(event->find("markers", markers) > 0)
				{
					for (OWL::Markers::iterator m = markers.begin(); m != markers.end(); m++)
					{
						if (m->cond > 0)
						{
							if (m->id == leftFoot_id)
							{
								leftFoot[0] = m->x;
								leftFoot[0] = m->y;
								leftFoot[0] = m->z;
							}
							if (m->id == rightFoot_id)
							{
								rightFoot[0] = m->x;
								rightFoot[0] = m->y;
								rightFoot[0] = m->z;
							}
						}
					}
				}

				if (event->find("rigids", rigids) > 0)
				{
					for (OWL::Rigids::iterator m = rigids.begin(); m != rigids.end(); m++)
					{
						if (m->cond > 0)
						{
							if (m->id == head_id)
							{
								headPosition[0] = m->pose[0] * 0.1f;
								headPosition[1] = m->pose[1] * 0.1f;
								headPosition[2] = m->pose[2] * 0.1f;

								float* headOrientation = &m->pose[3];
								float forward[3] = { 0.0f,0.0f,-1.0f }; //-1 because rigid body was created 'backwards'
								float up[3] = { 0.0f,1.0f,0.0f };
								float lookat[3];
								owl_mult_qvq(headOrientation, forward, headLookat.data());

								owl_mult_qvq(headOrientation, up, headUp.data());

							//	headLookat[0] = lookat[0];
							//	headLookat[1] = lookat[1];
							//	headLookat[2] = lookat[2];
							}
						}
					}
				}
			}
		}
	}

private:

	OWL::Context owl;
	OWL::Markers markers;
	OWL::Rigids rigids;

	bool connected;

	string address;
	size_t flags;

	unsigned int head_id;
	unsigned int leftFoot_id;
	unsigned int rightFoot_id;

	vector<float> headPosition;
	vector<float> headLookat;
	vector<float> headUp;
	vector<float> leftFoot;
	vector<float> rightFoot;

public:

	vector<float> GetHeadPosition()
	{
		return headPosition;
	}

	vector<float> GetHeadUp()
	{
		return headUp;
	}

	vector<float> GetHeadLookat()
	{
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


};

#endif /* PHASESPACETRACKER_HPP_ */
