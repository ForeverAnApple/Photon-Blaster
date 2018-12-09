/**
 * SMD5050 12V RGB Strip Testing Code. SparkFunMMA8452Q accelerometer
 * testing code.
 * Author: Daiwei Chen
 * Thank you to Ian Buckley for the example of the RGB Strip.
 */

#include "SparkFunMMA8452Q.h"
#include <vector>
#include <cstring>
#include <cstdlib>

#define RED_LED A7
#define BLUE_LED A4
#define GREEN_LED A5
#define BUZZER TX

// Notes frequencies
#define D3 147
#define E3 165
#define F3 175
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define Ah4 440 // Can't use A4 cause it's claimed by Photon :(
#define B4f 466
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define Ah5 880 // Phton claimed A5 :(
#define B5 988
#define C6 1046

// custom structs, RGB values
struct RGB {
  int r;
  int g;
  int b;
};

// custom enum for gauntlet state
// These states will determine how the gauntlet will react.
enum GauntletState {
  starting,
  idle,
  receiving,
  shooting,
  inactive
};

enum GauntletEvent {

};
// Gauntlet event queues
std::vector<GauntletEvent> gEventQueue;

// MMA8452Q accelerometer
MMA8452Q accel;

// Gauntlet control
GauntletState gState = starting;

// Accelerometer information
double accelX, accelY, accelZ;

// LED information
struct RGB rgbStatus; // Current RGB Status
struct RGB fadeColor; // Color to fade to
int fadeSpeed = 10;
int brightness = 0; // Range: 0 - 100
bool fadeDown = false;

// LED Functionalities
void TurnOn(); // Test function
void TurnOff(); // Test function
void TurnOnRainbow(int, struct RGB);
void RandomColor(int);
void PrintRGBInfo(struct RGB);
void LightColor(struct RGB);
void FadeColor(struct RGB, int, int);
void Breathe();

// Buzzer and Music
void playTone(int, int);
void playStartingTone(struct RGB);

// Accelerometer functionalities
void readAccel();
void printAccelGraph(float, String, int, float);

// gState functionalities, updating gState as needed
void updateState();

// IoT components, HTTP get and post requests handling
int setFadeColor(String);

void setup() {
  // Setup pinmodes
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Particle variables
  Particle.variable("red", rgbStatus.r);
  Particle.variable("green", rgbStatus.g);
  Particle.variable("blue", rgbStatus.b);

  // Particle functions
  Particle.function("fadeColor", setFadeColor);

  Serial.begin(9600);
  // Initialize the accelerometer with begin():
  // begin can take two parameters: full-scale range, and output data rate (ODR).
  // Full-scale range can be: SCALE_2G, SCALE_4G, or SCALE_8G (2, 4, or 8g)
  // ODR can be: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12, ODR_6 or ODR_1
  accel.begin(SCALE_2G, ODR_1); // Set up accel with +/-2g range, and slowest (1Hz) ODR
}

void loop(){
  switch(gState){
    case starting:
      rgbStatus = {.r=0, .g=0, .b=255};
      playStartingTone(rgbStatus);
      break;
    case idle:
      //TurnOnRainbow(5, rgbStatus);
      Breathe();
      break;
    default:
      break;
  }

  updateState();

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

/* =========================== gState Controller ============================ */
/**
 * Updates gState depending on many control factors such as HTTP POST received,
 * IoT Component calls, queued up events, accelerometer, and other control schemes.
 */
void updateState(){
  readAccel();
  switch(gState){
    case starting:
      gState = idle;
      rgbStatus = {.r=255, .g=0, .b=0};
  }

  // Orientation is subject to change, please check your gauntlet
  // When held parallel: idle mode. Down: inaction. Up: IoT receiving mode.
  if(accelX <= -1.0){
    gState = inactive;
  } else if(accelx >= 1.0){
    gState = receiving;
  } else {
    gState = idle;
  }
}

/* ========================== IoT Functionalities =========================== */
/**
 * This is a particle function that changes the color being faded from HTTP POST
 * The format of the color should be "### ### ###", in RGB respectively.
 * Input:
 *  - [colorCode]: String of information that includes the rgb value of the color
 * Output:
 *  - Success code
 */
int setFadeColor(String colorCode){
  colorCode.trim(); //Trim, just in case of bad values.
  char colorString[100] = colorCode.c_str(); //Potential buffer overflow, but oh well.
  char* token = std::strtok(colorString, ' '); //strtok is pretty neat, check it out.
  fadeColor.r = std::atoi(token);
  token = std::strtok(NULL, ' '); //Increment the strtok token to the next value

  fadeColor.g = std::atoi(token);
  token = std::strtok(NULL, ' ');

  fadeColor.b = std::atoi(token);

  return 0;
}

/* ============================= RGB AND COLOR ============================== */

/**
 * Print the RGB info.
 */
void PrintRGBInfo(struct RGB color){
  Serial.printf("Red: %d, Green: %d, Blue: %d\n", color.r, color.g, color.b);
}

/**
 * LightColor lights up the LEDs with RGB
 * Input:
 *  - [color]: Full RGB value of the color to light.
 */
void LightColor(struct RGB color){
  analogWrite(RED_LED, color.r);
  analogWrite(BLUE_LED, color.b);
  analogWrite(GREEN_LED, color.g);
  PrintRGBInfo(color);
}

/**
 * Deep breaths.
 */
void Breathe(){
  if(fadeDown && brightness > 0){
    brightness--;
  else
    brightness++;

  int red = fadeColor.r * brightness / 100;
  int green = fadeColor.g * brightness / 100;
  int blue = fadeColor.b * brightness / 100;

  rgbStatus = {.r=red, .g=green, .b=blue};
  LightColor(rgbStatus);
  // fade flip
  if(brightness <= 0)
    fadeDown = false;
  else if(brightness >= 255)
    fadeDown = true;
    
  delay(fadeSpeed);
}

/**
 * This function fades a specified color in and out for a specified amount of time.
 * Input:
 *  - [color]: Full RGB value of the color to fade.
 *  - [fadeTime]: Time to fade into and out of the color (in ms).
 *  - [onTime]: Time to stay on.
 */
void FadeColor(struct RGB color, int fadeTime, int onTime){
  rgbStatus = {.r=0, .g=0, .b=0};
  // Calculate the fade in and out step to correctly scale to the color.
  // Value may be slightly off due to integer, but to the naked eye it is impossible
  // to notice the difference.
  int rStep = color.r / fadeTime;
  int gStep = color.g / fadeTime;
  int bStep = color.b / fadeTime;

  // Fade in first using cStep
  Serial.printf("Fading in...");
  for(int i = 0; i < fadeTime; ++i){
    rgbStatus.r += rStep;
    rgbStatus.g += gStep;
    rgbStatus.b += bStep;
    LightColor(rgbStatus);
    delay(1);
  }

  delay(onTime);

  // Fade out first using cStep
  Serial.printf("Fading out...");
  for(int i = 0; i < onTime; ++i){
    rgbStatus.r -= rStep;
    rgbStatus.g -= gStep;
    rgbStatus.b -= bStep;
    LightColor(rgbStatus);
    delay(1);
  }
}

/**
 * Show all colors avaliable!
 * Input:
 *  - [speed]: Time between color change
 */
void TurnOnRainbow(int speed, struct RGB color) {
  rgbStatus = color;

  LightColor(rgbStatus);
  if(rgbStatus.r == 0 && rgbStatus.b < 256){
    rgbStatus.g--;
    rgbStatus.b++;
  } else if(rgbStatus.g == 0 && rgbStatus.r < 256) {
    rgbStatus.b--;
    rgbStatus.r++;
  } else if(rgbStatus.b == 0 && rgbStatus.g < 256) {
    gbStatus.r--;
    rgbStatus.g++;
  } else {
    rgbStatus = {.r=255, .g=0, .b=0};
  }

  delay(speed);


  // for(int i = 0; i < 256; i++){
  //   LightColor(rgbStatus);
  //   rgbStatus.r--;
  //   rgbStatus.g++;
  //   delay(speed);
  // }
  // for(int i = 0; i < 256; i++){
  //   LightColor(rgbStatus);
  //   rgbStatus.g--;
  //   rgbStatus.b++;
  //   delay(speed);
  // }
  // for(int i = 0; i < 256; i++){
  //   LightColor(rgbStatus);
  //   rgbStatus.b--;
  //   rgbStatus.r++;
  //   delay(speed);
  // }
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

// Lights up a random color for a certain amount of time.
void RandomColor(int delayTime){
  rgbStatus.r = random(0, 256);
  rgbStatus.g = random(0, 256);
  rgbStatus.b = random(0, 256);
  LightColor(rgbStatus);
  delay(delayTime);
}

/* ============================= Accelerometer ============================== */

// Updates the accelerometer
void readAccel(){
  accelX = accel.cx;
  accelY = accel.cy;
  accelZ = accel.cz;
  Serial.printf("AccelX: %f, AccelY: %f, AccelZ: %f\n", accelX, accelY, accelZ);
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

/* ================================= MUSIC ================================== */
/**
 * Play a tone for a certain amount of time
 * Input:
 *  - [freq]: frequency of the buzzerPin
 *  - [dur]: Duration of the tone
 */
void playTone(int freq, int dur){
  tone(BUZZER, freq, dur);
  delay(dur);
}

/**
 * Duuuuuuuuuuuuuuu duuuu dudu du duuuuuu
 */
void playStartingTone(struct RGB color){
  rgbStatus = {.r=0, .g=0, .b=0};
  // Calculate the fade in to the right color
  int rStep = color.r / 130;
  int gStep = color.g / 130;
  int bStep = color.b / 130;

  for(int i = G4; i < G5; i+=3){
    rgbStatus.r += rStep;
    rgbStatus.g += gStep;
    rgbStatus.b += bStep;
    LightColor(rgbStatus);
    playTone(i, 10);
  }
  rgbStatus = color;
  LightColor(rgbStatus);
  playTone(G5, 500);
  rgbStatus = {.r=0, .g=0, .b=0};
  LightColor(rgbStatus);
  delay(100);

  rgbStatus = color;
  LightColor(rgbStatus);
  playTone(G5, 150);
  rgbStatus = {.r=0, .g=0, .b=0};
  LightColor(rgbStatus);
  delay(50);

  rgbStatus = color;
  LightColor(rgbStatus);
  playTone(G5, 150);
  rgbStatus = {.r=0, .g=0, .b=0};
  LightColor(rgbStatus);
  delay(50);

  rgbStatus = color;
  LightColor(rgbStatus);
  playTone(F5, 150);
  rgbStatus = {.r=0, .g=0, .b=0};
  LightColor(rgbStatus);
  delay(50);

  rgbStatus = color;
  LightColor(rgbStatus);
  playTone(G5, 500);
  rgbStatus = {.r=0, .g=0, .b=0};
  LightColor(rgbStatus);
  delay(50);
}
