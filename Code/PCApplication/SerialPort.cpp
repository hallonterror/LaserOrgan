#include "SerialPort.h"
using namespace CustomSerial;

SerialPort::SerialPort() : h(INVALID_HANDLE_VALUE)
{
}


SerialPort::~SerialPort()
{
}

int SerialPort::Connect(string COM)
{
	// Make sure we are not already connected
	if (h != INVALID_HANDLE_VALUE)
	{
		CloseHandle(h);
		h = INVALID_HANDLE_VALUE;
	}

	// Create a new handle to the correct COM port
	if (COM.back() != ':')
		COM = COM + ":";
	wstring com_port(COM.begin(), COM.end());
	h = CreateFile(com_port.c_str(),
		GENERIC_READ | GENERIC_WRITE,	// access ( read and write)
		0,								// (share) 0:cannot share the COM port
		0,								// security  (None)
		OPEN_EXISTING,					// creation : open_existing
		0,								// we want nonoverlapped operation
		0								// no templates file for COM port...
		);

	// Check if handle is successfully created
	if (h == INVALID_HANDLE_VALUE)
		return FAILED_TO_CONNECT;
	
	return ALL_OK;
}

int SerialPort::Setup(unsigned int Baudrate)
{
	if (GetCommState(h, &dcbConfig))
	{
		dcbConfig.BaudRate = Baudrate;
		dcbConfig.ByteSize = 8;
		dcbConfig.Parity = NOPARITY;
		dcbConfig.StopBits = ONESTOPBIT;
		dcbConfig.fBinary = TRUE;
		dcbConfig.fParity = TRUE;
	}
	else
		return FAILED_TO_GET_COM_STATE;

	if (!SetCommState(h, &dcbConfig))
		return FAILED_TO_SET_COM_STATE;


	if (GetCommTimeouts(h, &commTimeout))
	{
		commTimeout.ReadIntervalTimeout = 50;
		commTimeout.ReadTotalTimeoutConstant = 50;
		commTimeout.ReadTotalTimeoutMultiplier = 10;
		commTimeout.WriteTotalTimeoutConstant = 50;
		commTimeout.WriteTotalTimeoutMultiplier = 10;
	}
	else
		return FAILED_TO_GET_TIMEOUTS;

	if (!SetCommTimeouts(h, &commTimeout))
		return FAILED_TO_SET_TIMEOUTS;

	return ALL_OK;

}

int SerialPort::ReadMessage(UnitStatus& msg)
{
	DWORD bytesRead;

	// Find message size
	unsigned int numBytes = sizeof(UnitStatus);
	char *data = new char[numBytes];

	int status = ALL_OK;
	if (ReadFile(h, data, numBytes, &bytesRead, NULL) != 0)
	{
		if ((unsigned int)bytesRead == numBytes)
			memcpy(&msg, data, (size_t)bytesRead);
		else
			status = READ_MESSAGE_SIZE_MISMATCH;
	}
	else
		status = READ_FAILED_TO_GET_MESSAGE;

	delete[] data;
	return status;
}

int SerialPort::ReadMessage(unsigned char& cmd, unsigned char& chl, unsigned char * param)
{
	DWORD bytesRead;

	if (ReadFile(h, &cmd, 1, &bytesRead, NULL) != 0)
	{
		chl = cmd & 0x0F;
		cmd = ((cmd & 0xF0) - 0x0F) >> 4;
	}
	else
		return READ_NO_MESSAGE_AVAILABLE;

	MIDI_MessageType msg = MIDI_MessageDefinitions[cmd];
	if (ReadFile(h, param, msg.params, &bytesRead, NULL) != 0)
	{
		if (bytesRead != msg.params)
			return READ_MESSAGE_SIZE_MISMATCH;
	}
	else
		return READ_FAILED_TO_GET_MESSAGE;

	return ALL_OK;
}

int SerialPort::WriteMessage(const unsigned char cmd, const unsigned char chl, const unsigned char * param)
{
	// Find message type
	if (cmd < 0 || cmd >= NUMBER_OF_MESSAGE_ENUMS)
		return UNKNOWN_COMMAND;
	MIDI_MessageType msg = MIDI_MessageDefinitions[cmd];

	// Generate data structure for message
	unsigned char * outData = new unsigned char[msg.params + 1];

	// Create the MIDI_Message
	if (chl < CHANNEL_START || chl > CHANNEL_END)
		return CHANNEL_ERROR;
	outData[0] = msg.command + chl;

	for (int index = 0; index < msg.params; index++)
	{
		if (param[index] < DATA_BYTES_START || param[index] > DATA_BYTES_END)
			return PARAMETER_ONE_ERROR + index;
		outData[1 + index] = param[index];
	}

	// Write to serial
	int writeStatus = WriteRaw(outData, sizeof(msg.params + 1));

	// Free dynamic memory
	delete[] outData;

	return writeStatus;
}

int SerialPort::WriteRaw(const unsigned char *data, unsigned int Size)
{
	DWORD bytes_written;
	WriteFile(h, data, Size, &bytes_written, NULL);
	
	if ((int)bytes_written != Size)
		return WARNING_WRITE_MISMATCH;

	return ALL_OK;
}

void SerialPort::Close()
{
	if (h != INVALID_HANDLE_VALUE)
	{
		CloseHandle(h);
		h = INVALID_HANDLE_VALUE;
	}
}