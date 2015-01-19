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
		camera_up = boost::assign::list_of(0)(1)(0);
	}

	virtual ~Camera() {}

	void set_lookat(float inclination, float azimuth)
	{
		inclination = DEG2RAD * inclination;
		azimuth = DEG2RAD * azimuth;

		camera_lookat = boost::assign::list_of(cos(inclination)*sin(azimuth))(sin(inclination))(cos(inclination)*cos(azimuth));

        w = subtract(camera_eye, camera_lookat);
        u = divide(cross(camera_up,w),(norm(cross(camera_up,w))));
        v = cross(w,u);
	}

private:

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
