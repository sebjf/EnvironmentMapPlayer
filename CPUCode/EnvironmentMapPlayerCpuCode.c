#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

int main(void)
{
	/* Prepare test variables */
	int n_rays = 16;
	int inputsize_bytes = n_rays * sizeof(int32_t);
	int32_t* x = malloc(inputsize_bytes);
	int32_t* y = malloc(inputsize_bytes);

	x[0] = 1;
	y[0] = 1;

	/* Initialize the maxfile to get an actions with which to configure the renderer */

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");
	max_actions_t* act = max_actions_init(maxfile, NULL);


	/* Rendering parameters */

	max_set_ticks(act,"EnvironmentMapPlayerKernel", n_rays);

	//	max_set_uint64t(actions, "EnvironmentMapPlayerKernel", "viewplane_hres", inscalar_EnvironmentMapPlayerKernel_viewplane_hres);
	max_set_uint64t(act,"EnvironmentMapPlayerKernel","viewplane_hres",100);

//...


	max_queue_input(act, "col", x, inputsize_bytes);
	max_queue_input(act, "row", y, inputsize_bytes);


	/* Prepare for output */

	int outputsize_bytes = sizeof(float) * 3 * n_rays;
	float* d = malloc(outputsize_bytes);

	max_queue_output(act, "d", d, outputsize_bytes);


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
