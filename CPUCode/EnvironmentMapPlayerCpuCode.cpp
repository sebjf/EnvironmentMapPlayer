#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Camera.hpp"
#include "EnvironmentMap.hpp"

int main(void)
{

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");

	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Initialise environment map */

	EnvironmentMap environmentMap(engine,maxfile);
	environmentMap.LoadEnvironmentMap("myimage");

	/* ignore memory input on subsequent runs */

	max_ignore_lmem(act,"environment_map");

	/* Specify camera properties */

	Camera camera;
	camera.set_lookat(95,29);

	/* Rendering parameters */

	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_hres", 100);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_vres", 100);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_pixelsize", 0.02);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_viewdistance", 1);

	max_set_uint64t(act,"EnvironmentMapPlayerKernel","map_width", 200);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","map_height", 150);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","segment_width", 50);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","segment_height", 50);

	/* Camera parameters */

	//sending these multiple times so there is enough data to do the transfer between the widths inside the dfe
	for(int i = 0; i < 3; i++){
		max_queue_input(act,"camera_u",camera.u.data(),sizeof(float)*4);
		max_queue_input(act,"camera_v",camera.v.data(),sizeof(float)*4);
		max_queue_input(act,"camera_w",camera.w.data(),sizeof(float)*4);
	}

	/* Viewport parameters */

	const int width = 16;
	const int height = 16;
	const int n_rays = width * height;
	int inputsize_bytes = n_rays * sizeof(int32_t);

	int32_t* x = (int32_t*)malloc(inputsize_bytes);
	int32_t* y = (int32_t*)malloc(inputsize_bytes);
	for(int xi = 0; xi < width; xi++){
		for(int yi = 0; yi < height; yi++){
			int i = (width * yi) + xi;
			x[i] = xi;
			y[i] = yi;
		}
	}

	//max_set_ticks(act,"EnvironmentMapPlayerKernel", n_rays);
	//max_set_ticks(act,"EnvironmentMapSampleCommandGeneratorKernel", n_rays);
	//max_set_ticks(act,"EnvironmentMapSampleReaderKernel", n_rays);

	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);
	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);
	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);
	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);

	/* Prepare for output */

	int outputdatasize_bytes = 4 * n_rays;
	void* outputdata = malloc(outputdatasize_bytes);

	max_queue_output(act, "sample", outputdata, outputdatasize_bytes);

	printf("Running on DFE...\n");

	//max_disable_validation(act);

	max_run(engine, act);



	max_unload(engine);
	

	for(int i = 0; i < n_rays; ++i)
	{
		printf("d %i: %X\n", i, ((uint32_t*)outputdata)[i]);
	}

	printf("Done.\n");
	return 0;
}
