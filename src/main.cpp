/**********************************************************************
*    Jeroen Gerritsen's B&O McKenzie Division - Tortoise Control Software
*    by: Mark Kellogg                          
***********************************************************************

***********************************************************************
*                   VERSION 1.0  1/10/2025                            *
***********************************************************************

-----------------------------Github Repository-------------------------
* 
* The repository has all code etc. along with flow control overview and 
* module graphics. 
* Repository:  https://github.com/Medkellogg/mkJunction

*
-----------------------------------------------------------------------
*
*                   M&K Junction Control Panel Software
* This code is for use on the Control Panel Driver Board and its purpose
* is to control individual Tortoise switch motors and associated LEDs on 
* the fascia control panel.  
*
* This software version is configured to drive 5 different Tortoise motors 
* and 2 indicator LEDs for each motor.
* 
* Each turnout has 3 channels, 1 for the Tortoise, one for the "Normal" 
* route LED, and one for the "Reverse" route.
*  
* Upon pressing a "route" I/O button associated with the specific Tortise, 
* all three bits will flip and the polarity of the 3 output pins will reverse.
*
* The Control Panel Driver Board has a total of 16 output channels driven
* by H bridges and 8 general I/O pins.  
***********************************************************************/

#include <Arduino.h>
#include <Bounce2.h>

// Pin assignments: Route pins are connected to external push buttons to control routes
const int routeA_pin = 26;
const int routeB_pin = 27;
const int routeC_pin = 14;
const int routeD_pin = 12;
const int routeE_pin = 23;
const int routeF_pin =  3;
const int routeG_pin = 19;
const int routeH_pin = 18;
const int latchPin   = 33;
const int clockPin   = 32;
const int dataPin    = 25;
const uint8_t LED_PIN {2};  // LED_PIN drives the ESP32 LED and the relay on the
                            // Control Panel Driver Board

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

// Function to update the shift registers: Two shift registers provide 
// 16 bits to control the output of the H bridges

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

//---Blink routines for spelling out B&O in American Morse -----------

// Function to blink the LED pin and relay for an American Morse dot
void blinkLEDdot(int x) {
    for (int count {0}; count < x; ++count)
    {digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay (100);
    };
 }

// Function to blink the LED pin and relay for an American Morse dash

void blinkLEDdash(int x) {
    for (int count {0}; count < x; ++count)
    {digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay (300);
    };
 }

/*------------------------------------------------*
*               SET UP                            *
*-------------------------------------------------*/
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
  pinMode(latchPin,   OUTPUT);
  pinMode(clockPin,   OUTPUT);
  pinMode(dataPin,    OUTPUT);
  pinMode(LED_PIN,    OUTPUT);

  // Attach debouncers to pins
  debouncerA.attach(routeA_pin);
  debouncerA.interval(5);
  debouncerB.attach(routeB_pin);
  debouncerB.interval(5);
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

//-----tap out B&O in American Morse on relay/LED at end of boot sequence
  blinkLEDdash(1);
   blinkLEDdot(3);
      delay(1000);
   blinkLEDdot(1);
       delay(250);
   blinkLEDdot(3);
      delay(1000);
   blinkLEDdot(1);
       delay(250);
   blinkLEDdot(1);
      delay(1000);
}

//----------------- End Setup ------------------------

/*------------------------------------------------*
*               MAIN LOOP                         *
*-------------------------------------------------*/

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