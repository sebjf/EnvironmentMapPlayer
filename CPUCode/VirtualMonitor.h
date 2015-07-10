/*
 * VirtualMonitor.h
 *
 *  Created on: 24 Jan 2014
 *      Author: sfriston
 */

#ifndef VIRTUALMONITOR_H_
#define VIRTUALMONITOR_H_

#include <string>
#include <ostream>
#include <fstream>
#include "MaxSLiCInterface.h"
#include <SDL/SDL.h>

#define SURFACE_PIXELSIZE 4
#define DFE_PIXELSIZE	8
#define MAXSLOTSIZE 4096

/*LDFLAGS   = $(O_FLAGS) $(shell slic-config --libs) $(RUNRULE_LDFLAGS) -lSDL*/

/* Virtual Monitor for Debugging Rendering Kernels running on the Simulator */

struct VirtualMonitorInfo
{
	int width;
	int height;
	int dualLink;

	size_t SURFACE_SIZE;

	size_t SLOT_SIZE;
	size_t NUM_SLOTS;
	size_t SLOTS_PER_FRAME;

	size_t displayData_size;
	void* displayData_ptr;

	max_llstream_t* displayData_llstream;

	SDL_Surface* surface;
	SDL_Surface* source;

	int pixelsReceived;
	int framesReceived;
	int slotsReceived;

	bool connected;
};

class VirtualMonitor
{
public:
	VirtualMonitor(int width, int height);
	VirtualMonitor(max_file_t* maxfile);
	~VirtualMonitor();

	void MirrorToFile(std::string filename);

	void Connect(max_engine_t* engine);
	void Refresh(int pixels_to_draw);
	void Refresh();

	void EnableRawMode()
	{
		enable_raw_mode = true;
	}

private:
	VirtualMonitorInfo monitor;
	max_file_t* m_maxfile;
	std::ofstream* file;
	bool enable_raw_mode; //When set the flags are ignored and all colour data is drawn to the screen
};

#endif /* VIRTUALMONITOR_H_ */
