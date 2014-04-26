
#include <AltSoftSerial.h>

// NON CONFIGURABLE CONNECTIONS
// ----------------------------
// Hardware serial RX: 0 : 2
// Hardware serial TX: 1 : 3
#define RXs 8
#define TXs 9	

// CONFIGURABLE CONNECTIONS
// ----------------------------
// Serial Communication pins


// Three diagnostic LEDs
#define LED_ON 12
#define LED_STATUS 11
#define LED_ERROR 10 // PWM disabled

// PWM led Codes
#define LED_WAITING 0
#define LED_SENDING 127
#define LED_RECEIVING 255
#define LED_ON_DUTY 50

#define MIDI_BAUDRATE 31250
#define IVC_BAUDRATE 115200

AltSoftSerial MIDI;

byte input;

void setup()
{
  // Setup LEDs
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);

  // Setup serial input (in from IVC)
  Serial.begin(IVC_BAUDRATE);
  
  // Setup serial output (out to MIDI)
  MIDI.begin(MIDI_BAUDRATE);
  
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_STATUS, HIGH);
  digitalWrite(LED_ERROR, HIGH);
  delay(500);
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(LED_ERROR, LOW);
}

void loop()
{
  if (Serial.available())
  {
    analogWrite(LED_STATUS, LED_RECEIVING);
    input = Serial.read();

    analogWrite(LED_STATUS, LED_SENDING);
    MIDI.write(input);
  }
  else
    analogWrite(LED_STATUS, LED_WAITING);
}



