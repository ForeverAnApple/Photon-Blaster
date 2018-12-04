/**
 * SMD5050 12V RGB Strip Testing Code.
 * Author: Daiwei Chen
 * Thank you to Ian Buckley for giving out example.
 */
#include "Ubidots.h"
#define TOKEN "A1E-Af0NNQDxMo3LyFrolJHPs6mS2GHeuT

#define RED_LED D0
#define BLUE_LED D1
#define GREEN_LED D2

int brightness = 255;

int gBright = 0;
int rBright = 0;
int bBright = 0;

int fadeSpeed = 10;

Ubidots ubidots(TOKEN);

void TurnOn();
void TurnOff();

void setup() {
   pinMode(GREEN_LED, OUTPUT);
   pinMode(RED_LED, OUTPUT);
   pinMode(BLUE_LED, OUTPUT);

   Serial.begin(9600);
}

void loop(){
  Serial.printf("Fading in...\n");
  TurnOn();
  delay(5000);
  Serial.printf("Fading out...\n");
  TurnOff();
}

// Testing fade in
void TurnOn() {
   for (int i = 0; i < 256; i++) {
       analogWrite(RED_LED, rBright);
       rBright +=1;
       delay(fadeSpeed);
   }

   for (int i = 0; i < 256; i++) {
       analogWrite(BLUE_LED, bBright);
       bBright += 1;
       delay(fadeSpeed);
   }

   for (int i = 0; i < 256; i++) {
       analogWrite(GREEN_LED, gBright);
       gBright +=1;
       delay(fadeSpeed);
   }
}

// Testing fade out
void TurnOff() {
   for (int i = 0; i < 256; i++) {
       analogWrite(GREEN_LED, brightness);
       analogWrite(RED_LED, brightness);
       analogWrite(BLUE_LED, brightness);

       brightness -= 1;
       delay(fadeSpeed);
   }
}
