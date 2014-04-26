#pragma once

#include <stdio.h>
#include <Windows.h>
#include <string>
#include "../ArduinoApplication/Enums.h"
#include "../ArduinoApplication/NotePackets.h"
#include "../ArduinoApplication/UnitStatus.h"

#include <iostream>
#include "MessagePrinting.h"

using namespace std;
using namespace IVCLaserOrgan;

namespace CustomSerial
{
	class SerialPort
	{
	public:
		SerialPort();
		~SerialPort();

		int Connect(string COM);
		int Setup(unsigned int Baudrate = 9600);

		int ReadMessage(UnitStatus& msg);
		int ReadMessage(unsigned char& cmd, unsigned char& chl, unsigned char * param);
		int WriteMessage(const unsigned char cmd, const unsigned char chl, const unsigned char * param);
		int WriteRaw(const unsigned char *data, unsigned int Size = 4);

		void Close();

	private:
		HANDLE h;
		DCB dcbConfig;
		COMMTIMEOUTS commTimeout;
	};

}