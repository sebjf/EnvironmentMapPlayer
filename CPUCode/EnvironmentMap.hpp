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
#include "SDL/SDL_rotozoom.h"
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"
#include <string.h>

/*https://www.libsdl.org/projects/SDL_image/release-1.2.html*/

#define BURSTSIZE_BYTES 64
#define TILE_STRIDE_BYTES 384

#define MIP_MIN	3
#define MIP_MAX 11
#define TILE_SIZE 8

#define ALPHA_MAP_DIMENSION 1024
#define PIXELS_PER_WORD		64


class EnvironmentMap
{
private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

	int m_map_size_in_tiles;
	int m_textures_data_size;
	char* m_textures_data;

public:
	int num_banks_used;
	int bank_start_num;
    int bank_address_bits_offset;

private:

    int mipMapWidth[12];
    int mipMapSizeInPixels[12];
    int mipMapWidthInTiles[12];
    int mipMapSizeInTiles[12];

    struct AlphaMapParams
    {
    	uint64_t*	m_data;
    	int  		m_index;
    	int			m_numWords;

    	AlphaMapParams(int index)
    	{
    		int dataLengthInBytes = ((ALPHA_MAP_DIMENSION * ALPHA_MAP_DIMENSION) / PIXELS_PER_WORD) * (PIXELS_PER_WORD / 8);
    		m_data = (uint64_t*)malloc(dataLengthInBytes);
    		memset(m_data, 0x0, dataLengthInBytes);
    		m_index = index;
    		m_numWords = (ALPHA_MAP_DIMENSION * ALPHA_MAP_DIMENSION) / PIXELS_PER_WORD;
    	}
    };

    std::vector<AlphaMapParams*> m_alphaMaps;

public:
	EnvironmentMap(max_engine_t* engine, max_file_t* maxfile)
	{
		this->m_engine = engine;
		this->m_maxfile = maxfile;

		// compute the mip map properites for future reference

		m_map_size_in_tiles = 0;

		for(int mip = MIP_MAX; mip >= MIP_MIN; mip--)
		{
			mipMapWidth[mip] 		= pow(2,mip);
			mipMapSizeInPixels[mip] = mipMapWidth[mip] * mipMapWidth[mip];
			mipMapWidthInTiles[mip] = mipMapWidth[mip] / TILE_SIZE;
			mipMapSizeInTiles[mip] 	= mipMapWidthInTiles[mip] * mipMapWidthInTiles[mip];

			m_map_size_in_tiles += mipMapSizeInTiles[mip];
		}

		bank_start_num = 0;
		num_banks_used = 1;
		bank_address_bits_offset = 25;

		m_alphaMaps.push_back(new AlphaMapParams(0));
		m_alphaMaps.push_back(new AlphaMapParams(1));
	}

private:

	int distanceToMipInPixels(int mipLevel)
	{
		return (int)((1 - pow(4, mipLevel + 1)) / (1 - 4));
	}



public:

	void SetNumMaps(int mapCount)
	{
		m_textures_data_size = m_map_size_in_tiles * mapCount * TILE_STRIDE_BYTES;
		m_textures_data = (char*)malloc(m_textures_data_size);
	}

	void LoadMap(string filename, int index)
	{
		SDL_Surface* map = IMG_Load(filename.c_str());

		if(map == NULL)
		{
			printf("ERROR: Could not load %s\n", filename.c_str());
			return;
		}

		for(int i = MIP_MAX; i >= MIP_MIN; i--)
		{
			ProcessMap(map, index, i);
		}
	}

	void LoadAlphaMap(int index, string filename)
	{
		SDL_Surface* map = IMG_Load(filename.c_str());

		if(map == NULL)
		{
			printf("ERROR: Could not load %s\n", filename.c_str());
		}

		AlphaMapParams* alphamap = NULL;
		for(uint i = 0; i < m_alphaMaps.size(); i++)
		{
			if(m_alphaMaps[i]->m_index == index){
				alphamap = m_alphaMaps[i];
				break;
			}
		}

		if(alphamap == NULL)
		{
			printf("ERROR: Alpha map slot %i does not exist.\n", index);
			return;
		}

		float scaleFactorX = ALPHA_MAP_DIMENSION / (float)map->w;
		float scaleFactorY = ALPHA_MAP_DIMENSION / (float)map->h;

		map = rotozoomSurfaceXY(map, 0, scaleFactorX, scaleFactorY, 1);

		for(int y = 0; y < ALPHA_MAP_DIMENSION; y++)
		{
			for(int x = 0; x < ALPHA_MAP_DIMENSION; x++)
			{
				char* pixelsData = (char*)map->pixels;
				char* pixelData = &pixelsData[((y * ALPHA_MAP_DIMENSION) + x) * map->format->BytesPerPixel];
				uint32_t pixelValue = *((uint32_t*)pixelData);

				uint8_t r,g,b;
				SDL_GetRGB(pixelValue, map->format, &r, &g, &b);

				int pixel = (y * ALPHA_MAP_DIMENSION) + x;
				int word = floor(pixel / PIXELS_PER_WORD);
				int offset = pixel - (word * PIXELS_PER_WORD);

				int v = (r + g + b)/3;

				uint64_t* worddata = &alphamap->m_data[word];
				uint64_t worddataMask = (1UL << offset);

				if(v > 16){
					*(worddata) = (*worddata) | worddataMask;
				}
			}
		}
	}

private:

	void ProcessMap(SDL_Surface* map, int index, int miplevel)
	{
		//mip the map

		float mipWidth = pow(2,miplevel);

		float scaleFactorX = mipWidth / (float)map->w;
		float scaleFactorY = mipWidth / (float)map->h;

		map = rotozoomSurfaceXY(map, 0.0f, scaleFactorX, scaleFactorY, 1);

		SDL_Rect SourcRect = {0, 0, TILE_SIZE, TILE_SIZE};
		SDL_Rect DestRect = {0, 0, TILE_SIZE, TILE_SIZE};
		SDL_Surface* tile = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

		//tile the map

		int mipTileWidth = mipWidth / TILE_SIZE;

		for(int x = 0; x < mipTileWidth; x++)
		{
			for(int y = 0; y < mipTileWidth; y++)
			{
				//compute address of tile
				int map_offset_in_tiles = m_map_size_in_tiles * index;
				int mip_offset_in_tiles = (distanceToMipInPixels(MIP_MAX) - distanceToMipInPixels(miplevel)) / (TILE_SIZE * TILE_SIZE);
				int tile_offset_in_tiles = (y * mipTileWidth) + x;

				int address_in_bytes = (map_offset_in_tiles + mip_offset_in_tiles + tile_offset_in_tiles) * TILE_STRIDE_BYTES;

				char* tileData = &m_textures_data[address_in_bytes];

				//copy the tile data into the staging memory

				SourcRect.x = (x * TILE_SIZE);
				SourcRect.y = (y * TILE_SIZE);

				SDL_BlitSurface(map, &SourcRect, tile, &DestRect);

				memcpy(tileData, tile->pixels, TILE_SIZE * TILE_SIZE * 4);
			}
		}
	}

public:

	void WriteEnvironmentMaps()
	{
		max_actions_t* actions[num_banks_used];
		for(int i = 0; i < num_banks_used; i++){

			int64_t bank_offset_in_bursts = (((int64_t)bank_start_num + (int64_t)i) << bank_address_bits_offset);
			int64_t bank_offset_in_bytes = bank_offset_in_bursts * BURSTSIZE_BYTES;
			int64_t address = bank_offset_in_bytes;

			max_actions_t* memact = max_actions_init(m_maxfile, "sampleMap_initialisation");

			max_set_param_uint64t(memact, "size", m_textures_data_size);
			max_set_param_uint64t(memact, "address", address);

			max_queue_input(memact,"sampleMap_fromCPU",m_textures_data,m_textures_data_size);
			actions[i] = memact;

			printf("writing environment map...(%i times)(address: %li)\n", num_banks_used, address);

			max_run(m_engine, memact);
		}
	}

	void WriteAlphaMaps(max_actions_t* memact)
	{
/*		uint64_t* values = (uint64_t*)malloc(16384 * sizeof(uint64_t));
		memset((uint32_t*)values, 0xFFFFFFFF, 16384 * 8);
		max_set_mem_range_uint64t(memact, "RayCasterKernel","alphaMapSlot2", 0, 16384, values);
		max_set_mem_range_uint64t(memact, "RayCasterKernel","alphaMapSlot3", 0, 16384, values);*/

		char slotName[128];
		for(uint i = 0; i < m_alphaMaps.size(); i++)
		{
			AlphaMapParams* alphaMap = m_alphaMaps[i];

			sprintf(slotName, "alphaMapSlot%i", alphaMap->m_index);
			max_set_mem_range_uint64t(memact, "RayCasterKernel",slotName, 0, alphaMap->m_numWords, alphaMap->m_data);
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
