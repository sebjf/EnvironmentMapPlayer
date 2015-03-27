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
	vector<float> camera_eye;
	vector<float> camera_lookat;
	vector<float> camera_up;

	Camera()
	{
		camera_eye = boost::assign::list_of(0)(0)(0);
		camera_lookat = boost::assign::list_of(0)(0)(1);
		camera_up = boost::assign::list_of(0)(-1)(0);	//Because the scanout of the display starts at 0 and increases as we move down, but the world coordinate system has Y increasing as we move up
		connected = false;
	}

	virtual ~Camera()
	{
		if(connected)
		{
			max_llstream_release(camera_e_stream);
			max_llstream_release(camera_l_stream);
			max_llstream_release(camera_u_stream);
		}
	}

	void set_eye(float x, float y, float z)
	{
		camera_eye[0] = x;
		camera_eye[1] = y;
		camera_eye[2] = z;
	}

	void set_lookat(float inclination, float azimuth)
	{
		inclination = DEG2RAD * inclination;
		azimuth = DEG2RAD * azimuth;

		camera_lookat = add(camera_eye, boost::assign::list_of(cos(inclination)*sin(azimuth))(sin(inclination))(cos(inclination)*cos(azimuth)));

        update_camera_streams();
	}

	void connect(max_engine* engine)
	{
		printf("Preparing low latency stream for camera updates...\n");

		static const int num_slots = 100;
		static const int slot_size = 16;

		const int buffer_size = num_slots * slot_size;

		void* camera_e_buffer;
		void* camera_l_buffer;
		void* camera_u_buffer;

		posix_memalign(&camera_e_buffer, 16, buffer_size);
		posix_memalign(&camera_l_buffer, 16, buffer_size);
		posix_memalign(&camera_u_buffer, 16, buffer_size);

		camera_e_stream = max_llstream_setup(engine, "camera_eye", 		num_slots, slot_size, camera_e_buffer);
		camera_l_stream = max_llstream_setup(engine, "camera_lookat", 	num_slots, slot_size, camera_l_buffer);
		camera_u_stream = max_llstream_setup(engine, "camera_up",		num_slots, slot_size, camera_u_buffer);

		connected = true;

		update_camera_streams();
	}

private:

	bool connected;
	max_llstream_t* camera_e_stream;
	max_llstream_t* camera_l_stream;
	max_llstream_t* camera_u_stream;


	void update_camera_streams()
	{
		if(connected)
		{
			void* camera_e_slots;
			if(max_llstream_write_acquire(camera_e_stream, 1, &camera_e_slots))
			{
				memcpy(camera_e_slots, camera_eye.data(), sizeof(float) * 3);
				max_llstream_write(camera_e_stream, 1);
			}

			void* camera_l_slots;
			if(max_llstream_write_acquire(camera_l_stream, 1, &camera_l_slots))
			{
				memcpy(camera_l_slots, camera_lookat.data(), sizeof(float) * 3);
				max_llstream_write(camera_l_stream, 1);
			}

			void* camera_u_slots;
			if(max_llstream_write_acquire(camera_u_stream, 1, &camera_u_slots))
			{
				memcpy(camera_u_slots, camera_up.data(), sizeof(float) * 3);
				max_llstream_write(camera_u_stream, 1);
			}

		}
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
