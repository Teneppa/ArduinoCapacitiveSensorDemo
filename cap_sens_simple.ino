/*
   This is a simple demo by Teukkaniikka
*/

#include <CapacitiveSensor.h>

// Connect a 1M - 50M resistor between pins 2 and 3
// Then connect a sense wire to pin 3, and an LED to pin 13
CapacitiveSensor cs_btn = CapacitiveSensor(2, 3);

void setup() {
  // Enable automatic calibration
  cs_btn.set_CS_AutocaL_Millis(0xFFFFFFFF);

  pinMode(13, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  long totalBtn = cs_btn.capacitiveSensor(30);
  Serial.println(totalBtn);

  // If the sensor was pressed, LED = ON
  if (totalBtn > 40) {
    digitalWrite(13, 1);
  } else {
    digitalWrite(13, 0);
  }
}
