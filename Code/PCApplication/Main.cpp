#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include "SerialPort.h"			// Includes robot communication

using namespace std;
using namespace CustomSerial;

int main()
{
	unsigned char data[255];
	unsigned int read;
	SerialPort sp;
	cout << sp.Connect("COM5") << endl;
	cout << sp.Setup() << endl;

	while (1)
	{
		sp.WriteRaw((const unsigned char *)100, 1);
		sp.ReadRaw(data, 255, &read);
		if (read != 0)
			cout << data << endl;
		//unsigned char cmd, chl;
		//cout << "Enter command code: ";
		//cin >> cmd;
		//if (cmd < 0 || cmd > 7)
		//	continue;

		//cout << "\nEnder the channel: ";
		//cin >> chl;
		//if (chl < 0 || chl > 15)
		//	continue;

		//MIDI_MessageType msg = MIDI_MessageDefinitions[cmd];
		//unsigned char * params = new unsigned char[msg.params];
		//for (int i = 0; i < msg.params; i++)
		//{
		//	cout << "\nEnter paramter " << i << ": ";
		//	cin >> params[i];
		//	if (params[i] < 0)
		//		params[i] = 0;
		//	if (params[i] > 127)
		//		params[i] = 127;
		//}

		//sp.WriteMessage(cmd, chl, params);

		//cout << "Message sent: " << cmd << ":" << chl << ":" << params[0];

		//sp.ReadMessage(cmd, chl, params);

		//cout << "Message read: " << cmd << ":" << chl << ":" << params[0];
	}

	sp.Close();
	cout << endl << "End" << endl;
	return 0;
}

