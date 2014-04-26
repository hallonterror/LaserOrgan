
#include <SoftwareSerial.h>

// NON CONFIGURABLE CONNECTIONS
// ----------------------------
// Hardware serial RX: 0 : 2
// Hardware serial TX: 1 : 3

// CONFIGURABLE CONNECTIONS
// ----------------------------
// Serial Communication pins
#define RXs	2			
#define TXs 3			

// Three diagnostic LEDs
#define LED_ON 9		
#define LED_ERROR 10	
#define LED_STATUS 11	

// PWM led Codes
#define LED_WAITING 0
#define LED_SENDING 127
#define LED_RECEIVING 255
#define LED_ON_DUTY 30

#define MIDI_BAUDRATE 31250
#define IVC_BAUDRATE 115200

SoftwareSerial MIDI(RXs, TXs);

void setup()
{
	// Enable pin for diagnostic LED
	pinMode(LED_ON, OUTPUT);
	pinMode(LED_ON, OUTPUT);
	pinMode(LED_STATUS, OUTPUT);
	analogWrite(LED_ON, LED_ON_DUTY);

	// Setup serial input (in from IVC)
	Serial.begin(IVC_BAUDRATE);

	// Setup serial output (out to MIDI)
	MIDI.begin(MIDI_BAUDRATE);
}

void loop()
{
	uint8_t input;
	if (Serial.available())
	{
		analogWrite(LED_STATUS, LED_RECEIVING);
		input = Serial.read();

		// Värdet OK här

		analogWrite(LED_STATUS, LED_SENDING);
		MIDI.write(input);
	}
	analogWrite(LED_STATUS, LED_WAITING);
}