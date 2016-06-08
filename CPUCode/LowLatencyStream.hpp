/*
 * LowLatencyStream.h
 *
 *  Created on: 7 Jun 2016
 *      Author: sfriston
 */

#ifndef LOWLATENCYSTREAM_H_
#define LOWLATENCYSTREAM_H_

#include "MaxSLiCInterface.h"
#include <string>

/* Packet based low latency stream (with packet type T) */

template <typename T>
class LowLatencyStream {

	max_file_t* m_maxfile;
	const char* m_name;
	max_llstream_t* m_llstream;

	int slot_size;
	int num_slots;

	bool connected;

public:
	LowLatencyStream(string name, max_file_t* maxfile)
	{
		m_maxfile = maxfile;
		m_name = name.c_str();

		slot_size = sizeof(T);

		if(slot_size % 16 != 0)
		{
			throw "Low Latency Stream Packet Type is Not a Multiple of PCIe Word Size";
		}

		num_slots = 50;
	}

	void Connect(max_engine_t* engine)
	{
		if(m_maxfile != NULL)
		{
			if(!max_has_handle_stream(m_maxfile, m_name))
			{
				printf("Maxfile does not have stream %s\n", m_name);
				return;
			}
		}

		const int buffer_size = num_slots * slot_size;

		void* stream_buffer;
		posix_memalign(&stream_buffer, 4096, buffer_size);
		m_llstream = max_llstream_setup(engine, m_name, num_slots, slot_size, stream_buffer);

		connected = true;
	}

	void Send(T packet)
	{
		if(connected)
		{
			void* pSlots;
			if(max_llstream_write_acquire(m_llstream, 1, &pSlots))
			{
				memcpy(pSlots, &packet, sizeof(T));
				max_llstream_write(m_llstream, 1);
			}
		}
	}

private:

	virtual ~LowLatencyStream()
	{
		if(connected)
		{
			max_llstream_release(m_llstream);
		}
	}
};

#endif /* LOWLATENCYSTREAM_H_ */
