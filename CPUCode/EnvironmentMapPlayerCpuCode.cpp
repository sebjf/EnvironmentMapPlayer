#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Camera.hpp"
#include "EnvironmentMap.hpp"
#include "VirtualMonitor.h"
#include "Mouse.hpp"

int main(void)
{

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");

	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Initialise environment map */

	EnvironmentMap environmentMap(engine,maxfile);
	environmentMap.LoadEnvironmentMap("/home/sfriston/maxworkspace/EnvironmentMapPlayer/cube_unwrapped.jpg");

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

	max_set_uint64t(act,"EnvironmentMapPlayerKernel","map_width", 2048);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","map_height", 1536);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","segment_width", 512);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","segment_height", 512);

	/* Video signal parameters */

	max_set_uint64t(act,"MaxVideoSignalKernel","HSyncPolarity",1);
	max_set_uint64t(act,"MaxVideoSignalKernel","VSyncPolarity",1);


	/* Camera parameters */

	//sending these multiple times so there is enough data to do the transfer between the widths inside the dfe
	for(int i = 0; i < 3; i++){
		max_queue_input(act,"camera_u",camera.u.data(),sizeof(float)*4);
		max_queue_input(act,"camera_v",camera.v.data(),sizeof(float)*4);
		max_queue_input(act,"camera_w",camera.w.data(),sizeof(float)*4);
	}


	printf("Running on DFE...\n");

	/* Prepare for output */


	//Get a set amount of data, useful for when using simwatch

/*
	int n_rays = 252*252;
	int outputdatasize_bytes = 8 * n_rays;
	void* outputdata = malloc(outputdatasize_bytes);
	max_queue_output(act, "displayDataOut", outputdata, outputdatasize_bytes);
*/

	max_run(engine, act);


	//Get a continuous stream and write to the display

	VirtualMonitor monitor(256,256);
	monitor.Connect(engine);

	Mouse mouse(false);
	int inclination = 0;;
	int elevation = 0;

	while(1){

		monitor.Refresh(256*256);

		MouseDelta d = mouse.readMouse(false);

		if(d.changed()){

			inclination += -d.y;
			elevation += -d.x;

			camera.set_lookat(inclination,elevation);

			printf("Setting %i %i\n", inclination, elevation);

			max_actions_t* updateact = max_actions_init(maxfile, NULL);

			/* Ignore the other parameters until we build a proper interface */

			max_ignore_lmem(updateact,"environment_map");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","viewplane_hres");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","viewplane_vres");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","viewplane_pixelsize");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","viewplane_viewdistance");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","map_width");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","map_height");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","segment_width");
			max_ignore_scalar(updateact,"EnvironmentMapPlayerKernel","segment_height");
			max_ignore_scalar(updateact,"MaxVideoSignalKernel","HSyncPolarity");
			max_ignore_scalar(updateact,"MaxVideoSignalKernel","VSyncPolarity");

			for(int i = 0; i < 3; i++){
				max_queue_input(updateact,"camera_u",camera.u.data(),sizeof(float)*4);
				max_queue_input(updateact,"camera_v",camera.v.data(),sizeof(float)*4);
				max_queue_input(updateact,"camera_w",camera.w.data(),sizeof(float)*4);
			}

			max_run(engine, updateact);
		}

	}


	max_unload(engine);
	
/*
	for(int i = 0; i < n_rays; ++i)
	{
		printf("d %i: %X\n", i, ((uint32_t*)outputdata)[i]);
	}
*/

	printf("Done.\n");
	return 0;
}
