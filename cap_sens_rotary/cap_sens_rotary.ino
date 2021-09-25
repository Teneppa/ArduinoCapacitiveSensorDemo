#include <CapacitiveSensor.h>

/*
 * This is a simple demo code made to demonstrate how to make a "potentiometer" / rotary encoder with capacitive
 * touch with only using 1M resistors, wires, aluminium foil and some paper.
 * 
 * A better circuit diagram can be found from the README.md!
 * 
 * - Teukkaniikka 2021 - https://www.youtube.com/watch?v=UD03HVoq1D4
 */

// Connect the pin 2 to the negative rail of the breadboard (NOTE: DO NOT CONNECT THE RAIL TO GND AT THE SAME TIME!)
// Then connect the resistors between the negative rail and the sense pins (from D3 to D10)
// The sense wires are then connected to the pins D3->D10
CapacitiveSensor cs[8] = {CapacitiveSensor(2, 3), CapacitiveSensor(2, 4), CapacitiveSensor(2, 5), CapacitiveSensor(2, 6), CapacitiveSensor(2, 7), CapacitiveSensor(2, 8), CapacitiveSensor(2, 9), CapacitiveSensor(2, 10)};


void setup() {
  // Connect an LED to this pin
  pinMode(11, OUTPUT);

  // Enable the autocalibration for the sensors
  // (the sensors are calibrated when the Arduino is reseted)
  for (uint8_t i = 0; i < 8; i++) {
    cs[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
  }
  Serial.begin(115200);
}

// This is the starting value for the "virtual potentiometer"
int16_t potValue = 0;

// Sensitivity = how much the potentiometer value changes per step
uint8_t sensitivity = 16;

// If the value was decreased, decrease it and check that it's in the limits
void potDecrease() {
  potValue -= sensitivity;
  if (potValue < 0) {
    potValue = 0;
  }
}

// If the value was increased, increased it and check that it's in the limits
void potIncrease() {
  potValue += sensitivity;
  if (potValue > 255) {
    potValue = 255;
  }
}

uint8_t lastVal = 0;
void loop() {
  analogWrite(11, potValue);

  // Go through all of the sensors, and find the one with the maximum value
  // If none of them are pressed, the index is -1
  long total[8];
  int8_t index = -1;
  uint16_t maxVal = 0;
  for (uint8_t i = 0; i < 8; i++) {
    total[i] = cs[i].capacitiveSensor(30);
    if (total[i] > maxVal && total[i] > 50) {
      maxVal = total[i];
      index = i;
    }
  }

  // If the sensor that was pressed was different from last time (not including index = -1)
  if (index != lastVal && index != -1) {
    Serial.println("IND: " + String(index) + "\tOLD:" + String(lastVal));
    Serial.println(potValue);

    // Find out the rotation direction based on the index
    // We have to do some extra trickery when the value goes from 7 -> 0 or vise versa
    if (!(index == 7 && lastVal == 0) && !(index == 0 && lastVal == 7)) {
      if (index > lastVal) {
        Serial.println("++++");
        potDecrease();
      }
      if (index < lastVal) {
        Serial.println("----");
        potIncrease();
      }

      // If we are going from index 7->0 or 0->7
    } else {
      if (index == 7 && lastVal == 0) {
        Serial.println("----");
        potIncrease();
      }
      if (index == 0 && lastVal == 7) {
        Serial.println("++++");
        potDecrease();
      }
    }

    // Set the previus value to the current one
    lastVal = index;
  }
}
