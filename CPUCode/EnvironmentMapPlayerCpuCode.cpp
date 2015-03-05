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
#include "VirtualMonitor.h"
#include "Mouse.hpp"

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

	EnvironmentMap environmentMap(engine,maxfile);
	environmentMap.LoadEnvironmentMap(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/museum.bmp");

	/* ignore memory input on subsequent runs */

	max_ignore_lmem(act,"environment_map");

	/* Specify camera properties */

	Camera camera;
	camera.set_lookat(0,0);

	/* Rendering parameters */

	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_hres", 252);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_vres", 252);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_pixelsize", 0.02);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_viewdistance", 1);

	/* Video signal parameters */

	max_set_uint64t(act,"MaxVideoSignalKernel","HSyncPolarity",1);
	max_set_uint64t(act,"MaxVideoSignalKernel","VSyncPolarity",1);


	printf("Running on DFE...\n");

	max_run(engine, act);


	//Get a continuous stream and write to the virtual display when simulating

	VirtualMonitor monitor(maxfile);
	monitor.Connect(engine);

	Mouse mouse(false);
	int inclination = 0;;
	int elevation = 0;

	camera.connect(engine);

	printf("Press CTRL+C key to exit.\n");

	int counter = 0;

	while(run){

		monitor.Refresh();

		MouseDelta d = mouse.readMouse(false);

		if(d.changed()){
			inclination += -d.y;
			elevation += -d.x;
			camera.set_lookat(inclination,elevation);
		}

		counter++;

		if(counter > 256){
		//	run = false;
		}
	}


	max_unload(engine);
	
	printf("Done.\n");
	return 0;
}
