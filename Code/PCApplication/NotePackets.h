#ifndef IVC_LASER_ORGAN
#define IVC_LASER_ORGAN
#include "Enums.h"
#include <string>

namespace IVCLaserOrgan
{
	struct MIDI_MessageType
	{
		char name[32];
		unsigned char command;
		unsigned short params;
	};

	class MIDI_Messages
	{
	public:
		static unsigned char getHexCode(unsigned char cmd)
		{
			// Inputs are expected to be [0, 7]
			// Make sure the returned value is between 0x80 and 0xFF
			return ((cmd | 0x08) << 4); 
		}
		static unsigned char getIntCode(unsigned char cmd)
		{
			// Extract the command type part of the command
			return ((cmd & 0xF0) - 0x0F) >> 4;
		}
		static unsigned char getChannel(unsigned char cmd)
		{
			// Channel [0, 15]
			return (cmd & 0x0F);
		}

		static char* getDescription(unsigned char cmd)
		{
			if (cmd > 7)
				cmd = getIntCode(cmd);
			return MIDI_MessageDefinitions[cmd].name;
		}
		static unsigned short getNumberOfParameters(unsigned char cmd)
		{
			if (cmd > 7)
				cmd = getIntCode(cmd);
			return MIDI_MessageDefinitions[cmd].params;
		}

		static MIDI_MessageType MIDI_MessageDefinitions[8];
		static char c[10];
	};

	MIDI_MessageType MIDI_Messages::MIDI_MessageDefinitions[] = 
	{
		{ "Note off", 0x80, 2 },
		{ "Note on", 0x90, 2 },
		{ "Aftertouch", 0xA0, 2 },
		{ "Continuous controller", 0xB0, 2 },
		{ "Patch change", 0xC0, 2 },
		{ "Channel pressure", 0xD0, 1 },
		{ "Pitch bend", 0xE0, 2 },
		{ "System message", 0xF0, 0 }
	};

}
#endif