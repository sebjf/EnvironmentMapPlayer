#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

int main(void)
{
	const int size = 384;
	int sizeBytes = size * sizeof(int32_t);
	int32_t *x = malloc(sizeBytes);
	int32_t *y = malloc(sizeBytes);
	int32_t *s = malloc(sizeBytes);
	int scalar = 3;

	// TODO Generate input data
	for(int i = 0; i < size; ++i) {
		x[i] = random() % 100;
		y[i] = random() % 100;
	}

	max_file_t *maxfile = EnvironmentMapPlayer_init();
	max_engine_t *engine = max_load(maxfile, "*");

	max_actions_t* act = max_actions_init(maxfile, NULL);
	
	max_set_param_uint64t(act, "N", size);
	max_set_param_uint64t(act, "EnvironmentMapPlayerKernel_a", scalar);
	max_queue_input(act, "x", x, sizeBytes);
	max_queue_input(act, "y", y, sizeBytes);
	max_queue_output(act, "s", s, sizeBytes);

	printf("Running on DFE (mode: ComputeWithScalar)...\n");
	max_run(engine, act);
	max_unload(engine);
	
	// TODO Use result data
	for(int i = 0; i < size; ++i)
		if ( s[i] != x[i] + y[i] + scalar)
			return 1;

	printf("Done.\n");
	return 0;
}
