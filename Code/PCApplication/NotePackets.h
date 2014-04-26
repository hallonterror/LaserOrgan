#pragma once
#include "Enums.h"

namespace IVCLaserOrgan
{
	struct MIDI_MessageType
	{
		char name[32];
		unsigned char command;
		unsigned short params;
	};

	MIDI_MessageType MIDI_MessageDefinitions[] = {
		{ "Note off", 0x80, 2 },
		{ "Note on", 0x90, 2 },
		{ "Aftertouch", 0xA0, 2 },
		{ "Continuous controller", 0xB0, 2 },
		{ "Patch change", 0xC0, 2 },
		{ "Channel pressure", 0xD0, 1 },
		{ "Pitch bend", 0xE0, 2 },
		{ "System message", 0xF0, 0 } };
}