
// Emulating the IVC

#include <SoftwareSerial.h>

// NON CONFIGURABLE CONNECTIONS
// ----------------------------
// Hardware serial RX: 0
// Hardware serial TX: 1

// CONFIGURABLE CONNECTIONS
// ----------------------------
// Serial Communication pins
#define RXi 2			
#define TXi 3			
#define RXm	4			
#define TXm 5			

#define MIDI_BAUDRATE 31250
#define IVC_BAUDRATE 115200
#define PC_BAUDRATE 115200

SoftwareSerial IVC(RXi, TXi);
SoftwareSerial MIDI(RXm, TXm);

void setup()
{
	// Setup serial input (PC)
	Serial.begin(PC_BAUDRATE);
	Serial.flush();

	// Setup serial input (Out from MIDI box)
	MIDI.begin(MIDI_BAUDRATE);
	MIDI.flush();

	// Setup serial output (from this emulator to box)
	IVC.begin(IVC_BAUDRATE);
	IVC.flush();
}

void loop()
{
	uint8_t input, output;

	// Check for new input from PC
	if (Serial.available())
	{
		// Get the message from the PC
		input = Serial.read();

		// Write received data to PC
		Serial.print("Message sent (115200): ");
		Serial.println(input, DEC);

		// Send it to the MIDI interface
		IVC.write(input);

		// Read it at another baudrate
		while (!MIDI.available())
		{
		}
		output = MIDI.read();

		// Write result back to PC
		Serial.print("Message returned (31250): ");
		Serial.println(output, DEC);
	}
}
