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
#include <boost/assign.hpp>
#include <errno.h>

#define DEG2RAD	0.0174532925

using namespace std;

class Camera {
public:

	struct vector3
	{
		float x;
		float y;
		float z;

		vector3(){}
		vector3(float ix, float iy, float iz){ x = ix; y = iy; z = iz; }
	};

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
		camera_settings.camera_up.y = -1; 		//Because the scanout of the display starts at 0 and increases as we move down, but the world coordinate system has Y increasing as we move up

		connected = false;
	}

	virtual ~Camera()
	{
		if(connected)
		{
			max_llstream_release(camera_stream);
		}
	}

	void set_eye(float x, float y, float z)
	{
		camera_settings.camera_eye.x = x;
		camera_settings.camera_eye.y = y;
		camera_settings.camera_eye.z = z;
	}

	void set_lookat(float inclination, float azimuth)
	{
		inclination = DEG2RAD * inclination;
		azimuth = DEG2RAD * azimuth;

		camera_settings.camera_lookat = add(camera_settings.camera_eye, vector3((cos(inclination)*sin(azimuth)),(sin(inclination)),(cos(inclination)*cos(azimuth))));

		update_camera_streams();
	}

	void set_ovr(vector3 forward, vector3 up)
	{
		camera_settings.camera_lookat = add(camera_settings.camera_eye, forward);
		camera_settings.camera_up = up;

		update_camera_streams();
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

	vector3 add(vector3 a, vector3 b)
	{
		vector3 ans;
		ans.x = (a.x + b.x);
		ans.y = (a.y + b.y);
		ans.z = (a.z + b.z);
		return ans;
	}

	vector<float> add(vector<float> a, vector<float> b)
	{
		vector<float> ans(3);
		ans[0] = (a[0] + b[0]);
		ans[1] = (a[1] + b[1]);
		ans[2] = (a[2] + b[2]);
		return ans;
	}

	vector<float> divide(vector<float> a, float b)
	{
		vector<float> ans(3);
		ans[0] = (a[0] / b);
		ans[1] = (a[1] / b);
		ans[2] = (a[2] / b);
		return ans;
	}

	vector<float> subtract(vector<float> a, vector<float> b)
	{
		vector<float> ans(3);
		ans[0] = (a[0] - b[0]);
		ans[1] = (a[1] - b[1]);
		ans[2] = (a[2] - b[2]);
		return ans;
	}

	float norm(vector<float> v)
	{
		float sum = 0;
		for(unsigned int i = 0; i < v.size(); i++)
		{
			sum += (v[i] * v[i]);
		}
		return sqrt(sum);
	}

	vector<float> cross(vector<float> a, vector<float> b)
	{
		vector<float> ans(3);

		const int x = 0;
		const int y = 1;
		const int z = 2;

		ans[0] = ((a[y]*b[z]) - (a[z]*b[y]));
		ans[1] = ((a[z]*b[x]) - (a[x]*b[z]));
		ans[2] = ((a[x]*b[y]) - (a[y]*b[x]));

		return ans;
	}

};

#endif /* CAMERA_H_ */
