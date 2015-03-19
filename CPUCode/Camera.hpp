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
	vector<float> u;
	vector<float> v;
	vector<float> w;

	Camera()
	{
		camera_eye = boost::assign::list_of(0)(0)(0);
		camera_lookat = boost::assign::list_of(0)(0)(1);
		camera_up = boost::assign::list_of(0)(-1)(0);
		connected = false;
	}

	virtual ~Camera()
	{
		if(connected)
		{
			max_llstream_release(camera_u_stream);
			max_llstream_release(camera_v_stream);
			max_llstream_release(camera_w_stream);
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

        w = subtract(camera_eye, camera_lookat);
        u = divide(cross(camera_up,w),(norm(cross(camera_up,w))));
        v = cross(w,u);

        update_camera_streams();
	}

	void connect(max_engine* engine)
	{
		printf("Preparing low latency stream for camera updates...\n");

		static const int num_slots = 100;
		static const int slot_size = 16;

		const int buffer_size = num_slots * slot_size;

		void* camera_u_buffer;
		void* camera_v_buffer;
		void* camera_w_buffer;
		void* camera_eye_buffer;

		posix_memalign(&camera_u_buffer, 16, buffer_size);
		posix_memalign(&camera_v_buffer, 16, buffer_size);
		posix_memalign(&camera_w_buffer, 16, buffer_size);
		posix_memalign(&camera_eye_buffer, 16, buffer_size);

		camera_u_stream = max_llstream_setup(engine, "camera_u", num_slots, slot_size, camera_u_buffer);
		camera_v_stream = max_llstream_setup(engine, "camera_v", num_slots, slot_size, camera_v_buffer);
		camera_w_stream = max_llstream_setup(engine, "camera_w", num_slots, slot_size, camera_w_buffer);
		camera_eye_stream = max_llstream_setup(engine, "camera_eye", num_slots, slot_size, camera_eye_buffer);

		connected = true;

		update_camera_streams();
	}

private:

	bool connected;
	max_llstream_t* camera_u_stream;
	max_llstream_t* camera_v_stream;
	max_llstream_t* camera_w_stream;
	max_llstream_t* camera_eye_stream;


	void update_camera_streams()
	{
		if(connected)
		{
			void* camera_u_slots;
			if(max_llstream_write_acquire(camera_u_stream, 1, &camera_u_slots))
			{
				memcpy(camera_u_slots, u.data(), sizeof(float) * 3);
				max_llstream_write(camera_u_stream, 1);
			}

			void* camera_v_slots;
			if(max_llstream_write_acquire(camera_v_stream, 1, &camera_v_slots))
			{
				memcpy(camera_v_slots, v.data(), sizeof(float) * 3);
				max_llstream_write(camera_v_stream, 1);
			}

			void* camera_w_slots;
			if(max_llstream_write_acquire(camera_w_stream, 1, &camera_w_slots))
			{
				memcpy(camera_w_slots, w.data(), sizeof(float) * 3);
				max_llstream_write(camera_w_stream, 1);
			}

			void* camera_eye_slots;
			if(max_llstream_write_acquire(camera_eye_stream, 1, &camera_eye_slots))
			{
				memcpy(camera_eye_slots, camera_eye.data(), sizeof(float) * 3);
				max_llstream_write(camera_eye_stream, 1);
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
