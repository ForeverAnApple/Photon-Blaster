// This example is to get the last value of variable from the Ubidots API

// This example is to save multiple variables to the Ubidots API with TCP method

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "Your_Token"  // Put here your Ubidots TOKEN
#endif

Ubidots ubidots(TOKEN);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float value1 = analogRead(A0);
  unsigned long timestamp_seconds = Time.now();

  ubidots.add("temperature", value1, NULL, timestamp_seconds);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if(bufferSent){
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
