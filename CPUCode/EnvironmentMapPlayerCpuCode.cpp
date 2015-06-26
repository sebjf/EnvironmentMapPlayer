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
	max_engine_t *engine = max_load(maxfile, "*");

	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Initialise environment map */

	EnvironmentMap environmentMap(engine, maxfile);
	environmentMap.LoadEnvironmentMap(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/map.bmp");

	/* Initialise the sample parameter map */

	SampleParameterMap sampleParameterMap(engine, maxfile);
	sampleParameterMap.m_offset_in_bursts = environmentMap.GetMapSizeInBursts();
	sampleParameterMap.InitialiseMapFromFile(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/rayParameterMap.bin");

	/* ignore memory input on subsequent runs */

	max_ignore_lmem(act,"environment_map");

	/* Specify camera properties */

	Camera camera;
	camera.set_eye(0, 100, -50);
	camera.set_lookat(90,90);


	/* Rendering parameters */

	max_set_uint64t(act, "RaySampleReaderKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());
	max_set_uint64t(act, "RaySampleCommandGeneratorKernel", "sampleParameterMapAddress", sampleParameterMap.GetOffsetInBursts());

	max_set_double(act, "RayCasterKernel", "ipd", 6.5f);

	//max_set_uint64t(act,"RayCasterKernel", "viewplane_hres", max_get_constant_uint64t(maxfile,"DisplayActiveWidth"));
	//max_set_uint64t(act,"RayCasterKernel", "viewplane_vres", max_get_constant_uint64t(maxfile,"DisplayActiveHeight"));
	max_set_double( act,"RayCasterKernel", "viewplane_pixelsize", 0.01);
	max_set_double( act,"RayCasterKernel", "viewplane_viewdistance", 1);

	max_set_uint64t(act,"MapSampleReaderKernel","backgroundColour", 0x101010);

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
	int inclination = 60;
	int elevation = 90;

	CharacterController characterController("/dev/input/by-id/usb-DELL_Dell_USB_Entry_Keyboard-event-kbd");
	characterController.set_position(0, 100, -50);

	camera.connect(engine);

	printf("Press CTRL+C key to exit.\n");

	while(run){

		monitor.Refresh();

		MouseDelta d = mouse.readMouse(false);
		//characterController.update();

		camera.set_eye(characterController.position[0],characterController.position[1],characterController.position[3]);

		if(d.changed()){
			inclination += -d.y;
			elevation += -d.x;
		}

		camera.set_lookat(inclination,elevation);

	}


	max_unload(engine);
	
	printf("Done.\n");
	return 0;
}
