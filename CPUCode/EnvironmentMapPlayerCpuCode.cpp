#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/input.h>
#include <pthread.h>

#include "MaxSLiCInterface.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Camera.hpp"
#include "EnvironmentMap.hpp"
#include "RayParameterMap.hpp"
#include "Primitives.hpp"
#include "VirtualMonitor.h"
#include "Mouse.hpp"
#include "CharacterController.hpp"
#include "Logging.hpp"
#include "Stopwatch.hpp"
#include "RemoteInterface.hpp"
#include "PhaseSpaceTracker.hpp"
#include "VirtualEnvironment.hpp"
#include "Watchdog.hpp"
#include "Oculus.hpp"
#include "TrackerIntegrator.hpp"
#include "Delay.hpp"

//gnc#define USEOCULUS

#ifdef USEOCULUS
#include "Oculus.hpp"
#endif

#define DEG2RAD	0.0174532925

bool run = true;

void int_handler(int s){
	(void)s;
   run = false;
}

int main(int argc, const char** argv)
{
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = int_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	//make current thread realtime

	sched_param priority;
	priority.__sched_priority = sched_get_priority_max(SCHED_FIFO);
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &priority);


	Watchdog watchdog(0.001f);

	VirtualRoom room;
	room.root = "/home/sfriston/Dropbox/Investigations/Latency Gait and Distance Study/TFF Model/";
	room.Load(string("room.csv"));

	VirtualEnvironment ve;

	PhaseSpaceRigidBody headRigidBody(string("/home/sfriston/Dropbox/Investigations/Latency Gait and Distance Study/DK2Head.json"));
	PhaseSpaceTracker phasespace("128.16.8.253", &headRigidBody);
	phasespace.Connect();
	Oculus oculus;
	TrackerIntegrator tracker(&phasespace, &oculus);

	Logging log;
	Delay delay(0.001f,0.500f);

	RemoteInterface veinterface(9001);
	veinterface.ve = &ve;
	veinterface.log = &log;
	veinterface.tracker = &phasespace;
	veinterface.trackerIntegrator = &tracker;
	veinterface.watchdog = &watchdog;
	veinterface.Start();

	ve.Initialise(&room);

	/* Connect the virtual monitor */

	VirtualMonitor monitor(ve.getMaxfile());
	monitor.Connect(ve.getEngine());

	/* Set up the input devices */

	Mouse mouse(false);
	int inclination = 0;
	int elevation = 0;

	CharacterController characterController(false);	//no parameters opens the default keyboard

	ve.Start();

	printf("Press CTRL+C key to exit.\n");

	while(run){

		watchdog.Update();

		monitor.Refresh();

		veinterface.Update();
		phasespace.Update();
		oculus.Update();
		tracker.Update();

		delay.Update(tracker.GetHeadPosition(), tracker.GetHeadLookat(), tracker.GetHeadUp());
		Delay::Record trackerdata = delay.GetRecord(veinterface.latency * 0.001f);
		ve.getCamera()->set_eye(trackerdata.head);
		ve.getCamera()->set_lookat(trackerdata.lookat);
		ve.getCamera()->set_up(trackerdata.up);

		log.Update(tracker.GetHeadPosition(), tracker.GetHeadLookat(), phasespace.GetLeftFoot(), phasespace.GetRightFoot());

		MouseDelta d = mouse.readMouse(false);
		__u16 keycode = characterController.update(); //character controller reads the keyboard and outputs any character read, whether or not it acted on it

		if(d.changed()){
			inclination += -d.y;
			elevation += -d.x;
	//		ve.getCamera()->set_lookat(inclination, elevation);
		}

		switch(keycode)
		{
		case KEY_R:
			ve.Reset();
			break;
		case KEY_Q:
			run = false;
			break;
		}

	}

	printf("Exiting...");

	ve.Stop();

	printf("Done.\n");
	return 0;
}
