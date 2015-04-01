/*
 * SampleParameterMap.hpp
 *
 *  Created on: Apr 1, 2015
 *      Author: demo
 */

#ifndef SAMPLEPARAMETERMAP_HPP_
#define SAMPLEPARAMETERMAP_HPP_

#include <string>
#include "SDL/SDL.h"
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"
#include <math.h>

/*https://www.libsdl.org/projects/SDL_image/release-1.2.html*/

#define BURSTSIZE_BYTES 384

class SampleParameterMap
{
public:
	struct __attribute__((__packed__)) SampleParameters_t
	{
		float col;
		float row;
		char  cam_scalar;
	};

	int m_offset_in_bursts;

private:
	max_engine_t* m_engine;
	max_file_t* m_maxfile;

	int m_width;
	int m_height;

	char* m_sampleParameterData;
	int m_sampleParameterDataSize;

	SampleParameters_t** m_SampleParameters;

public:
	SampleParameterMap(max_engine_t* engine, max_file_t* maxfile)
	{
		m_engine = engine;
		m_maxfile = maxfile;
		m_width = max_get_constant_uint64t(maxfile,"DisplayActiveWidth");
		m_height = max_get_constant_uint64t(maxfile,"DisplayActiveHeight");
		m_offset_in_bursts = 0;

		m_SampleParameters = new SampleParameters_t *[m_width];
		for ( int i = 0; i < m_width; i++ ){
			m_SampleParameters[i] = new SampleParameters_t[m_height];
		}
	}

	int GetOffsetInBursts()
	{
		return m_offset_in_bursts;
	}

	int GetOffsetInBytes()
	{
		return m_offset_in_bursts * BURSTSIZE_BYTES;
	}

	void InitialiseBasicMap()
	{
		for(int y = 0; y < m_height; y++)
		{
			for(int x = 0; x < m_width; x++)
			{
				m_SampleParameters[x][y].col = x;
				m_SampleParameters[x][y].row = y;
				m_SampleParameters[x][y].cam_scalar = 0;
			}
		}

		UpdateParameterMap();
	}

	void InitialiseBarrelDistortionMap()
	{
		for(int y = 0; y < m_height; y++)
		{
			for(int x = 0; x < m_width; x++)
			{
				m_SampleParameters[x][y].col = x;
				m_SampleParameters[x][y].row = y;
				m_SampleParameters[x][y].cam_scalar = 0;

				// first move into texture and then normalised device coordinates

				float rx = ((x / (float)m_width ) * 2) - 1;
				float ry = ((y / (float)m_height) * 2) - 1;

				// subtract the lens center from the sample in ndc

				float lens_center_x = 0.15;
				rx = rx - lens_center_x;

				// correct for the aspect ratio of the viewport

				float u_aspect = (float)m_width / (float)m_height;
				ry = ry / u_aspect;

				// calculate distortion scale

				float radius2 = (rx * rx) + (ry * ry);
				float distortion[] = { 1, .22f, .24f, 0.0f };

				float distortion_scale =
					(distortion[0]) +
					(distortion[1] * radius2) +
					(distortion[2] * radius2 * radius2) +
					(distortion[3] * radius2 * radius2 * radius2);

				rx = rx * distortion_scale;
				ry = ry * distortion_scale;

				// move the distorted coordiantes from ndc back into columns and
				// rows

				ry = ry * u_aspect;
				rx = rx + lens_center_x;

				rx = rx + 1;
				ry = ry + 1;

				rx = rx / 2;
				ry = ry / 2;

				rx = rx * m_width;
				ry = ry * m_height;

				m_SampleParameters[x][y].col = rx;
				m_SampleParameters[x][y].row = ry;
				m_SampleParameters[x][y].cam_scalar = 0;
			}
		}

		UpdateParameterMap();
	}

private:

	void UpdateParameterMap()
	{
		int samples_per_burst = (int)floor( BURSTSIZE_BYTES / sizeof(SampleParameters_t));
		float total_samples = m_width * m_height;
		float total_bursts = ceil(total_samples / samples_per_burst);
		m_sampleParameterDataSize = total_bursts * BURSTSIZE_BYTES;

		m_sampleParameterData = new char[m_sampleParameterDataSize];

		/* Initialise the sample data */

		for(int y = 0; y < m_height; y++)
		{
			for(int x = 0; x < m_width; x++)
			{
				int address_in_samples = x + (y * m_width);
				int address_in_bursts = floor((float)address_in_samples / (float)samples_per_burst);
				int offset_in_samples = address_in_samples % samples_per_burst;

				int address_in_bytes = (address_in_bursts * BURSTSIZE_BYTES) + (offset_in_samples * sizeof(SampleParameters_t));

				SampleParameters_t* parameters = (SampleParameters_t*)(m_sampleParameterData + address_in_bytes);
				*parameters = m_SampleParameters[x][y];
			}
		}

		max_actions_t* memact = max_actions_init(m_maxfile, "memoryInitialisation");
		max_set_param_uint64t(memact, "size", m_sampleParameterDataSize);
		max_set_param_uint64t(memact, "address", GetOffsetInBytes());
		max_queue_input(memact,"environment_map_in",m_sampleParameterData,m_sampleParameterDataSize);

		max_run(m_engine, memact);
	}

};


#endif /* SAMPLEPARAMETERMAP_HPP_ */
