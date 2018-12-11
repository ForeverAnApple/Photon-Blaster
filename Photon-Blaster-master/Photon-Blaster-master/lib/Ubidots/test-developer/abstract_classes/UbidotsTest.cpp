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

Ubidots ubidots(TOKEN, UBI_EDUCATIONAL, UBI_PARTICLE);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);
}

void loop() {
  //ubidots.add("variable", 1, "\"key-1\"=1", 100);
  // ubidots.addContext("key-1", 1);
  // ubidots.addContext("key-2", 2);
  // char* context = ubidots.getContext();

  // ubidots.addContext("key-1", "sunny");
  // ubidots.addContext("key-2", "cold");
  // char* context = ubidots.getContext();
  
  // @ToDo add support for device types
  // @ToDo add support to subscribe to Particle handlers

  // float value = 30;
  // ubidots.addContext("key-1", "1");
  // ubidots.addContext("key-2", "2");
  // ubidots.addContext("key-3", "3");
  // ubidots.addContext("key-4", "4");
  // ubidots.addContext("key-5", "5");

  // char* context_result = (char *) malloc(sizeof(char) * 100);
  // ubidots.getContext(context_result);

  // Serial.println(context_result);

  // ubidots.add("variable-1", 1, context_result);
  //ubidots.add("variable-2", 1, context, time.Time(), 123);

  // bool result = ubidots.send("device-type-particle/?type=hola");

  // free(context_result);

  //bool result = ubidots.send("xs", "name", PUBLIC);

  float value = ubidots.get("ubiControl", "");
  Serial.println(value);
  //Serial.println(result);
  delay(5000);
}
