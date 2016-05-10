/*
 * Primitives.hpp
 *
 *  Created on: 9 May 2016
 *      Author: sfriston
 */

#ifndef PRIMITIVES_HPP_
#define PRIMITIVES_HPP_

#include "Types.hpp"
#include <vector>
#include <memory.h>
#include "CSVParser/csvparser.h"

#define PRIMITIVES_COUNT 6

class Primitives
{
public:
	struct Primitive
	{
		vector3 center;
		vector3 normal;
		vector3 up; //tangent to left of plane
		float width;
		float height;
		int	  mapIndex;
	};

private:
	struct __attribute__((__packed__)) planeParamsUpdate_t
	{
		char	index;
		char	enable;
		vector3 center;
		vector3 normal;
		uint32_t face;
		vector3 u_basis;
		vector3 v_basis;
		char 	padding[10];
	};

private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

	planeParamsUpdate_t* m_primitiveParameters;
	int m_primitiveParametersSize;


public:
	Primitives(max_engine_t* engine, max_file_t* maxfile)
	{
		m_engine = engine;
		m_maxfile = maxfile;

		m_primitiveParametersSize = sizeof(struct planeParamsUpdate_t) * PRIMITIVES_COUNT;
		m_primitiveParameters = (planeParamsUpdate_t*)malloc(m_primitiveParametersSize);
	}

	void SetPrimitive(int id, struct Primitive primitive)
	{
		planeParamsUpdate_t* p = &(m_primitiveParameters[id]);

		p->index = id;
		p->enable = true;
		p->center = primitive.center;
		p->normal = primitive.normal;
		p->face = primitive.mapIndex;

		vector3 tangent = vector3::cross(primitive.normal, primitive.up);
		p->u_basis = vector3::multiply(tangent.normalised(), primitive.width);

		p->v_basis = vector3::multiply(primitive.up.normalised(), primitive.height);
	}

	void SetPrimitives(string csvFilename)
	{
		SetPrimitives((const char*)csvFilename.c_str());
	}

	void SetPrimitives(const char* csvFilename)
	{
		CsvParser* parser = CsvParser_new(csvFilename,",",0);
		CsvRow* row = CsvParser_getRow(parser);

		int i = 0;

		while(row != NULL && row->numOfFields_ >= 13)
		{
			const char** fields = CsvParser_getFields(row);

			Primitive p;
			p.center.x = atof(fields[1]);
			p.center.y = atof(fields[2]);
			p.center.z = atof(fields[3]);

			p.normal.x = atof(fields[4]);
			p.normal.y = atof(fields[5]);
			p.normal.z = atof(fields[6]);

			p.up.x = atof(fields[7]);
			p.up.y = atof(fields[8]);
			p.up.z = atof(fields[9]);

			p.width = atof(fields[10]);
			p.height = atof(fields[11]);

			p.mapIndex = atoi(fields[12]);

			SetPrimitive(i++, p);

			CsvParser_destroy_row(row);
			row = CsvParser_getRow(parser);
		}

		CsvParser_destroy(parser);
	}

	//for before the design begins to run
	void InitialisePrimitives()
	{
		printf("Initialising primitives...");

		max_actions_t* act = max_actions_init(m_maxfile, "primitives_initialisation");
		max_queue_input(act, "primitivesStream", m_primitiveParameters,m_primitiveParametersSize);
		max_run(m_engine, act);

		//wait for a moment, because after we return the cpu program will reset the dfe, dropping the initialisation commands we just issued if they have not yet been executed
		sleep(1);
	}

	void connect()
	{
//		printf("Preparing low latency stream for primitive updates...\n");
	}
};

#endif /* PRIMITIVES_HPP_ */