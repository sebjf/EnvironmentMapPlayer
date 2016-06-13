/*
 * VirtualEnvironment.hpp
 *
 *  Created on: Jun 9, 2016
 *      Author: demo
 */

#ifndef VIRTUALENVIRONMENT_HPP_
#define VIRTUALENVIRONMENT_HPP_

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

class VirtualEnvironment
{
private:

	max_file_t *maxfile;
	max_engine_t *engine;
	max_actions_t* act;
	Primitives* primitives;
	Camera* camera;

public:
	VirtualEnvironment()
	{
		maxfile = EnvironmentMapPlayer_init();
	}
	~VirtualEnvironment()
	{

	}

	max_file_t* getMaxfile()
	{
		return maxfile;
	}

	max_engine_t* getEngine()
	{
		return engine;
	}

	void Initialise()
	{
		/* Initialize the maxfile to get an actions with which to configure the renderer */

		engine = max_load(maxfile, "*");

		bool isSimulation = max_get_constant_uint64t(maxfile,"IS_SIMULATION");

		/* Initialise main run actions */

		act = max_actions_init(maxfile, NULL);

		/* Initialise the sample parameter map */

		RayParameterMap rayParameterMap(engine, maxfile);
		if(!isSimulation)
		{
		rayParameterMap.InitialiseMapFromFile(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/rayParameterMap.bin");
		}

		/* Geometry Parameters */

		primitives = new Primitives(engine, maxfile);
		primitives->SetPrimitives(string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/cube_room.csv");
		primitives->InitialisePrimitives();
		primitives->connect();

		/* Initialise Maps */

		EnvironmentMap environmentMap(engine, maxfile);
		environmentMap.bank_address_bits_offset = 25;
		environmentMap.num_banks_used = 1;
		environmentMap.bank_start_num = 0;

		if(!isSimulation)
		{
		printf("Loading maps...");
		environmentMap.SetNumMaps(primitives->maps.size());
		for(uint i = 0; i < primitives->maps.size(); i++)
		{
			environmentMap.LoadMap(primitives->maps[i].filename, primitives->maps[i].index);
		}
		printf("Loaded maps.\n");

		environmentMap.num_banks_used = 8;
		environmentMap.WriteEnvironmentMaps();
		}

		environmentMap.LoadAlphaMap(0, string(getenv("HOME")) + "/maxworkspace/EnvironmentMapPlayer/maps/alpha.jpg");

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

		max_set_double( act,"RayCasterKernel", "ipd", 0.6f);

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

		camera = new Camera();
		camera->connect(getEngine());
	}

	void Start()
	{
		max_reset_engine(engine);

		printf("Running on DFE...\n");

		max_run(engine, act);

		primitives->SetShade(1.0f);
	}

	void Reset()
	{
		Start();
	}

	void Stop()
	{
		max_unload(engine);
	}

	void SetShade(float f)
	{
		primitives->SetShade(f);
	}

	float GetShade()
	{
		return primitives->GetShade();
	}

	Camera* getCamera()
	{
		return camera;
	}

	vector<Primitives::LoadedPrimitive> GetPrimitives()
	{
		return primitives->GetGeometry();
	}

	void SetPrimitiveCenter(int id, vector<float> c)
	{
		primitives->SetPrimitiveCenter(id, c);
	}

};



#endif /* VIRTUALENVIRONMENT_HPP_ */
