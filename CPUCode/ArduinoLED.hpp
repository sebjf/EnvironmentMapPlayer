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
	ArduinoLED(bool enabled)
	{
		connected = false;

		if(!enabled){
			return;
		}

		m_comPort = open("/dev/ttyACM0", O_RDWR|O_NOCTTY);

		if(m_comPort <= 0)
		{
			printf("Could not open arduino com port - permissions are right?\n");
			return;
		}

		int parity = 0;

		struct termios tty;
		memset(&tty,0,sizeof(struct termios));

		tcgetattr(m_comPort,&tty);

		cfsetospeed (&tty, 9600);
		cfsetispeed (&tty, 9600);

		tty.c_iflag &= ~IGNBRK;         // disable break processing
		tty.c_lflag = 0;                // no signaling chars, no echo,
		                                        // no canonical processing
		tty.c_oflag = 0;

		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 20;

		tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

		tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
										// enable reading
		tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
		tty.c_cflag |= parity;
		tty.c_cflag &= ~CSTOPB;
		tty.c_cflag &= ~CRTSCTS;

		tcsetattr(m_comPort, TCSANOW, &tty);

		WaitAck();

		connected = true;
		state = false;
		enable_loopback = false;
	}

	void On()
	{
		WriteByte('a');
		state = true;
	}

	void Off()
	{
		WriteByte('b');
		state = false;
	}

	void Invert()
	{
		if(state)
			Off();
		else
			On();
	}

private:
	int m_comPort;

	bool state;
	bool connected;
	bool enable_loopback;

	void WriteByte(char byte)
	{
		if(!connected)
		{
			return;
		}

		write(m_comPort,&byte,1);

		if(enable_loopback){
			if(WaitAck() != byte)
			{
				printf("Serial Port Error.\n");
			}
		}
	}

	char WaitAck()
	{
		char value = 0;
		int numread = read(m_comPort,&value,1);
		(void)numread;
		return value;
	}
};

#endif /* ARDUINOLED_HPP_ */
