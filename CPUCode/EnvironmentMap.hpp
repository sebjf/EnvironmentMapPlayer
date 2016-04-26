/*
 * EnvironmentMap.hpp
 *
 *  Created on: Jan 20, 2015
 *      Author: demo
 */

#ifndef ENVIRONMENTMAP_HPP_
#define ENVIRONMENTMAP_HPP_

#include <string>
#include <vector>
#include "SDL/SDL.h"
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

/*https://www.libsdl.org/projects/SDL_image/release-1.2.html*/

#define BURSTSIZE_BYTES 64

class EnvironmentMap
{
private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

	int m_map_size_in_bytes;

public:
	int num_banks_used;
	int bank_start_num;
    int bank_address_bits_offset;

public:
	EnvironmentMap(max_engine_t* engine, max_file_t* maxfile)
	{
		this->m_engine = engine;
		this->m_maxfile = maxfile;
		m_map_size_in_bytes = 0;

		bank_start_num = 0;
		num_banks_used = 1;
		bank_address_bits_offset = 25;
	}

	void LoadEnvironmentMap(string filename)
	{
		SDL_Surface* img = IMG_Load(filename.c_str());

		if(!img){
			printf("IMG_Load: %s\n", IMG_GetError());
		}

		SDL_Surface* surface = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_BlitSurface(img,NULL,surface,NULL);

		int texture_size_bytes = surface->w * surface->h * 4;

		m_map_size_in_bytes = texture_size_bytes;

		printf("Texture Size: %i bytes\n", m_map_size_in_bytes);

		unsigned long bank_size = (pow(2, bank_address_bits_offset)*BURSTSIZE_BYTES);
		if(bank_size <= (unsigned long)m_map_size_in_bytes)
		{
			printf("Warning: texture too large to fit in a single bank. (%lu bytes)\n", bank_size);
		}

		max_actions_t* actions[num_banks_used];

		for(int i = 0; i < num_banks_used; i++){

			int64_t bank_offset_in_bursts = (((int64_t)bank_start_num + (int64_t)i) << bank_address_bits_offset);
			int64_t bank_offset_in_bytes = bank_offset_in_bursts * BURSTSIZE_BYTES;
			int64_t address = bank_offset_in_bytes;

			max_actions_t* memact = max_actions_init(m_maxfile, "sampleMap_initialisation");

			max_set_param_uint64t(memact, "size", texture_size_bytes);
			max_set_param_uint64t(memact, "address", address);

			max_queue_input(memact,"sampleMap_fromCPU",surface->pixels,texture_size_bytes);
			actions[i] = memact;

			printf("writing environment map...(%i times)(address: %i)\n", num_banks_used, address);

			max_run(m_engine, memact);
		}
	}

	void WriteDebugValues(string filename)
	{
		SDL_Surface* img = IMG_Load(filename.c_str());
		int texture_size_bytes = img->w * img->h * 4;
		void* pixels = malloc(texture_size_bytes);

		int i = 0;
		for(int x = 0; x < img->w; x++)
		{
			for(int y = 0; y < img->h; y++){
				((uint32_t*)pixels)[i] = i;
				i++;
			}
		}

		max_actions_t* memact = max_actions_init(m_maxfile, "sampleMap_initialisation");
		max_set_param_uint64t(memact, "size", texture_size_bytes);
		max_queue_input(memact,"sampleMap_fromCPU",pixels,texture_size_bytes);

		max_run(m_engine, memact);

		free(pixels);
	}

};

#endif /* ENVIRONMENTMAP_HPP_ */
