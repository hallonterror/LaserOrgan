
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <Line.h> // for envelope
#include <mozzi_midi.h>
#include <ADSR.h>
#include <mozzi_fixmath.h>

#define CONTROL_RATE 128

#define NUM_SAMPLES 2048 // If the sample tables are changed update the size here
#if NUM_SAMPLES == 2048
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h> // saw table for oscillator
#include <tables/square_no_alias_2048_int8.h> // square table for oscillator
#include <tables/triangle2048_int8.h> // triangle table for oscillator
#define NUM_WAVE_MODES 4
#elif NUM_SAMPLES == 4096
#include <tables/sin4096_int8.h> // sine table for oscillator
#include <tables/saw4096_int8.h> // saw table for oscillator
#include <tables/square_no_alias_4096_int8.h> // square table for oscillator
#define NUM_WAVE_MODES 3
#else
#include <tables/sin8192_int8.h> // sine table for oscillator
#include <tables/saw8192_int8.h> // saw table for oscillator
#include <tables/square_no_alias_8192_int8.h> // square table for oscillator
#define NUM_WAVE_MODES 3
#endif

// set the arduino digital pins to use (tested on arduino nano atmega328)
#define WAVE_SWITCH 12
#define LED 13

// Don't know much about these
#define ATTACK 50     // long enough for control rate to catch it
#define DECAY 50
#define SUSTAIN 60000 // Sustain 60 seconds unless a noteOff comes.
#define RELEASE 200
#define ATTACK_LEVEL 255
#define DECAY_LEVEL 255

// This is the maximum number notes played at the same time
const int numNotes = 6;

// Declare without a wavetable, and use setTable() later
Oscil <NUM_SAMPLES, AUDIO_RATE> oscillators[numNotes] = {
  Oscil <NUM_SAMPLES, AUDIO_RATE>(), Oscil <NUM_SAMPLES, AUDIO_RATE>(),
  Oscil <NUM_SAMPLES, AUDIO_RATE>(), Oscil <NUM_SAMPLES, AUDIO_RATE>(),
  Oscil <NUM_SAMPLES, AUDIO_RATE>(), Oscil <NUM_SAMPLES, AUDIO_RATE>()
};

// Envelope generators
ADSR <CONTROL_RATE, CONTROL_RATE> envelopes[numNotes] = {
  ADSR <CONTROL_RATE, CONTROL_RATE>(), ADSR <CONTROL_RATE, CONTROL_RATE>(),
  ADSR <CONTROL_RATE, CONTROL_RATE>(), ADSR <CONTROL_RATE, CONTROL_RATE>(),
  ADSR <CONTROL_RATE, CONTROL_RATE>(), ADSR <CONTROL_RATE, CONTROL_RATE>()
};

// Indicators for notes that are currently playing
byte notes[numNotes] = {0};

// Gain values to carry control rate envelope levels from updateControl() to updateAudio()
byte gains[numNotes] = {0};

// wave type control
boolean waveLatchOn = false; // acts as a semaphore to handle button push/release actions, to avoid looping
byte waveNumber = 1; // defines selected wave type

void HandleNoteOn(byte channel, byte note, byte velocity) {
  if (velocity == 0)
  {
    for (int i = 0; i < numNotes; ++i) {
      if (notes[i] == note) {
        envelopes[i].noteOff();
        notes[i] = 0;
      }
    }
  }
  else // finds the next available note (if any), starts it and assigns the calling note to corresponding oscillators
  {
    for (int i = 0; i < numNotes; ++i) {
      if (notes[i] == 0) {
        notes[i] = note;
        oscillators[i].setFreq_Q16n16(Q16n16_mtof(Q8n0_to_Q16n16(note)));
        envelopes[i].noteOn();
        break;
      }
    }
  }

  int noteSum = 0;
  for (int i = 0; i < numNotes; ++i)
    noteSum += notes[i];
  if (noteSum == 0) {
    digitalWrite(LED, LOW); // no notes playing
  }
  else {
    digitalWrite(LED, HIGH); // at least one note is playing
  }
}

void UpdateNotes()
{
  if (!Serial.available())
    return;

  short index = 0;
  byte data[3]; // channel, note, velocity;
  while (Serial.available())
  {
    byte inByte = (byte)Serial.read();

    // Resync here if we happen to fall out of sync
    // In the MIDI standard the only byte that is
    // allowed to start with 1 is the Status byte.
    if (inByte & 10000000)  // This is a new note
      index = 0;

    data[index] = inByte;
    index = (index + 1) % 3;

    // If index is 0 here it means we have
    // wrapped around from 2 and therefore
    // we have a new note.
    if (index == 0) {
      // Echo new note
      Serial.print(data[1]);
      Serial.print("\t");
      Serial.print(data[2]);
      HandleNoteOn(data[0], data[1], data[2]);
    }
  }
}

void setup() {

  pinMode(LED, OUTPUT); // set led to indicate playing note(s)
  pinMode(WAVE_SWITCH, INPUT); // receives input from pushbutton to select wave type

  // Initiate communications
  Serial.begin(115200);

  for (int i = 0; i < numNotes; ++i)
  {
    envelopes[i].setADLevels(ATTACK_LEVEL, DECAY_LEVEL);
    envelopes[i].setTimes(ATTACK, DECAY, SUSTAIN, RELEASE);
  }
  setWave(0);

  startMozzi(CONTROL_RATE);
  Serial.write("Synthesizer started");
}

void setWave(byte wave_num) {
  waveNumber = wave_num;

  // switch/case is faster than if/else
  switch (wave_num) {
    case 0:
      for (int i = 0; i < numNotes; ++i)
        oscillators[i].setTable(SIN2048_DATA);
      break;
    case 1:
      for (int i = 0; i < numNotes; ++i)
        oscillators[i].setTable(SAW2048_DATA);
      break;
    case 2:
      for (int i = 0; i < numNotes; ++i)
        oscillators[i].setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
#if NUM_WAVE_MODES > 3
    case 3:
      for (int i = 0; i < numNotes; ++i)
        oscillators[i].setTable(TRIANGLE2048_DATA);
      break;
#endif
  }
}

void updateControl() {
  UpdateNotes();

  boolean switch_pos = digitalRead(WAVE_SWITCH);
  // sets the next wave type upon button push action
  if (switch_pos == HIGH && waveLatchOn == false) {
    setWave((waveNumber + 1) % NUM_WAVE_MODES);
    waveLatchOn = true;
  }
  else if (switch_pos == LOW && waveLatchOn == true) {
    waveLatchOn = false; // release latch after button release action allowing new push action
  }

  for (int i = 0; i < numNotes; ++i) {
    envelopes[i].update();
    gains[i] = envelopes[i].next();
  }
}


int updateAudio() {
  return ((long)gains[0] * oscillators[0].next() +
          (int)gains[1] * oscillators[1].next() +
          (int)gains[2] * oscillators[2].next() +
          (int)gains[3] * oscillators[3].next() +
          (int)gains[4] * oscillators[4].next() +
          (int)gains[5] * oscillators[5].next()) >> 6;
}

void loop() {
  audioHook(); // required here
}








