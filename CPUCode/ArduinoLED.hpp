/*
 * ArduinoLED.hpp
 *
 *  Created on: 20 Aug 2015
 *      Author: sfriston
 */

#ifndef ARDUINOLED_HPP_
#define ARDUINOLED_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

class ArduinoLED
{
public:
	ArduinoLED()
	{
		m_comPort = open("/dev/ttyACM0", O_RDWR|O_NOCTTY);

		if(m_comPort <= 0)
		{
			printf("Could not open arduino com port - permissions are right?\n");
		}

		struct termios tty;
		memset(&tty,0,sizeof(struct termios));

		tcgetattr(m_comPort,&tty);

		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 20;

		tcsetattr(m_comPort, TCSANOW, &tty);

		WaitAck();
	}

	void On()
	{
		WriteByte('a');
	}

	void Off()
	{
		WriteByte('b');
	}

private:
	int m_comPort;

	void WriteByte(char byte)
	{
		write(m_comPort,&byte,1);
		if(WaitAck() != byte)
		{
			printf("Serial Port Error.");
		}
	}

	char WaitAck()
	{
		char value = 0;
		int numread = read(m_comPort,&value,1);
		return value;
	}
};

#endif /* ARDUINOLED_HPP_ */
