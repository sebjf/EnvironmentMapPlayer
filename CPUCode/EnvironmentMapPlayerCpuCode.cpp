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
#include "RayParameterMap.hpp"
#include "Primitives.hpp"
#include "VirtualMonitor.h"
#include "Mouse.hpp"
#include "CharacterController.hpp"
#include "Logging.hpp"
#include "ArduinoLED.hpp"
#include "Stopwatch.hpp"

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

int main(void)
{
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = int_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	/* set up arduino for sync indication */
	ArduinoLED led(false);

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");

	bool isSimulation = max_get_constant_uint64t(maxfile,"IS_SIMULATION");

	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Initialise the sample parameter map */

	RayParameterMap rayParameterMap(engine, maxfile);
	if(!isSimulation)
	{
	rayParameterMap.InitialiseMapFromFile(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/rayParameterMap.bin");
	}

	/* Geometry Parameters */

	Primitives primitives(engine, maxfile);
	primitives.SetPrimitives(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/cube_room.csv");
	primitives.InitialisePrimitives();

	/* Initialise environment map */

	EnvironmentMap environmentMap(engine, maxfile);
	environmentMap.bank_address_bits_offset = 25;
	environmentMap.num_banks_used = 1;
	environmentMap.bank_start_num = 0;

	if(!isSimulation)
	{
	printf("Loading maps...");
	environmentMap.SetNumMaps(primitives.maps.size());
	for(uint i = 0; i < primitives.maps.size(); i++)
	{
		environmentMap.LoadMap(primitives.maps[i].filename, primitives.maps[i].index);
	}
	printf("Loaded maps.\n");

	environmentMap.num_banks_used = 4;
	environmentMap.WriteEnvironmentMaps();
	}

	environmentMap.LoadAlphaMap(2, "/home/sfriston/maxworkspace/EnvironmentMapPlayer/maps/alpha.jpg");
	environmentMap.LoadAlphaMap(3, "/home/sfriston/maxworkspace/EnvironmentMapPlayer/maps/alpha.jpg");

	/* ignore memory input on subsequent runs */

	max_set_uint64t(act,"rayParameterMap_toMem_addrGen","io_cmd_force_disabled",1);

	max_ignore_lmem(act,"sampleMap_toDimm1");
	max_ignore_lmem(act,"sampleMap_toDimm2");
	max_ignore_lmem(act,"rayParameterMap_toMem");

	max_ignore_block(act,"sampleDataFanout");

	max_ignore_kernel(act,"rayParameterMap_toMem_addrGen");
	max_ignore_kernel(act,"sampleMapDimm1_toMem_addrGen");
	max_ignore_kernel(act,"sampleMapDimm2_toMem_addrGen");

	/* Last map initialisation */

	environmentMap.WriteAlphaMaps(act);

	/* Rendering parameters */

	max_set_uint64t(act,"RaySampleCommandGeneratorKernel","num_banks_used", rayParameterMap.num_banks_used);
	max_set_uint64t(act,"RaySampleCommandGeneratorKernel","start_bank_num", rayParameterMap.bank_start_num);
	max_set_uint64t(act,"RaySampleCommandGeneratorKernel","sampleParameterMapAddress", rayParameterMap.GetOffsetInBursts());
	max_set_uint64t(act,"RaySampleReaderKernel", "sampleParameterMapAddress", rayParameterMap.GetOffsetInBursts());

	max_set_double(act, "RayCasterKernel", "ipd", 0.6f);

	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize_h", tan(1.15f/1.0f));
	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize_v", tan(1.235f/1.0f));
	max_set_double( act,"RayCasterKernel", "viewplane_viewdistance", 1);
	max_set_double( act,"RayCasterKernel", "viewplane_hres", 960);
	max_set_double( act,"RayCasterKernel", "viewplane_vres", 1080);

	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","num_banks_used", environmentMap.num_banks_used);
	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","start_bank_num", environmentMap.bank_start_num);
	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","bank_address_bits_offset", environmentMap.bank_address_bits_offset);
	max_set_uint64t(act,"MapSampleReaderKernel","backgroundColour", 0x0);

	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","min_mip_level", 3);
	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","max_mip_level", 11);

	/* Video signal parameters */

	max_set_uint64t(act,"MaxVideoSignalKernel","HSyncPolarity",1);
	max_set_uint64t(act,"MaxVideoSignalKernel","VSyncPolarity",1);


	max_reset_engine(engine);


	printf("Running on DFE...\n");

	max_run(engine, act);

	//Get a continuous stream and write to the virtual display when simulating

	VirtualMonitor monitor(maxfile);
	monitor.Connect(engine);

	/* Set up the input devices */

	Mouse mouse(false);
	int inclination = 0;
	int elevation = 0;

	CharacterController characterController(false);	//no parameters opens the default keyboard
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

	bool enablePlayback = false;
	bool startPlayback = false;
	bool enableInteractive = !enablePlayback;

#ifdef USEOCULUS
	if(enablePlayback){
		logger.Load("/home/sfriston/Dropbox/Investigations/Rendering Experiment/Head Tracking Logs/HeadMotionMaster.csv");
	}

	double timeInSeconds = 0;
#endif

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

		switch(keycode)
		{
		case KEY_S:
			startPlayback = true;
			break;
		case KEY_R:
			max_reset_engine(engine);
			break;
		case KEY_Q:
			run = false;
			break;
		}

#ifdef USEOCULUS

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
				lastFiveHundredMsSegment = 0;
				led.Off();

				printf("total time: %f\n", stopwatch.getTimeInSeconds());
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
