/*
 * Primitives.hpp
 *
 *  Created on: 9 May 2016
 *      Author: sfriston
 */

#ifndef PRIMITIVES_HPP_
#define PRIMITIVES_HPP_

#include "Types.hpp"
#include "LowLatencyStream.hpp"
#include <vector>
#include <memory.h>
#include "CSVParser/csvparser.h"

#define PRIMITIVES_COUNT 8

class Primitives
{
public:

	struct Primitive
	{
		int  index;
		vector3 center;
		vector3 normal;
		vector3 up; //tangent to left of plane
		float width;
		float height;
		int	  mapIndex;
	};

	struct __attribute__((__packed__)) LoadedPrimitive
	{
		float	center[3];
		float	u[3]; //right
		float 	v[3]; //up
		float	index;
		float	map;
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

	vector<float> offset;

	planeParamsUpdate_t* m_primitiveParameters;
	int m_primitiveParametersSize;

	struct __attribute__((__packed__)) coefficients_t
	{
		float	padding[3];
		float 	coefficient;
	};

	coefficients_t coefficients;

	LowLatencyStream<planeParamsUpdate_t>* primitivesSettingsStream;
	LowLatencyStream<coefficients_t>* coefficientsStream;

public:
	Primitives(max_engine_t* engine, max_file_t* maxfile)
	{
		m_engine = engine;
		m_maxfile = maxfile;

		offset.assign(3,0);

		m_primitiveParametersSize = sizeof(struct planeParamsUpdate_t) * PRIMITIVES_COUNT;
		m_primitiveParameters = (planeParamsUpdate_t*)malloc(m_primitiveParametersSize);

		for(int i = 0; i < PRIMITIVES_COUNT; i++)
		{
			planeParamsUpdate_t* p = &(m_primitiveParameters[i]);
			p->index = i;
			p->enable = false;
		}

		primitivesSettingsStream = new LowLatencyStream<planeParamsUpdate_t>("primitivesStream", maxfile);
		coefficientsStream = new LowLatencyStream<coefficients_t>("coefficients", maxfile);
	}

	void SetPrimitive(int id, struct Primitive primitive)
	{
		planeParamsUpdate_t* p = &(m_primitiveParameters[id]);

		p->index = id;
		p->enable = true;
		p->center = vector3::add(primitive.center, offset);
		p->normal = primitive.normal;
		p->face = primitive.mapIndex;

		vector3 tangent = vector3::cross(primitive.normal, primitive.up);
		p->u_basis = vector3::multiply(tangent.normalised(), primitive.width);

		p->v_basis = vector3::multiply(primitive.up.normalised(), primitive.height);
	}

	void SetPrimitives(std::vector<Primitive> primitives)
	{
		for(unsigned int i = 0; i < primitives.size(); i++)
		{
			SetPrimitive(primitives[i].index, primitives[i]);
		}
	}

	//for before the design begins to run
	void InitialisePrimitives()
	{
		for(int i = 0; i < PRIMITIVES_COUNT; i++)
		{
			primitivesSettingsStream->Send(m_primitiveParameters[i]);
		}
		SetShade(1.0f);
	}

	//for after the design begins to run
	void connect()
	{
		printf("Preparing low latency stream for primitive updates...\n");
		primitivesSettingsStream->Connect(m_engine);
		coefficientsStream->Connect(m_engine);
	}

	void SetPrimitiveCenter(int id, vector<float> position)
	{
		planeParamsUpdate_t* p = &(m_primitiveParameters[id]);
		p->center.x = position.at(0);
		p->center.y = position.at(1);
		p->center.z = position.at(2);
		primitivesSettingsStream->Send(*p);
	}

	void SetOffset(vector<float> o)
	{
		offset = o;
	}

	void SetShade(float v)
	{
		coefficients.coefficient = v;
		coefficientsStream->Send(coefficients);
	}

	float GetShade()
	{
		return coefficients.coefficient;
	}

	vector<LoadedPrimitive> GetGeometry()
	{
		vector<LoadedPrimitive> primitives;
		for(unsigned int i = 0; i < PRIMITIVES_COUNT; i++)
		{
			planeParamsUpdate_t plane = m_primitiveParameters[i];
			if(plane.enable)
			{
				LoadedPrimitive p;
				memcpy(p.center, &plane.center, sizeof(struct vector3));
				memcpy(p.u, &plane.u_basis, sizeof(struct vector3));
				memcpy(p.v, &plane.v_basis, sizeof(struct vector3));
				p.map = plane.face;

				primitives.push_back(p);
			}
		}
		return primitives;
	}

};

#endif /* PRIMITIVES_HPP_ */
