#include "Ubidots.h"

/*
 *  Cleans the buffer that stores values. It should be called if the method
 * isDirty() returns true and the device needs to store new values.
 */

void cleanValuesBufferUtils(typedef struct * val, uint8_t _current_value) {
  
  for (uint8_t i = 0; i < _current_value;) {
    (val + i)->variable_label = NULL;
    (val + i)->dot_value = NULL;
    (val + i)->dot_timestamp_seconds = NULL; 
    (val + i)->dot_timestamp_millis = NULL; 
    (val + i)->dot_context = NULL;
  }

  //_current_value = 0;
  //_dirty = false;

//   if (_debug) {
//     Serial.println("Buffer is now clear");
//   }

}
