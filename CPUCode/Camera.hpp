/*
 * Camera.h
 *
 *  Created on: Jan 19, 2015
 *      Author: demo
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <math.h>
#include <vector>
#include <errno.h>
#include "Types.hpp"

#define DEG2RAD	0.0174532925

using namespace std;

class Camera {
public:
	struct CameraUpdate
	{
		vector3 camera_eye;
		vector3 camera_lookat;
		vector3 camera_up;
		char	padding[28];
	};

	CameraUpdate camera_settings;


	Camera()
	{
		memset(&camera_settings, 0, sizeof(CameraUpdate));
		camera_settings.camera_lookat.z = 1;
		camera_settings.camera_up.y = 1;

		connected = false;
	}

	virtual ~Camera()
	{
		if(connected)
		{
			max_llstream_release(camera_stream);
		}
	}

	void set_eye(vector<float> v)
	{
		camera_settings.camera_eye.x = v[0];
		camera_settings.camera_eye.y = v[1];
		camera_settings.camera_eye.z = v[2];
	}

	void set_eye(float x, float y, float z)
	{
		camera_settings.camera_eye.x = x;
		camera_settings.camera_eye.y = y;
		camera_settings.camera_eye.z = z;
	}

	void set_eye(float* v)
	{
		camera_settings.camera_eye.x = v[0];
		camera_settings.camera_eye.y = v[1];
		camera_settings.camera_eye.z = v[2];
	}

	void set_lookat(float inclination, float azimuth)
	{
		inclination = DEG2RAD * inclination;
		azimuth = DEG2RAD * azimuth;

		camera_settings.camera_lookat = vector3::add(camera_settings.camera_eye, vector3((cos(inclination)*sin(azimuth)),(sin(inclination)),(cos(inclination)*cos(azimuth))));

		update_camera_streams();
	}

	void set_lookat(vector<float> v)
	{
		camera_settings.camera_lookat.x = camera_settings.camera_eye.x + v[0];
		camera_settings.camera_lookat.y = camera_settings.camera_eye.y + v[1];
		camera_settings.camera_lookat.z = camera_settings.camera_eye.z + v[2];

		update_camera_streams();
	}

	void set_lookat(float* v)
	{
		camera_settings.camera_lookat.x = camera_settings.camera_eye.x + v[0];
		camera_settings.camera_lookat.y = camera_settings.camera_eye.y + v[1];
		camera_settings.camera_lookat.z = camera_settings.camera_eye.z + v[2];

		update_camera_streams();
	}

	void set_up(vector<float> v)
	{
		camera_settings.camera_up.x = v[0];
		camera_settings.camera_up.y = v[1];
		camera_settings.camera_up.z = v[2];
	}

	void set_up(float* v)
	{
		camera_settings.camera_up.x = v[0];
		camera_settings.camera_up.y = v[1];
		camera_settings.camera_up.z = v[2];
	}

	void connect(max_engine* engine)
	{
		printf("Preparing low latency stream for camera updates...\n");

		static const int num_slots = 50;
		static const int slot_size = sizeof(CameraUpdate);

		const int buffer_size = num_slots * slot_size;

		void* camera_buffer;
		posix_memalign(&camera_buffer, 4096, buffer_size);
		camera_stream = max_llstream_setup(engine, "cameraUpdates", num_slots, slot_size, camera_buffer);

		connected = true;

		update_camera_streams();
	}

private:

	bool connected;
	max_llstream_t* camera_stream;

	void update_camera_streams()
	{
		if(connected)
		{
			void* camera_slots;
			if(max_llstream_write_acquire(camera_stream, 1, &camera_slots))
			{
				memcpy(camera_slots, &camera_settings, sizeof(CameraUpdate));
				max_llstream_write(camera_stream, 1);
			}
		}
	}
};

#endif /* CAMERA_H_ */
