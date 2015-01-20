/*
 * EnvironmentMap.hpp
 *
 *  Created on: Jan 20, 2015
 *      Author: demo
 */

#ifndef ENVIRONMENTMAP_HPP_
#define ENVIRONMENTMAP_HPP_

#include <string>
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

class EnvironmentMap
{
private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

public:
	EnvironmentMap(max_engine_t* engine, max_file_t* maxfile)
	{
		this->m_engine = engine;
		this->m_maxfile = maxfile;
	}

	void LoadEnvironmentMap(string filename)
	{
		max_actions_t* memact = max_actions_init(m_maxfile, "memoryInitialisation");

		int texturesize_bursts = (ceil((200 * 250 * 4)/384)*384);
		void* texturedata = malloc(texturesize_bursts);

		for(int i = 0; i < texturesize_bursts; i++)
		{
			char* texturecontent = (char*)texturedata;
			texturecontent[i] = 0xf0;
		}

		max_set_param_uint64t(memact, "size", texturesize_bursts);
		max_queue_input(memact,"environment_map_in",texturedata,texturesize_bursts);

		max_run(m_engine, memact);
	}

};

#endif /* ENVIRONMENTMAP_HPP_ */
