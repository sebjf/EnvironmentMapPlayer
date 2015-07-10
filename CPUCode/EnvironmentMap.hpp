/*
 * EnvironmentMap.hpp
 *
 *  Created on: Jan 20, 2015
 *      Author: demo
 */

#ifndef ENVIRONMENTMAP_HPP_
#define ENVIRONMENTMAP_HPP_

#include <string>
#include "SDL/SDL.h"
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

/*https://www.libsdl.org/projects/SDL_image/release-1.2.html*/

class EnvironmentMap
{
private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

	int m_map_size_in_bytes;

public:
	EnvironmentMap(max_engine_t* engine, max_file_t* maxfile)
	{
		this->m_engine = engine;
		this->m_maxfile = maxfile;
		m_map_size_in_bytes = 0;
	}

	void LoadEnvironmentMap(string filename)
	{
		SDL_Surface* img = IMG_Load(filename.c_str());

		if(!img){
			printf("IMG_Load: %s\n", IMG_GetError());
		}

		SDL_Surface* surface = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_BlitSurface(img,NULL,surface,NULL);

		printf("writing environment map...\n");

		int texture_size_bytes = surface->w * surface->h * 4;

		m_map_size_in_bytes = texture_size_bytes;

		max_actions_t* memact = max_actions_init(m_maxfile, "memoryInitialisation");
		max_set_param_uint64t(memact, "size", texture_size_bytes);
		max_set_param_uint64t(memact, "address", 0);
		max_queue_input(memact,"environment_map_in",surface->pixels,texture_size_bytes);

		max_run(m_engine, memact);
	}

	int GetMapSizeInBursts()
	{
		return ceil((float)m_map_size_in_bytes / 384.0f);
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

		max_actions_t* memact = max_actions_init(m_maxfile, "memoryInitialisation");
		max_set_param_uint64t(memact, "size", texture_size_bytes);
		max_queue_input(memact,"environment_map_in",pixels,texture_size_bytes);

		max_run(m_engine, memact);

		free(pixels);
	}

};

#endif /* ENVIRONMENTMAP_HPP_ */
