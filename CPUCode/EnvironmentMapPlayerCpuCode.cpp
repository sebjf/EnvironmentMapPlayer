#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/input.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Camera.hpp"
#include "EnvironmentMap.hpp"
#include "SampleParameterMap.hpp"
#include "VirtualMonitor.h"
#include "Mouse.hpp"
#include "CharacterController.hpp"
#include "Logging.hpp"
#include "ArduinoLED.hpp"
#include "Stopwatch.hpp"

#define USEOCULUS

#ifdef USEOCULUS
#include "Oculus.hpp"
#endif

#define DEG2RAD	0.0174532925

bool run = true;

void int_handler(int s){
   run = false;
   exit(1);
}

int main(void)
{

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = int_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	/* set up arduino for sync indication */
	ArduinoLED led(true);

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
    //max_set_max_runnable_timing_score(maxfile, 1000);
	max_engine_t *engine = max_load(maxfile, "*");

	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Initialise environment map */

	EnvironmentMap environmentMap(engine, maxfile);
	environmentMap.LoadEnvironmentMap(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/lazarus_map.bmp");

	/* Initialise the sample parameter map */

	SampleParameterMap sampleParameterMap(engine, maxfile);
	sampleParameterMap.m_offset_in_bursts = environmentMap.GetMapSizeInBursts();
	sampleParameterMap.InitialiseMapFromFile(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/rayParameterMap.bin");

	/* ignore memory input on subsequent runs */

	max_ignore_lmem(act,"environment_map");

	/* Rendering parameters */

	max_set_uint64t(act,"RaySampleCommandGeneratorKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());
	max_set_uint64t(act,"RaySampleCommandGeneratorKernel","num_banks_used", sampleParameterMap.num_banks_used);
	max_set_uint64t(act,"RaySampleCommandGeneratorKernel","start_bank_num", sampleParameterMap.bank_start_num);
	max_set_uint64t(act,"RaySampleReaderKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());

	max_set_double(act, "RayCasterKernel", "ipd", 3.2f);

	float fov = 90.0f;
	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize_h", tan(DEG2RAD*(fov/2)));
	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize_v", tan(DEG2RAD*(fov/2)));
	max_set_double( act,"RayCasterKernel", "viewplane_viewdistance", 1);
	max_set_double( act,"RayCasterKernel", "viewplane_hres", 960);
	max_set_double( act,"RayCasterKernel", "viewplane_vres", 1080);

	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","num_banks_used", environmentMap.num_banks_used);
	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","start_bank_num", environmentMap.bank_start_num);
	max_set_uint64t(act,"MapSampleReaderKernel","backgroundColour", 0xF0F0F0);

	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","min_mip_level", 3);
	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","max_mip_level", 11);

	/* Video signal parameters */

	max_set_uint64t(act,"MaxVideoSignalKernel","HSyncPolarity",1);
	max_set_uint64t(act,"MaxVideoSignalKernel","VSyncPolarity",1);

	printf("Running on DFE...\n");

	max_run(engine, act);


	//Get a continuous stream and write to the virtual display when simulating

	VirtualMonitor monitor(maxfile);
	monitor.Connect(engine);

	/* Set up the input devices */

	Mouse mouse(false);
	int inclination = 0;
	int elevation = 0;

	CharacterController characterController("/dev/input/by-id/usb-LITEON_Technology_USB_Multimedia_Keyboard-event-kbd");
	characterController.set_position(0, 0, 0);

	/* Specify camera properties */

	Camera camera;
	camera.connect(engine);

	/* Get the Oculus */
#ifdef USEOCULUS
	Oculus oculus;
	Logging logger(oculus.Hmd);
#endif

	Stopwatch stopwatch;

	/* Begin the head tracking logging */

	printf("Press CTRL+C key to exit.\n");

	bool enablePlayback = true;
	bool startPlayback = false;
	bool enableInteractive = !enablePlayback;

	if(enablePlayback){
		logger.Load("/home/sfriston/Dropbox/Investigations/Rendering Experiment/Head Tracking Logs/HeadMotionMaster.csv");
	}

	double timeInSeconds = 0;

	while(run){

		monitor.Refresh();

		MouseDelta d = mouse.readMouse(false);
		__u16 keycode = characterController.update(); //character controller reads the keyboard and outputs any character read, whether or not it acted on it

		if(enableInteractive){
			camera.set_eye(characterController.position[0],characterController.position[1],characterController.position[2]);
		}

		if(d.changed()){
			inclination += -d.y;
			elevation += -d.x;
		}

#ifdef USEOCULUS

		switch(keycode)
		{
		case KEY_S:
			startPlayback = true;
			break;
		}

		if(enablePlayback && startPlayback)
		{
			static bool isFirstRun = true;

			//when we signal we are about to read the first head log, that should be point at which we start syncing the leds
			if(isFirstRun)
			{
				isFirstRun = false;
				stopwatch.Restart();
				led.On();
			}

			timeInSeconds = stopwatch.getTimeInSeconds();

			oculus.Update(logger.GetState(timeInSeconds));

			static float lastFiveHundredMsSegment = 0;
			float fiveHundredMsSegmentNum = floor(timeInSeconds / 0.5f);

			if(fiveHundredMsSegmentNum != lastFiveHundredMsSegment)
			{
				lastFiveHundredMsSegment = fiveHundredMsSegmentNum;
				led.Invert();
			}

			double logLength = logger.GetLastTime();
			if(timeInSeconds >= logLength)
			{
				isFirstRun = true;
				startPlayback = false;

				printf("total time: %f", stopwatch.getTimeInSeconds());
			}
		}
		else
		{
			oculus.Update();
		}

		camera.set_ovr(oculus.GetCameraForward(), oculus.GetCameraUp());

#else
		camera.set_lookat(inclination, elevation);
#endif

	}

	printf("Exiting...");

	max_unload(engine);

	printf("Done.\n");
	return 0;
}
