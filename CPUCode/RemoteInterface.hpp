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
	}

	Primitives* primitives;

private:
	pthread_t serverThread;
	vector<pthread_t> clientThreads;

public:
	void Start()
	{
		int result = pthread_create(&serverThread, NULL, &ListenThread, NULL);
		if(result != 0)
		{
			printf("Could not start server thread.");
		}
	}

private:
	int portno;

	void ListenThread(void* arg)
	{
		int sockfd, n;
		struct sockaddr_in serv_addr;

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			printf("ERROR opening socket");
		  	return;
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(portno);
		serv_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("ERROR binding socket");
			return;
		}

		listen(sockfd,5);

		while(1){
			int clilen = sizeof(cli_addr);
			struct sockaddr_in cli_addr;
			int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
			if (newsockfd < 0)
			{
				printf("ERROR accepting connection to socket");
				return;
			}

			pthread_t tid;
			pthread_create(&tid, NULL, &ServerThread, (void*)newsockfd);

			clientThreads.push_back(tid);
		}
	}

	void ServerThread(void* arg)
	{
		int sockfd = (int)arg;

		while(1)
		{
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s",buffer);
		}
	}


	~RemoteInterface()
	{

	}

	/* Blank the display by multiplying all pixels with a low constant */

	bool updateCoefficient = false;
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

	int primitive_index = 0;

	void Local_SetPrimitiveLocation(int i, float x, float y, float z)
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
