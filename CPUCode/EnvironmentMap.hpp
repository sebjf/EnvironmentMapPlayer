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

public:
	EnvironmentMap(max_engine_t* engine, max_file_t* maxfile)
	{
		this->m_engine = engine;
		this->m_maxfile = maxfile;
	}

	void LoadEnvironmentMap(string filename)
	{
		SDL_Surface* img = IMG_Load(filename.c_str());
		SDL_Surface* surface = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_BlitSurface(img,NULL,surface,NULL);

		int texture_size_bytes = surface->w * surface->h * 4;

/*
		int i = 0;
		for(int x = 0; x < surface->w; x++)
		{
			for(int y = 0; y < surface->h; y++){
				((uint32_t*)surface->pixels)[i] = i;
				i++;
			}
		}
*/

		max_actions_t* memact = max_actions_init(m_maxfile, "memoryInitialisation");
		max_set_param_uint64t(memact, "size", texture_size_bytes);
		max_queue_input(memact,"environment_map_in",surface->pixels,texture_size_bytes);

		max_run(m_engine, memact);
	}

};

#endif /* ENVIRONMENTMAP_HPP_ */
