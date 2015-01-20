#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/numeric/ublas/vector.hpp>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

#include "Camera.hpp"

using namespace boost::numeric::ublas;

int main(void)
{

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");
	max_actions_t* act = max_actions_init(maxfile, NULL);

	/* Specify camera properties */

	Camera camera;
	camera.set_lookat(0,0);

	/* Rendering parameters */

	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_hres", 100);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_vres", 100);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_pixelsize", 0.02);
	max_set_double(act,"EnvironmentMapPlayerKernel","viewplane_viewdistance", 1);

	/* Camera parameters */

	max_queue_input(act,"camera_u",camera.u.data(),sizeof(float)*4);
	max_queue_input(act,"camera_v",camera.v.data(),sizeof(float)*4);
	max_queue_input(act,"camera_w",camera.w.data(),sizeof(float)*4);

	/* Viewport parameters */

	const int width = 16;
	const int height = 16;

	const int n_rays = width * height;

	int inputsize_bytes = n_rays * sizeof(int32_t);

	int32_t* x = (int32_t*)malloc(inputsize_bytes);
	int32_t* y = (int32_t*)malloc(inputsize_bytes);
	for(int i = 0; i < width; i++){
		x[i] = i;
	}
	for(int i = 0; i < width; i++){
		y[i] = i;
	}


	max_set_ticks(act,"EnvironmentMapPlayerKernel", n_rays);

	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);

	/* Prepare for output */

	float* d = new float[n_rays * 3];
	max_queue_output(act, "d", d, sizeof(float) * n_rays * 3);


	printf("Running on DFE...\n");

	max_run(engine, act);
	max_unload(engine);
	

	for(int i = 0; i < n_rays; ++i)
	{
		printf("d %i: %f %f %f\n", i, d[(3*i) + 0],d[(3*i) + 1],d[(3*i) + 2]);
	}

	printf("Done.\n");
	return 0;
}
