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

#include "PhaseSpace/owl.h"
#include "PhaseSpace/owl_math.h"

#define X 0
#define Y 1
#define Z 2

class PhaseSpaceTracker
{
	struct state
	{
		float head_position[3];
		float head_orientation[4];
		float lfoot_position[3];
		float rfoot_position[3];
	};

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
		lfootTrackerId = 6;
		rfootTrackerId = 7;

		connected = false;

	}
	~PhaseSpaceTracker()
	{

	}

	void Connect()
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		bzero((char*)&serv_addr, sizeof(struct sockaddr_in));
		serv_addr.sin_family = AF_INET;

		inet_aton("128.16.6.125", &serv_addr.sin_addr);

		serv_addr.sin_port = htons(27015);

		if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in))<0)
		{
			printf("Could not connect to PhaseSpace forwarder");
		}
		else
		{
			connected = true;
		}
	}

	bool isConnected()
	{
		return connected;
	}

	void Update()
	{
		if(connected)
		{
			write(sockfd, &n, 1);

			struct state thestate;

			int toread = sizeof(struct state);
			while(toread > 0)
			{
				n = read(sockfd, &thestate, toread);
				toread -= n;
			}

			headPosition[0] = thestate.head_position[0] * 0.1f;
			headPosition[1] = thestate.head_position[1] * 0.1f;
			headPosition[2] = thestate.head_position[2] * -0.1f;

			float v[3] = { 0.0f,0.0f,1.0f };
			float lookat[3];
			mult_qvq((const float*)&thestate.head_orientation, v, lookat );
			headLookat[0] = headPosition[0] + lookat[0];
			headLookat[1] = headPosition[1] + lookat[1];
			headLookat[2] = headPosition[2] + lookat[2];

		}
	}

private:

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	bool connected;

	string	address;
	size_t flags;

	int headTrackerId;
	int lfootTrackerId;
	int rfootTrackerId;

	OWLRigid rigids[1];
	OWLMarker markers[2];

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
