/*
 * RemoteInterface.hpp
 *
 *  Created on: 7 Jun 2016
 *      Author: sfriston
 */

#ifndef REMOTEINTERFACE_HPP_
#define REMOTEINTERFACE_HPP_

#include "Primitives.hpp"
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


enum Commands : char {
	CMD_STARTLOGGING = 1,
	CMD_STOPLOGGING = 2,
	CMD_DOWNLOADLOG = 3,
	CMD_CLEARLOG = 4,
	CMD_PARTICIPANTID = 5,
	CMD_TRIALID = 6,
	CMD_HMDVISIBILITY = 7,
	CMD_SETTARGET = 8,
	CMD_REQUESTSTATUS = 9,
};

class RemoteInterface
{
public:
	RemoteInterface(int port)
	{
		portno = port;

		setShade = false;
		updatePrimitiveLocation = false;

		primitive_index = 0;
	}

	~RemoteInterface()
	{

	}

	Primitives* primitives;

private:
	pthread_t serverThread;
	vector<pthread_t> clientThreads;

public:
	void Start()
	{
		ListenThreadArgs* args = new ListenThreadArgs();
		args->interface = this;

		int result = pthread_create(&serverThread, NULL, &ListenThread, args);
		if(result != 0)
		{
			printf("Could not start server thread.");
		}

		bzero(&currentStatus, sizeof(struct StatusPacket));
	}

private:
	int portno;

	struct __attribute__((__packed__)) CommandPacket
	{
		Commands command;
		float param1;
		float param2;
		float param3;
	};

	struct __attribute__((__packed__)) StatusPacket
	{
		float hmdState;
		float phasespaceState;
		float recordCount;
		float participantId;
		float trialID;
		float latency;
		float logging;
		float headx;
		float heady;
		float headz;
	};

	struct StatusPacket currentStatus;

	class ListenThreadArgs
	{
	public:
		RemoteInterface* interface;
	};

	static void* ListenThread(void* arg)
	{
		ListenThreadArgs* args = (ListenThreadArgs*)arg;

		int sockfd;
		struct sockaddr_in serv_addr;

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			printf("ERROR opening socket\n");
		  	return NULL;
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(args->interface->portno);
		serv_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("ERROR binding socket\n");
			return NULL;
		}

		listen(sockfd,5);

		while(1){
			struct sockaddr_in cli_addr;
			__socklen_t clilen = sizeof(cli_addr);
			int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
			if (newsockfd < 0)
			{
				printf("ERROR accepting connection to socket\n");
				return NULL;
			}

			printf("Remote admin connected.\n");

			ServerThreadArgs* server_args = new ServerThreadArgs();
			server_args->interface = args->interface;
			server_args->newsockfd = newsockfd;

			pthread_t tid;
			pthread_create(&tid, NULL, &ServerThread, server_args);

			args->interface->clientThreads.push_back(tid);
		}

		delete args;
		return NULL;
	}

	class ServerThreadArgs
	{
	public:
		RemoteInterface* interface;
		int newsockfd;
	};

	static void* ServerThread(void* arg)
	{
		ServerThreadArgs* args = (ServerThreadArgs*)arg;

		int sockfd = args->newsockfd;

		CommandPacket packet;

		char* buffer = (char*)&packet;
		int n;
		int toread = sizeof(struct CommandPacket);

		while(1)
		{
			int readn = 0;
			do{
				n = read(sockfd,&buffer[readn],toread);
				readn += n;

				if (n < 0) {
					printf("ERROR reading from socket\n");
				}
				if(n == 0)
				{
					printf("Remote admin disconnected.\n");

					//socket is closed
					delete args;
					return NULL;
				}

			}while(readn < toread);


			switch(packet.command)
			{
			case CMD_STARTLOGGING:
				break;
			case CMD_STOPLOGGING:
				break;
			case CMD_DOWNLOADLOG:
				break;
			case CMD_CLEARLOG:
				break;
			case CMD_PARTICIPANTID:
				break;
			case CMD_TRIALID:
				break;
			case CMD_HMDVISIBILITY:
				args->interface->SetShade(packet.param1);
				break;
			case CMD_SETTARGET:
				break;
			case CMD_REQUESTSTATUS:
				write(sockfd, &args->interface->currentStatus, sizeof(struct StatusPacket));
				break;
			}
		}

		delete args;
		return NULL;
	}

	/* Local flags for thread synchronization */

	bool setShade;


	/* Other variables for communicating between threads (will be locked by flags above) */

	float shade;

	/* Blank the display by multiplying all pixels with a low constant */

	void SetShade(float v)
	{
		//wait for the last update to be processed
		while(setShade){
		};

		shade = v;
		setShade = true;
	}


	/* Updates the location of a nominated primitive */

	bool updatePrimitiveLocation;
	vector<float> primitiveLocation;

	int primitive_index;

	void Local_SetPrimitiveLocation(float x, float y, float z)
	{
		while(updatePrimitiveLocation){
		};

		primitiveLocation.clear();
		primitiveLocation.push_back(x);
		primitiveLocation.push_back(y);
		primitiveLocation.push_back(z);

		updatePrimitiveLocation = true;
	}

	/* Called from the main low latency thread */
public:
	void Main_Update()
	{
		// blank display
		if(setShade)
		{
			primitives->SetShade(shade);
			setShade = false;
		}

		//set primitive location
		if(updatePrimitiveLocation)
		{
			primitives->SetPrimitiveCenter(primitive_index, primitiveLocation);
		}

		currentStatus.hmdState = primitives->GetShade();
		currentStatus.latency++;
	}

};

#endif /* REMOTEINTERFACE_HPP_ */
