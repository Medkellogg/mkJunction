#include <Arduino.h>
#include <Bounce2.h>

// Pin assignments
const int routeA_pin = 26;
const int routeB_pin = 27;
const int routeC_pin = 14;
const int routeD_pin = 12;
const int routeE_pin = 23;
const int routeF_pin = 3;
const int routeG_pin = 19;
const int routeH_pin = 18;
const int latchPin = 33;
const int clockPin = 32;
const int dataPin = 25;

// Initialize Bounce objects
Bounce debouncerA = Bounce();
Bounce debouncerB = Bounce();
Bounce debouncerC = Bounce();
Bounce debouncerD = Bounce();
Bounce debouncerE = Bounce();
Bounce debouncerF = Bounce();
Bounce debouncerG = Bounce();
Bounce debouncerH = Bounce();

// Initial state of the 16 bits
uint16_t bits = 0b0100100100100100;

// Function to update the shift registers
void updateShiftRegisters() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bits >> 8); // Send high byte
  shiftOut(dataPin, clockPin, MSBFIRST, bits & 0xFF); // Send low byte
  digitalWrite(latchPin, HIGH);
}

// Function to toggle specific bits
void toggleBits(int bit1, int bit2, int bit3) {
  bits ^= (1 << bit1) | (1 << bit2) | (1 << bit3);
  updateShiftRegisters();
}

void setup() {
  // Set pin modes
  pinMode(routeA_pin, INPUT_PULLUP);
  pinMode(routeB_pin, INPUT_PULLUP);
  pinMode(routeC_pin, INPUT_PULLUP);
  pinMode(routeD_pin, INPUT_PULLUP);
  pinMode(routeE_pin, INPUT_PULLUP);
  pinMode(routeF_pin, INPUT_PULLUP);
  pinMode(routeG_pin, INPUT_PULLUP);
  pinMode(routeH_pin, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Attach debouncers to pins
  debouncerA.attach(routeA_pin);
  debouncerA.interval(8);
  debouncerB.attach(routeB_pin);
  debouncerB.interval(8);
  debouncerC.attach(routeC_pin);
  debouncerC.interval(5);
  debouncerD.attach(routeD_pin);
  debouncerD.interval(5);
  debouncerE.attach(routeE_pin);
  debouncerE.interval(5);
  debouncerF.attach(routeF_pin);
  debouncerF.interval(5);
  debouncerG.attach(routeG_pin);
  debouncerG.interval(5);
  debouncerH.attach(routeH_pin);
  debouncerH.interval(5);

  // Initialize shift registers
  updateShiftRegisters();
}

void loop() {
  // Update debouncers
  debouncerA.update();
  debouncerB.update();
  debouncerC.update();
  debouncerD.update();
  debouncerE.update();
  debouncerF.update();
  debouncerG.update();
  debouncerH.update();

  // Check for button presses and toggle corresponding bits
  if (debouncerA.fell()) toggleBits(0, 1, 2);
  if (debouncerB.fell()) toggleBits(3, 4, 5);
  if (debouncerC.fell()) toggleBits(6, 7, 8);
  if (debouncerD.fell()) toggleBits(9, 10, 11);
  if (debouncerE.fell()) toggleBits(12, 13, 14);
  
}