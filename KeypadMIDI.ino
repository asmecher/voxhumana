/**
 * Vox Humana
 * by Alec Smecher, 2017
 * http://www.cassettepunk.com
 *
 * Permit an Arduino Leonardo to impersonate a simple MIDI keyboard.
 */

#include <pitchToNote.h>
#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <pitchToFrequency.h>

#include <Keypad.h>

// Send the state of the "Vox Humana" stop switch as a special note press/release.
const char STOP_SWITCH = pitchD7b;

const byte channel = 0;

const byte rows = 9;
const byte cols = 9;
char keys[rows][cols] = {
{pitchC1, pitchA1,  pitchG2b, pitchE3b, pitchC4, pitchA4, pitchG5b, pitchE6b, pitchC7},
  {pitchD1b, pitchB1b, pitchG2, pitchE3, pitchD4b, pitchB4b, pitchG5, pitchE6, STOP_SWITCH},
  {pitchD1, pitchB1, pitchA2b, pitchF3, pitchD4, pitchB4, pitchA5b, pitchF6, -1},
  {pitchE1b, pitchC2, pitchA2, pitchG3b, pitchE4b, pitchC5, pitchA5, pitchG6b, -1},
  {pitchE1, pitchD2b, pitchB2b, pitchG3, pitchE4, pitchD5b, pitchB5b, pitchG6, -1},
  {pitchF1, pitchD2, pitchB2, pitchA3b, pitchF4, pitchD5, pitchB5, pitchA6b, -1},
  {pitchG1b, pitchE2b, pitchC3, pitchA3, pitchG4b, pitchE5b, pitchC6, pitchA6, -1},
  {pitchG1, pitchE2, pitchD3b, pitchB3b, pitchG4, pitchE5, pitchD6b, pitchB6b, -1},
  {pitchA1b, pitchF2, pitchD3, pitchB3, pitchA4b, pitchF5, pitchD6, pitchB6, -1}
};
byte rowPins[rows] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
byte colPins[cols] = {11, 12, 13, A0, A1, A2, A3, A4, A5};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() {
  keypad.setDebounceTime(1);
}

void loop() {
  if (keypad.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (keypad.key[i].stateChanged) switch (keypad.key[i].kstate) {
        case PRESSED: {
          midiEventPacket_t noteOn = {0x09, 0x90 | channel, keypad.key[i].kchar, 127};
          MidiUSB.sendMIDI(noteOn);
          MidiUSB.flush();
        } break;
        case RELEASED: {
          midiEventPacket_t noteOff = {0x08, 0x80 | channel, keypad.key[i].kchar, 127};
          MidiUSB.sendMIDI(noteOff);
          MidiUSB.flush();
        } break;
      }
    }
  }
}
