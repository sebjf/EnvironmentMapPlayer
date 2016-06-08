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




class RemoteInterface
{
public:
	RemoteInterface(int port)
	{
		portno = port;

		updateCoefficient = false;
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
	}

private:
	int portno;

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

		delete arg;

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

		char buffer[256];
		int n;

		while(1)
		{
			bzero(buffer,256);
			n = read(sockfd,buffer,255);
			int err = errno;
			if (n < 0) {
				printf("ERROR reading from socket\n");
			}
			if(n == 0)
			{
				printf("Remote admin disconnected.\n");
				break; //socket is closed
			}
			printf("Here is the message: %s",buffer);
		}

		delete arg;

		return NULL;
	}

	/* Blank the display by multiplying all pixels with a low constant */

	bool updateCoefficient;
	float coefficient;

	void Local_SetBlank(float v)
	{
		//wait for the last update to be processed
		while(updateCoefficient){
		};

		coefficient = v;
		updateCoefficient = true;
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
		if(updateCoefficient)
		{
			primitives->SetShade(coefficient);
		}

		//set primitive location
		if(updatePrimitiveLocation)
		{
			primitives->SetPrimitiveCenter(primitive_index, primitiveLocation);
		}
	}

};

#endif /* REMOTEINTERFACE_HPP_ */
