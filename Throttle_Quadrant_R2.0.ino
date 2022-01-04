/**

   Using Project: ArduinoJoystickLibrary
   Source: https://github.com/MHeironimus/ArduinoJoystickLibrary
   Author: Matthew Heironimus
   Library: Joystick.h

*/

#include "Joystick.h"
Joystick_ Joystick(0x06, 0x04,
                   1, 0,                 // Button Count, Hat Switch Count
                   false, false, false,   // No Axis
                   true, true, true,   // Rx, Ry and Rz
                   false, true,          // No Rudder and throttle
                   false, false, false);  // No accelerator, brake or steering

// Potentiometer pins (Make sure these matches your configuration);

int throttle_pin = A0;
int prop_pin = A1;
int mixt_pin = A2;

// Calibration (These are the only ones you're going to change)

float throttle_lever_full_up = 851;
float throttle_lever_down_notch = 530;
float throttle_lever_full_down = 458;

float prop_lever_full_up = 469;
float prop_lever_full_down = 52;

float cutoff_lever_full_up = 590;
float cutoff_lever_full_down = 1014;

// Throttle Quadrant Arrays

int throttle_array[4];
int throttle_array_old[4];

// Throttle Lever and Reverse Button

float thr_max = 1023 - throttle_lever_full_up;
float thr_min = 1023 - throttle_lever_down_notch;
float deadzone = 7; // Analog units for deadzone.
float rev_min = thr_min + deadzone;
float rev_max = 1023 - throttle_lever_full_down;
float reverso = 0; //Toggle Reverse Button

// Propeller
int prop_max = 1023 - prop_lever_full_up;
int prop_min = 1023 - prop_lever_full_down;

// Mixture
int mixt_max = cutoff_lever_full_up;
int mixt_min = cutoff_lever_full_down;

// Potentiometer "Deadzone" to update Joystick (analog units)
int dif = 3;


unsigned long startMillisT;
unsigned long currentMillisT;
const unsigned long timeframeT = 15;
unsigned long startMillisX;
unsigned long currentMillisX;
const unsigned long timeframeX = 15;
unsigned long startMillisY;
unsigned long currentMillisY;
const unsigned long timeframeY = 15;
unsigned long startMillisZ;
unsigned long currentMillisZ;
const unsigned long timeframeZ = 15;
unsigned long startMillisB;
unsigned long currentMillisB;
const unsigned long timeframeB = 15;


void setup() {

  //Initialize Joystick Library
  Joystick.begin();

  // Initializes serial port.
  //Serial.begin(9600);
  // Waits for serial port to connect (needed for Leonardo only).
  //while (!Serial);

  // Joystick Axes Setup

  // Throttle
  Joystick.setThrottleRange(thr_min, thr_max);
  // Prop
  Joystick.setRxAxisRange(prop_min, prop_max);
  // Mixture/Cutoff
  Joystick.setRyAxisRange(mixt_min, mixt_max);
  // Reverse
  Joystick.setRzAxisRange(rev_min, rev_max);

  startMillisT = millis();
  startMillisX = millis();
  startMillisY = millis();
  startMillisZ = millis();
  startMillisB = millis();
}
void loop() {
  potentiometers();
}

void potentiometers() {

  throttle_array[0] = 1023 - analogRead(throttle_pin);
  throttle_array[1] = 1023 - analogRead(prop_pin);
  throttle_array[2] = analogRead(mixt_pin);
  throttle_array[3] = throttle_array[0];


  //  Serial.print(analogRead(throttle_pin));
  //  Serial.print(" ");
  //  Serial.print(analogRead(prop_pin));
  //  Serial.print(" ");
  //  Serial.println(analogRead(mixt_pin));

  currentMillisB = millis();
  if (currentMillisB - startMillisB >= timeframeB) {
    if (abs(throttle_array[0] - throttle_array_old[0]) >= dif) {
      if (throttle_array[0] >= thr_min + deadzone) {
        // Toggles the reverse button when it exits the threshold (1st time)
        Joystick.setButton(0, 1);
      }
      Joystick.setThrottle(throttle_array[0]);
      if (throttle_array[0] <= rev_min) {
        Joystick.setButton(0, 0);
      }
      throttle_array_old[0] = throttle_array[0];
    }
    startMillisB = currentMillisB;
  }

  currentMillisT = millis();
  if (currentMillisT - startMillisT >= timeframeT) {
    if (abs(throttle_array[0] - throttle_array_old[0]) >= dif) {
      Joystick.setThrottle(throttle_array[0]);
      throttle_array_old[0] = throttle_array[0];
    }
    startMillisT = currentMillisT;
  }

  currentMillisX = millis();
  if (currentMillisX - startMillisX >= timeframeX) {
    if (abs(throttle_array[1] - throttle_array_old[1]) >= dif) {
      Joystick.setRxAxis(throttle_array[1]);
      throttle_array_old[1] = throttle_array[1];
    }
    startMillisX = currentMillisX;
  }

  currentMillisY = millis();
  if (currentMillisY - startMillisY >= timeframeY) {
    if (abs(throttle_array[2] - throttle_array_old[2]) >= dif) {
      Joystick.setRyAxis(throttle_array[2]);
      throttle_array_old[2] = throttle_array[2];
    }
    startMillisY = currentMillisY;
  }

  currentMillisZ = millis();
  if (currentMillisZ - startMillisZ >= timeframeZ) {
    if (abs(throttle_array[3] - throttle_array_old[3]) >= dif) {
      Joystick.setRzAxis(throttle_array[3]);
      throttle_array_old[3] = throttle_array[3];
    }
    startMillisZ = currentMillisZ;
  }

}
