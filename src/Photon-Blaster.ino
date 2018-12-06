/**
 * SMD5050 12V RGB Strip Testing Code. SparkFunMMA8452Q accelerometer
 * testing code.
 * Author: Daiwei Chen
 * Thank you to Ian Buckley for giving out example for the RGB Strip.
 */

#include "SparkFunMMA8452Q.h"

#define RED_LED A7
#define BLUE_LED A4
#define GREEN_LED A5

// MMA8452Q accelerometer
MMA8452Q accel;

int fadeSpeed = 100;

void TurnOn();
void TurnOff();
void TurnOnRainbow();
void printAccelGraph(float, String, int, float);

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  Serial.begin(9600);
  // Initialize the accelerometer with begin():
  // begin can take two parameters: full-scale range, and output data rate (ODR).
  // Full-scale range can be: SCALE_2G, SCALE_4G, or SCALE_8G (2, 4, or 8g)
  // ODR can be: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12, ODR_6 or ODR_1
  accel.begin(SCALE_2G, ODR_1); // Set up accel with +/-2g range, and slowest (1Hz) ODR
}

void loop(){
  // Serial.printf("Fading in...\n");
  // TurnOn();
  // delay(3000);
  // Serial.printf("Fading out...\n");
  // TurnOff();
  // TurnOnRainbow();
  // delay(3000);
  // TurnOff();
  int red = random(0, 256);
  int blue = random(0, 256);
  int green = random(0, 256);
  analogWrite(RED_LED, red);
  analogWrite(BLUE_LED, blue);
  analogWrite(GREEN_LED, green);
  delay(fadeSpeed);

  // accel.available() will return 1 if new data is available, 0 otherwise
  if (accel.available())
  {
    // To update acceleration values from the accelerometer, call accel.read();
    accel.read();

    // After reading, six class variables are updated: x, y, z, cx, cy, and cz.
    // Those are the raw, 12-bit values (x, y, and z) and the calculated
    // acceleration's in units of g (cx, cy, and cz).

    // use the printAccelGraph funciton to print the values along with a bar
    // graph, to see their relation to eachother:
    printAccelGraph(accel.cx, "X", 20, 2.0);
    printAccelGraph(accel.cy, "Y", 20, 2.0);
    printAccelGraph(accel.cz, "Z", 20, 2.0);
    Serial.println();
  }
}

void TurnOnRainbow() {
  for(int i = 0; i < 256; i++){
    for(int j = 0; j < 256; i++){
      for(int k = 0; k < 256; i++){
        analogWrite(RED_LED, i);
        analogWrite(BLUE_LED, j);
        analogWrite(GREEN_LED, k);
        delay(fadeSpeed);
      }
    }
  }
}

// Testing fade in
void TurnOn() {
  Serial.printf("Red...\n");
  for (int i = 0; i < 256; i++) {
    analogWrite(RED_LED, i);
    delay(fadeSpeed);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(RED_LED, i);
    delay(fadeSpeed);
  }

  // ========================

  Serial.printf("Blue...\n");
  for (int i = 0; i < 256; i++) {
    analogWrite(BLUE_LED, i);
    delay(fadeSpeed);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(BLUE_LED, i);
    delay(fadeSpeed);
  }

  // ========================

  Serial.printf("Green...\n");
  for (int i = 0; i < 256; i++) {
    analogWrite(GREEN_LED, i);
    delay(fadeSpeed);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(GREEN_LED, i);
    delay(fadeSpeed);
  }
}

// Testing fade out
void TurnOff() {
  for (int i = 255; i >= 0; i--) {
    analogWrite(GREEN_LED, i);
    analogWrite(RED_LED, i);
    analogWrite(BLUE_LED, i);

    delay(fadeSpeed);
  }
}


// printAccelGraph prints a simple ASCII bar graph for a single accelerometer axis value.
// Examples:
//	printAccelGraph(-0.1, "X", 20, 2.0) will print:
// 		X:                    =|                     (0.1 g)
//	printAccelGraph(1.0, "Z", 20, 2.0) will print:
//		Z:                     |==========           (1.0 g)
// Input:
//	- [value]: calculated value of an accelerometer axis (e.g accel.cx, accel.cy)
//	- [name]: name of the axis (e.g. "X", "Y", "Z")
//	- [numBarsFull]: Maximum number of bars either right or left of 0 point.
//	- [rangeAbs]: Absolute value of the maximum acceleration range
void printAccelGraph(float value, String name, int numBarsFull, float rangeAbs)
{
  // Calculate the number of bars to fill, ignoring the sign of numBars for now.
  int numBars = abs(value / (rangeAbs / numBarsFull));

  Serial.print(name + ": "); // Print the axis name and a colon:

  // Do the negative half of the graph first:
  for (int i=0; i<numBarsFull; i++)
  {
    if (value < 0) // If the value is negative
    {
      // If our position in the graph is in the range we want to graph
      if (i >= (numBarsFull - numBars))
        Serial.print('='); // Print an '='
      else
        Serial.print(' '); // print spaces otherwise
    }
    else // If our value is positive, just print spaces
      Serial.print(' ');
  }

  Serial.print('|'); // Print a pipe (|) to represent the 0-point

  // Do the positive half of the graph last:
  for (int i=0; i<numBarsFull; i++)
  {
    if (value > 0)
    {	// If our position in the graph is in the range we want to graph
      if (i <= numBars)
        Serial.print('='); // Print an '='
      else
        Serial.print(' '); // otherwise print spaces
    }
    else // If value is negative, just print spaces
      Serial.print(' ');
  }

  // To end the line, print the actual value:
  Serial.println(" (" + String(value, 2) + " g)");
}
