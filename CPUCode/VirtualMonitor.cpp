/*
 * VirtualMonitor.cpp
 *
 *  Created on: 24 Jan 2014
 *      Author: sfriston
 */


#include "VirtualMonitor.h"

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <algorithm>
#include <fstream>

#include <SDL/SDL.h>
#include <stdint.h>

/* This macro simplifies accessing a given pixel component on a surface. */
#define pel(surf, x, y, rgb) ((unsigned char *)(surf->pixels))[y*(surf->pitch)+x*3+rgb]

/* PCIe is big-endian and Intel CPUs are little endian so the order of the bytes (but not the bits in the bytes) is reversed */

struct DisplayPixel
{
	char A;
	char B;
	char G;
	char R;
};

struct PrimaryPixel
{
	char R;
	char G;
	char B;
	char Control;
};

struct SecondaryPixel
{
	char R;
	char G;
	char B;
	char Metadata;
};

struct DualLinkPixel
{
	PrimaryPixel odd;
	SecondaryPixel even;
};

#define DE				(0x1 << 0)
#define HSync 			(0x1 << 1)
#define VSync 			(0x1 << 2)
#define StartOfFrame 	(0x1 << 3)
#define EndOfFrame 		(0x1 << 4)

void DrawIntialSurface(VirtualMonitorInfo &monitor)
{
	DisplayPixel* surface_data = (DisplayPixel*)monitor.source->pixels;
	for(int y = 0; y < monitor.height; y++){
		for(int x = 0; x < monitor.width; x++){

			int cp = (y * monitor.width) + x;

			((uint32_t*)surface_data)[cp] = 0x00000000;

			if(y % 2 == 0){
				if(x % 2 == 0){
					((uint32_t*)surface_data)[cp] = 0xFF000000;
				}
			}else{
				if(x % 2 != 0){
					((uint32_t*)surface_data)[cp] = 0x00000000;
				}
			}
		}
	}
}

void CreateSurfaceForMonitor(VirtualMonitorInfo &monitor)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("Virtual Monitor","Virtual Monitor");
	monitor.surface = SDL_SetVideoMode(monitor.width, monitor.height, 32, SDL_SWSURFACE);
	SDL_SetAlpha(monitor.surface, 0x0, 0xFF);
	monitor.source = SDL_CreateRGBSurface(0,monitor.width, monitor.height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000 );
	DrawIntialSurface(monitor);
}

void RedrawMonitorFromSource(VirtualMonitorInfo &monitor)
{
	SDL_BlitSurface(monitor.source,NULL,monitor.surface,NULL);
	SDL_Flip(monitor.surface);
}

void InitialiseMonitor(VirtualMonitorInfo &monitor, int width, int height)
{
	monitor.width = width;
	monitor.height = height;

	monitor.SURFACE_SIZE = width * height * SURFACE_PIXELSIZE;

	int INCOMING_FRAME_SIZE = (width * height * DFE_PIXELSIZE);

	int slot_size = 4096;
	while(INCOMING_FRAME_SIZE % slot_size != 0)
	{
		slot_size = slot_size >> 1;
	}

	monitor.SLOT_SIZE = slot_size;
	monitor.NUM_SLOTS = 512;
	monitor.SLOTS_PER_FRAME = INCOMING_FRAME_SIZE / monitor.SLOT_SIZE;

	monitor.displayData_size = monitor.SLOT_SIZE * monitor.NUM_SLOTS;

	void* write_buffer = NULL;
	if(posix_memalign(&write_buffer, 4096, monitor.displayData_size) == ENOMEM){
		printf("Could not allocate memory.");
	}

	memset(write_buffer,0,monitor.displayData_size );
	monitor.displayData_ptr = write_buffer;

	printf("Calculated slot size of %i (%i slots per frame)\n", (int)monitor.SLOT_SIZE, (int)monitor.SLOTS_PER_FRAME);

	monitor.pixelsReceived = 0;
	monitor.framesReceived = 0;
	monitor.slotsReceived = 0;

	monitor.connected = false;
}

VirtualMonitor::VirtualMonitor(max_file_t* maxfile)
{
	int width = (int)max_get_constant_uint64t(maxfile, "DisplayTotalWidth");
	int height = (int)max_get_constant_uint64t(maxfile, "DisplayTotalHeight");
	InitialiseMonitor(monitor, width, height);
	file = NULL;
	enable_raw_mode = false;
	m_maxfile = maxfile;
	enable_saving_frames = false;
}

VirtualMonitor::VirtualMonitor(int width, int height)
{
	InitialiseMonitor(monitor, width, height);
	file = NULL;
	enable_raw_mode = false;
	m_maxfile = NULL;
	enable_saving_frames = false;
}

void VirtualMonitor::Connect(max_engine_t* engine)
{
	printf("Preparing low latency stream for virtual monitor...\n");

	if(m_maxfile != NULL)
	{
		if(!max_has_handle_stream(m_maxfile, "displayDataOut"))
		{
			printf("Maxfile does not have a displayDataOut stream.\n");
			return;
		}
	}

	max_llstream_t* read_llstream = 0;
	read_llstream = max_llstream_setup(engine, "displayDataOut", monitor.NUM_SLOTS, monitor.SLOT_SIZE, monitor.displayData_ptr);
	monitor.displayData_llstream = read_llstream;

	CreateSurfaceForMonitor(monitor);
	RedrawMonitorFromSource(monitor);

	monitor.connected = true;

}

void VirtualMonitor::Refresh()
{
	Refresh(monitor.width);
}

void VirtualMonitor::Refresh(int pixels_to_update)
{
	if(!monitor.connected)
	{
		return;
	}

	uint slots_got = 0;
	void* frame_data;

	int pixels_per_slot = (monitor.SLOT_SIZE / DFE_PIXELSIZE);

	uint SLOTS_TO_READ = std::max(pixels_to_update / pixels_per_slot, 1);

	while(slots_got < SLOTS_TO_READ){

		int slots_to_get = monitor.SLOTS_PER_FRAME - slots_got;
		slots_to_get = std::min(512,slots_to_get);

		int num_slots = max_llstream_read(monitor.displayData_llstream, slots_to_get, &frame_data);
		int num_pixels = num_slots * pixels_per_slot;

		for(int np = 0; np < num_pixels; np++){

			DisplayPixel* surface_data = (DisplayPixel*)monitor.source->pixels;

			DualLinkPixel pixel_data = ((DualLinkPixel*)frame_data)[np];

			if(file != NULL)
			{
				file->write((char*)&pixel_data, sizeof(DualLinkPixel));
			}

			if( pixel_data.odd.Control & StartOfFrame){
				monitor.pixelsReceived = 0;
			}

			int current_pixel = monitor.pixelsReceived % (monitor.width * monitor.height);

			if((pixel_data.odd.Control & DE) | enable_raw_mode){
				surface_data[current_pixel].R = pixel_data.odd.R;
				surface_data[current_pixel].G = pixel_data.odd.G;
				surface_data[current_pixel].B = pixel_data.odd.B;
			}

	//		if(pixel_data.odd.Control & DE){
	//			((uint32_t*)surface_data)[current_pixel] = 0xFFFFFFFF;
	//		}

			if(pixel_data.odd.Control & StartOfFrame){
				((uint32_t*)surface_data)[current_pixel] = 0xFFFF0000;
			}

			if(pixel_data.even.Metadata != 0 && false){
				((uint32_t*)surface_data)[current_pixel] = 0xFF000000;
			}

			if(monitor.pixelsReceived == 0)
			{
				SaveCurrentFrame();
			}

			monitor.pixelsReceived++;
		}

		max_llstream_read_discard(monitor.displayData_llstream, num_slots);

		slots_got += num_slots;
		monitor.slotsReceived += num_slots;

		if(num_slots <= 0){ //if there is no data the graph could be deadlocked, so just leave it for now and continue the next time refresh is called
			break;
		}
	}

	RedrawMonitorFromSource(monitor);

}

void VirtualMonitor::MirrorToFile(std::string filename)
{
	file = new std::ofstream(filename.c_str(), std::ofstream::out);
}

void VirtualMonitor::SaveFrames(std::string filenameformat) //must have one integer parameter
{
	frame_filename_format = filenameformat;
	enable_saving_frames = true;
	saved_frame_count = 0;
}

void VirtualMonitor::SaveCurrentFrame()
{
	if(enable_saving_frames)
	{
		char filename[256];
		sprintf(filename, frame_filename_format.c_str(), saved_frame_count);
		SDL_SaveBMP(monitor.source, filename);
		saved_frame_count++;
	}
}

VirtualMonitor::~VirtualMonitor()
{
	if(monitor.connected){
		max_llstream_release(monitor.displayData_llstream);

		SDL_FreeSurface(monitor.surface);
		SDL_FreeSurface(monitor.source);

		SDL_Quit();
	}

	if(file != NULL){
		file->close();
		delete file;
	}

	free(monitor.displayData_ptr);
}
