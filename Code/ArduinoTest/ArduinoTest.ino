
// Emulating the IVC

//#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

// NON CONFIGURABLE CONNECTIONS
// ----------------------------
// Hardware serial RX: 0
// Hardware serial TX: 1
#define MIDI_RX	8			
#define MIDI_TX 9	

// CONFIGURABLE CONNECTIONS
// ----------------------------
// Serial Communication pins
//#define IVC_RX 2			
//#define IVC_TX 3			
		

#define MIDI_BAUDRATE 31250
//#define IVC_BAUDRATE 115200
#define PC_BAUDRATE 115200

//SoftwareSerial IVC(IVC_RX, IVC_TX);
AltSoftSerial MIDI;

void setup()
{
  // Setup serial input (PC)
  Serial.begin(PC_BAUDRATE);

  // Setup serial input (Out from MIDI box)
  MIDI.begin(MIDI_BAUDRATE);

  // Setup serial output (from this emulator to box)
  //IVC.begin(IVC_BAUDRATE);
}

void loop()
{
  byte input, output;

  // Check for new input from PC
  if (Serial.available())
  {
    // Get the message from the PC
    input = Serial.read();

    // Write received data to PC
    Serial.print("Message sent (115200): ");
    Serial.print(input, DEC);
    Serial.print(" at time: ");
    Serial.println(millis());

    // Send it to the MIDI interface
    //IVC.write(input);
  }

  if (MIDI.available())
  {
    output = MIDI.read();

    // Write result back to PC
    Serial.print("Message returned (31250): ");
    Serial.print(output, DEC);
    Serial.print(" at time: ");
    Serial.println(millis());
  }
}

