/*
 * CharacterController.hpp
 *
 *  Created on: 18 Mar 2015
 *      Author: sfriston
 */

#ifndef CHARACTERCONTROLLER_HPP_
#define CHARACTERCONTROLLER_HPP_

#include <linux/input.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <vector>
#include <boost/assign.hpp>

#define X 0
#define Y 1
#define Z 2

class CharacterController
{
public:
	CharacterController(char* device)
	{
		fd = open(device, O_RDONLY | O_NONBLOCK);
		if(fd < 0)
		{
			printf("Unable to open keyboard\n");
		}

		position = boost::assign::list_of(0)(0)(0);
		dposition = boost::assign::list_of(0)(0)(0);

		scale = 500;
	}

	void set_position(float x, float y, float z)
	{
		position[X] = x;
		position[Y] = y;
		position[Z] = z;
	}

	__u16 update()
	{
		__u16 ret = 0x0;

		rd = read(fd, &ev, sizeof(struct input_event));
		if(rd >= (int)sizeof(struct input_event)){
			if(ev.type == EV_KEY){
				switch(ev.code)
				{
				case KEY_Q:
					dposition[Y] = ev.value;
					break;
				case KEY_E:
					dposition[Y] = -ev.value;
					break;
				case KEY_W:
					dposition[Z] = ev.value;
					break;
				case KEY_S:
					dposition[Z] = -ev.value;
					break;
				case KEY_A:
					dposition[X] = -ev.value;
					break;
				case KEY_D:
					dposition[X] = ev.value;
					break;
				};
				ret = ev.code;
			}
		}

		position = add(position, divide(dposition, scale));

		return ret;

	}

	vector<float> position;
	float scale;

private:
	struct input_event ev;
	int fd;
	int rd;

	vector<float> dposition;


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
};


#endif /* CHARACTERCONTROLLER_HPP_ */
