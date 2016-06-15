/*
 * RemoteInterface.hpp
 *
 *  Created on: 7 Jun 2016
 *      Author: sfriston
 */

#ifndef REMOTEINTERFACE_HPP_
#define REMOTEINTERFACE_HPP_

#include "VirtualEnvironment.hpp"
#include "Logging.hpp"
#include "PhaseSpaceTracker.hpp"
#include "Watchdog.hpp"
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
	CMD_SETLATENCY = 10,
	CMD_CLEARCOPIEDLOG = 11,
	CMD_REQUESTGEOMETRY = 12,
	CMD_SETFLAG = 13,
};

class RemoteInterface
{
public:
	RemoteInterface(int port)
	{
		portno = port;

		setPrimitiveLocation = false;
		clearLog = false;
		requestLog = false;
		logDataCopied = false;

		participantid = -1;
		trialid = -1;
		flag = -1;
		currentShade = -1.0f;
		shade = 1.0f;

		primitive_index = 0;

		latency = 0;
	}

	~RemoteInterface()
	{

	}

	VirtualEnvironment* ve;
	Logging* log;
	PhaseSpaceTracker* tracker;
	Watchdog* watchdog;

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

	volatile float latency; // the latency set by the admin, the one variable that veinterface will actually maintain itself

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
		float heada;
		float headb;
		float headc;
		float watchdogerror;
		float watchdogperiod;
		float currentTime;
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
				n = read(sockfd,&buffer[readn],toread-readn);
				readn += n;

				if(n <= 0)
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
				args->interface->startLogging = true;
				break;
			case CMD_STOPLOGGING:
				args->interface->stopLogging = true;
				break;
			case CMD_DOWNLOADLOG:
				args->interface->Local_WriteLogData(sockfd);
				break;
			case CMD_CLEARLOG:
				args->interface->clearLog = true;
				break;
			case CMD_PARTICIPANTID:
				args->interface->participantid = (int)packet.param1;
				break;
			case CMD_TRIALID:
				args->interface->trialid = (int)packet.param1;
				break;
			case CMD_HMDVISIBILITY:
				args->interface->shade = packet.param1;
				break;
			case CMD_SETTARGET:
				args->interface->Local_SetPrimitiveLocation(packet.param1, packet.param2, packet.param3);
				break;
			case CMD_REQUESTSTATUS:
				write(sockfd, &args->interface->currentStatus, sizeof(struct StatusPacket));
				break;
			case CMD_SETLATENCY:
				args->interface->latency = packet.param1; //floats can probably be set safely from other threads, but even if they cant the user should not be able to see when its done so.
				break;
			case CMD_CLEARCOPIEDLOG:
				args->interface->logDataCopied = false;
				break;
			case CMD_REQUESTGEOMETRY:
				args->interface->Local_WritePrimitives(sockfd);
				break;
			case CMD_SETFLAG:
				args->interface->flag = packet.param1;
				break;
			}
		}

		delete args;
		return NULL;
	}

	/* Local flags for thread synchronization */

	// this is NOT the right way to do this, but we cant have anything that could surrender the main thread (mutexes) and simple atomics are only in c++11.

	volatile sig_atomic_t startLogging;
	volatile sig_atomic_t stopLogging;

	volatile float shade;

	float currentShade;

	volatile sig_atomic_t clearLog;
	volatile sig_atomic_t requestLog;
	volatile sig_atomic_t logDataCopied;

	char* logData;
	volatile int logDataLength;
	volatile int logDataRecordCount;

	volatile bool setPrimitiveLocation;
	volatile float primitiveLocationX;
	volatile float primitiveLocationY;
	volatile float primitiveLocationZ;

	volatile int participantid;
	volatile int trialid;
	volatile float flag;

	/* Updates the location of a nominated primitive */

	int primitive_index;

	void Local_SetPrimitiveLocation(float x, float y, float z)
	{
		while(setPrimitiveLocation){
		};

		primitiveLocationX = x;
		primitiveLocationY = y;
		primitiveLocationZ = z;

		setPrimitiveLocation = true;
	}

	void Local_WritePrimitives(int socket)
	{
		vector<Primitives::LoadedPrimitive> primitives = ve->GetPrimitives();

		int size = primitives.size();
		write(socket, &size, sizeof(int));

		for(unsigned int i = 0; i < primitives.size(); i++)
		{
			write(socket, &primitives[i], sizeof(struct Primitives::LoadedPrimitive));
		}
	}

	void Local_WriteLogData(int socket)
	{
		//if another client has requested the log data, wait. Neither requestLog or logDataCopied should be true when *this* instance of the call is made.
		while(requestLog || logDataCopied)
		{
			(void)0;
		}

		//requestLog is low, so main thread has finished copying it. logDataCopied is also low, so the other cilent has finished sending it to its administrator and freed the memory.

		requestLog = true;

		// now we have requested the data, wait until the main thread has copied it to a staging area

		while(!logDataCopied)
		{
			(void)0;
		}

		// transmit the data to the host

		int _logDataRecordCount = logDataRecordCount;

		write(socket, &_logDataRecordCount, sizeof(int));
		write(socket, logData, logDataLength);

		//wait until the data has been recieved before freeing

		char buf;
		read(socket,&buf,1);

		free(logData);
		logDataLength = -1;
		logDataRecordCount = -1;

		logDataCopied = false;
	}

	/* Called from the main low latency thread */
public:
	void Update()
	{
		if(startLogging)
		{
			log->SetEnable(true);
			startLogging = false;
		}

		if(stopLogging)
		{
			log->SetEnable(false);
			stopLogging = false;
		}

		log->SetParticipantId(participantid);
		log->SetTrialId(trialid);
		log->SetFlag(flag);

		// blank display
		if(shade != currentShade)
		{
			currentShade = shade;
			ve->SetShade(currentShade);
		}

		//set primitive location
		if(setPrimitiveLocation)
		{
			vector<float> primitiveLocation;
			float x = primitiveLocationX;
			float y = primitiveLocationY;
			float z = primitiveLocationZ;
			primitiveLocation.push_back(x);
			primitiveLocation.push_back(y);
			primitiveLocation.push_back(z);
			ve->SetPrimitiveCenter(primitive_index, primitiveLocation);
			setPrimitiveLocation = false;
		}

		if(clearLog)
		{
			log->Clear();
			clearLog = false;
		}

		if(requestLog)
		{
			// copy log data
			char* src;
			int length;
			log->GetLogPtr(&src, &length);

			logData = (char*)malloc(length);
			memcpy(logData, src, length);
			logDataLength = length;
			logDataRecordCount = log->GetRecordCount();

			log->ReleaseLogPtr();

			logDataCopied = true;
			requestLog = false;
		}

		currentStatus.hmdState = ve->GetShade();
		currentStatus.phasespaceState = tracker->isConnected();
		currentStatus.recordCount = log->GetRecordCount();
		currentStatus.participantId = log->GetParticipantId();
		currentStatus.trialID = log->GetTrialId();
		currentStatus.logging = log->GetState();
		currentStatus.latency = latency;

		vector<float> head = tracker->GetHeadPosition();
		currentStatus.headx = head[0];
		currentStatus.heady = head[1];
		currentStatus.headz = head[2];

		vector<float> lookat = tracker->GetHeadLookat();
		currentStatus.heada = lookat[0];
		currentStatus.headb = lookat[1];
		currentStatus.headc = lookat[2];

		currentStatus.watchdogerror = watchdog->GetError();
		watchdog->ClearError();
		currentStatus.watchdogperiod = watchdog->GetPeriod();

		currentStatus.currentTime = log->GetCurrentTimeInSeconds();
	}

};

#endif /* REMOTEINTERFACE_HPP_ */
