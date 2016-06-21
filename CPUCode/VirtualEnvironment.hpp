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
#include <vector>

class Map
{
public:
	string  filename;
	int 	index;
};

class VirtualRoom
{
public:
	std::vector<Primitives::Primitive> primitives;
	std::vector<Map> maps;
	std::vector<Map> alphaMaps;

	string root;

	void Load(string csvFilename)
	{
		csvFilename = root + csvFilename;
		Load((const char*)csvFilename.c_str());
	}

private:
	void Load(const char* csvFilename)
	{
		CsvParser* parser = CsvParser_new(csvFilename,",",0);

		/* number of planes */
		CsvRow* row = CsvParser_getRow(parser);
		const char** fields = CsvParser_getFields(row);
		int numPrimitives = atoi(fields[0]);
		CsvParser_destroy_row(row);

		for(int i = 0; i < numPrimitives; i++)
		{
			row = CsvParser_getRow(parser);
			fields = CsvParser_getFields(row);

			if(CsvParser_getNumFields(row) < 14)
			{
				throw "Malformed Room Definition";
			}

			Primitives::Primitive p;

			p.index = atoi(fields[0]);
			p.center.x = atof(fields[2]);
			p.center.y = atof(fields[3]);
			p.center.z = atof(fields[4]);

			p.normal.x = atof(fields[5]);
			p.normal.y = atof(fields[6]);
			p.normal.z = atof(fields[7]);

			p.up.x = atof(fields[8]);
			p.up.y = atof(fields[9]);
			p.up.z = atof(fields[10]);

			p.width = atof(fields[11]);
			p.height = atof(fields[12]);

			p.mapIndex = atoi(fields[13]);

			primitives.push_back(p);

			CsvParser_destroy_row(row);
		}

		/* number of texture maps */
		row = CsvParser_getRow(parser);
		fields = CsvParser_getFields(row);
		int numTextureMaps = atoi(fields[0]);
		CsvParser_destroy_row(row);

		for(int i = 0; i < numTextureMaps; i++)
		{
			row = CsvParser_getRow(parser);
			fields = CsvParser_getFields(row);

			Map m;
			m.filename = root + string(fields[0]);
			m.index = i;

			maps.push_back(m);

			CsvParser_destroy_row(row);
		}

		/* number of alpha maps */
		row = CsvParser_getRow(parser);
		fields = CsvParser_getFields(row);
		int numTAlphaMaps = atoi(fields[0]);
		CsvParser_destroy_row(row);

		for(int i = 0; i < numTAlphaMaps; i++)
		{
			row = CsvParser_getRow(parser);
			fields = CsvParser_getFields(row);

			Map m;
			m.filename = root + string(fields[0]);
			m.index = i;

			alphaMaps.push_back(m);

			CsvParser_destroy_row(row);
		}

		CsvParser_destroy(parser);
	}
};

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

	void Initialise(VirtualRoom* room)
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
		primitives->SetPrimitives(room->primitives);
		primitives->connect();


		/* Initialise Maps */

		EnvironmentMap environmentMap(engine, maxfile);
		environmentMap.bank_address_bits_offset = 25;
		environmentMap.num_banks_used = 1;
		environmentMap.bank_start_num = 0;

		if(!isSimulation)
		{
		printf("Loading maps...");
		environmentMap.SetNumMaps(room->maps.size());
		for(uint i = 0; i < room->maps.size(); i++)
		{
			environmentMap.LoadMap(room->maps[i].filename, room->maps[i].index);
		}
		printf("Loaded maps.\n");

		environmentMap.num_banks_used = 8;
		environmentMap.WriteEnvironmentMaps();
		}

		for(unsigned int i = 0; i < room->alphaMaps.size(); i++){
			environmentMap.LoadAlphaMap(room->alphaMaps[i].index, room->alphaMaps[i].filename);
		}

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

		primitives->InitialisePrimitives();
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
