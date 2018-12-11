// This example is to get the last value of variable from the Ubidots API

// This example is to save multiple variables to the Ubidots API with TCP method

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#include "Particle.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "BBFF-YTP65d9ngV1useDm97rfZQ7j4QtijV"  // Put here your Ubidots TOKEN
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
  ubidots.setDebug(true);  //Uncomment this line for printing debug messages
}

void loop() {
  float value1 = analogRead(A0);
  float value2 = analogRead(A1);
  float value3 = analogRead(A2);
  ubidots.add("Variable_Name_One", value1);  // Change for your variable name
  unsigned long seconds = Time.now();
  unsigned int millis = 120;
  ubidots.add("Variable_Name_Two", value2, NULL, seconds, millis);
  ubidots.add("Variable_Name_Three", value3);

  bool bufferSent = false;
  if(ubidots.isDirty()){  // There are stored values in buffer
    //bufferSent = ubidots.sendValuesHttp();
    bufferSent = ubidots.sendValuesWebhook("xs", PRIVATE | WITH_ACK);
  }

  if(bufferSent){
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
