#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

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

	max_set_uint64t(act, "RaySampleReaderKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());
	max_set_uint64t(act, "RaySampleCommandGeneratorKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());

	max_set_double(act, "RayCasterKernel", "ipd", 3.5f);

	//max_set_uint64t(act,"RayCasterKernel", "viewplane_hres", max_get_constant_uint64t(maxfile,"DisplayActiveWidth"));
	//max_set_uint64t(act,"RayCasterKernel", "viewplane_vres", max_get_constant_uint64t(maxfile,"DisplayActiveHeight"));
	float fov = 90.0f;
	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize", tan(DEG2RAD*(fov/2)));
	max_set_double( act,"RayCasterKernel", "viewplane_viewdistance", 1);
	max_set_double( act,"RayCasterKernel", "viewplane_hres", 960);
	max_set_double( act,"RayCasterKernel", "viewplane_vres", 1080);

	max_set_uint64t(act,"MapSampleCommandGeneratorKernel","num_banks_used", environmentMap.num_banks_used);
	max_set_uint64t(act,"MapSampleReaderKernel","backgroundColour", 0xF0F0F0);

	/* Video signal parameters */

	max_set_uint64t(act,"MaxVideoSignalKernel","HSyncPolarity",1);
	max_set_uint64t(act,"MaxVideoSignalKernel","VSyncPolarity",1);

	//disable burst input for debugging

//	max_set_uint64t(act,"MapSampleReaderKernel","io_burst_input_force_disabled",1);
//	max_set_uint64t(act,"MapSampleReaderKernel","io_cache_valid_force_disabled",1);
//	max_set_uint64t(act,"MapSampleReaderKernel","io_sample_offset_in_pixels_force_disabled",1);

	printf("Running on DFE...\n");

	max_run(engine, act);


	//Get a continuous stream and write to the virtual display when simulating

	VirtualMonitor monitor(maxfile);
	monitor.Connect(engine);

	/* Set up the input devices */

	Mouse mouse(false);
	int inclination = 0;
	int elevation = 0;

	CharacterController characterController("/dev/input/by-id/usb-Dell_Dell_USB_Keyboard-event-kbd");
	characterController.set_position(0, 0, 0);

	/* Specify camera properties */

	Camera camera;
	camera.connect(engine);

	/* Get the Oculus */
#ifdef USEOCULUS
	Oculus oculus;
#endif

	printf("Press CTRL+C key to exit.\n");

	while(run){

		monitor.Refresh();

		MouseDelta d = mouse.readMouse(false);
		characterController.update();

		camera.set_eye(characterController.position[0],characterController.position[1],characterController.position[2]);

		if(d.changed()){
			inclination += -d.y;
			elevation += -d.x;
		}

#ifdef USEOCULUS
		oculus.Update();
		camera.set_ovr(oculus.GetCameraForward(), oculus.GetCameraUp());
#else
		camera.set_lookat(inclination, elevation);
#endif

	}


	max_unload(engine);
	
	printf("Done.\n");
	return 0;
}
