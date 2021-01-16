/*
LED VU meter for Arduino and Adafruit NeoPixel LEDs.
 
Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Adafruit Flora RGB Smart Pixels (ID: 1260)
   OR
 - Adafruit NeoPixel Digital LED strip (ID: 1138)
 - Optional: battery for portable use (else power through USB or adapter)
Software requirements:
 - Adafruit NeoPixel library
 
Connections:
 - 3.3V to mic amp +
 - GND to mic amp -
 - Analog pin to microphone output (configurable below)
 - Digital pin to LED data input (configurable below)
 See notes in setup() regarding 5V vs. 3.3V boards - there may be an
 extra connection to make and one line of code to enable or disable.
 
Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/
 
#include <FastLED.h>
#include "IRremote.h"

#define NUM_LEDS  326  // Number of pixels in strand
#define NUM_LEDS2 240
#define NUM_LEDS3 158
#define MIC_PIN   A1  // Microphone is attached to this analog pin
#define LED_PIN    9 // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0 // DC offset in mic signal - if unusure, leave 0
#define NOISE     0 // Noise/hum/interference in mic signal
#define SAMPLES   30  // Length of buffer for dynamic level adjustment

#define IR_PIN 3

 #define outputA 0
 #define outputB 2

  CRGB leds[NUM_LEDS]; 
  CRGB leds2[NUM_LEDS2];
  CRGB leds3[NUM_LEDS3];
//  IRrecv irrecv(IR_PIN);     // create instance of 'irrecv'
 // decode_results results;   
  
uint8_t gHue=0;



void setup() {
 
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
analogReference(EXTERNAL);

FastLED.addLeds<WS2812, 9, GRB>(leds, NUM_LEDS);
FastLED.addLeds<WS2812, 8, GRB>(leds2, NUM_LEDS2);
FastLED.addLeds<WS2812, 11, GRB>(leds3, NUM_LEDS3);
Serial.begin(9600);

//irrecv.enableIRIn();        
randomSeed(analogRead(A6));
EVERY_N_MILLISECONDS (40) {gHue+=30;}

}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].fadeToBlackBy(5); } }
void fadeall2() { for(int i = 0; i < NUM_LEDS2; i++) { leds2[i].fadeToBlackBy(5);}}
void fadeall3() { for(int i = 0; i < NUM_LEDS3; i++) { leds3[i].fadeToBlackBy(5);}}

void waterfall(){
  //WATERFALL FROM CENTER/////////////////////
int p = abs(analogRead(A1)-900);
int i;
if (p<600){
  leds[NUM_LEDS/2-15]=CHSV(gHue,255,50);
  leds2[NUM_LEDS2/2]=CHSV(gHue,255,50);
  leds3[NUM_LEDS3]=CHSV(gHue,255,50);
  gHue+=1;
}
else{
  gHue+=5;
  leds[NUM_LEDS/2-15] = CHSV(gHue+50,255,255);
  leds[NUM_LEDS/2+1-15] = leds[NUM_LEDS/2-15];
  leds[NUM_LEDS/2-1-15] = leds[NUM_LEDS/2-15];
  leds[NUM_LEDS/2+2-15] = leds[NUM_LEDS/2-15];
  leds[NUM_LEDS/2-2-15] = leds[NUM_LEDS/2-15];
  leds2[NUM_LEDS2/2] = CHSV(gHue+50,255,255);
  leds2[NUM_LEDS2/2+1] = leds2[NUM_LEDS2/2];
  leds2[NUM_LEDS2/2-1] = leds2[NUM_LEDS2/2];
  leds2[NUM_LEDS2/2+2] = leds2[NUM_LEDS2/2];
  leds2[NUM_LEDS2/2-2] = leds2[NUM_LEDS2/2];
  leds3[NUM_LEDS3]= CHSV(gHue+50,255,255);
}
   for (i = NUM_LEDS - 1; i > NUM_LEDS/2-15; i--)
    leds[i] = leds[i - 1];
   for (i = 0; i < NUM_LEDS/2-15 ; i++)
      leds[i] = leds[i+1];

   for (i = NUM_LEDS2 - 1; i > NUM_LEDS2/2; i--)
    leds2[i] = leds2[i - 1];
   for (i = 0; i < NUM_LEDS2/2 ; i++)
      leds2[i] = leds2[i+1];
   for(i=0;i<NUM_LEDS3;i++)
      leds3[i]=leds3[i+1];

FastLED.show();
        
        
   
/////////////////////////////////////////////
}


void meterSplit(){
//  int p = abs(analogRead(MIC_PIN) - 550);
//  Serial.println(p);

int p = abs(analogRead(MIC_PIN)-900);
for(int i = 0; i <NUM_LEDS/2;i++){
  if(i < p/4){
    leds[i-15]=CHSV(gHue+i,255,255);
    leds[NUM_LEDS/2+i-15] = CHSV(gHue+i, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds[i-15].fadeToBlackBy(20);
    leds[i+NUM_LEDS/2-15].fadeToBlackBy(20);
  }
}
for(int i = 0; i <NUM_LEDS2/2;i++){
  if(i < p/4){
    leds2[i]=CHSV(gHue+i,255,255);
    leds2[NUM_LEDS2/2+i] = CHSV(gHue+i, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds2[i].fadeToBlackBy(20);
    leds2[i+NUM_LEDS2/2].fadeToBlackBy(20);
  }
}
for(int i =0; i <NUM_LEDS3;i++){
  if(i < p/4){
    leds3[i]=CHSV(gHue+i,255,255);
  }
  else
    leds3[i].fadeToBlackBy(20);
}

if(p > 500)
  gHue+= 1;
  FastLED.show();
}

void solidFlash(){
//  int p = abs(analogRead(MIC_PIN) - 550);

int p = abs(analogRead(MIC_PIN)-900);
Serial.println(p);
int k;
if(p>500){
  delay(20);
  k = random(20,130);
}
else{
  k=0;
}
for(int i = 0; i <NUM_LEDS/2;i++){
  if(i < k){
    leds[i-15]=CHSV(gHue,255,255);
    leds[NUM_LEDS/2+i-15] = CHSV(gHue, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds[i-15].fadeToBlackBy(20);
    leds[i+NUM_LEDS/2-15].fadeToBlackBy(20);
  }
}
for(int i = 0; i <NUM_LEDS2/2;i++){
  if(i < k){
    leds2[i]=CHSV(gHue,255,255);
    leds2[NUM_LEDS2/2+i] = CHSV(gHue, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds2[i].fadeToBlackBy(20);
    leds2[i+NUM_LEDS2/2].fadeToBlackBy(20);
  }
}
for(int i =0; i <NUM_LEDS3;i++){
  if(i < k){
    leds3[i]=CHSV(gHue,255,255);
  }
  else
    leds3[i].fadeToBlackBy(20);
}

if(p > 500)
  gHue+= random(4);
  FastLED.show();
 
}

void meterSim(){
//  int p = abs(analogRead(MIC_PIN) - 550);

int p = abs(analogRead(MIC_PIN)-900);
Serial.println(p);
int k;
if(p>500){ 
  k = random(20,130);
}
else{
  k=0;
}
for(int i = 0; i <NUM_LEDS/2;i++){
  if(i < k){
    leds[i-15]=CHSV(gHue+i,255,255);
    leds[NUM_LEDS/2+i-15] = CHSV(gHue+i, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds[i-15].fadeToBlackBy(20);
    leds[i+NUM_LEDS/2-15].fadeToBlackBy(20);
  }
}
for(int i = 0; i <NUM_LEDS2/2;i++){
  if(i < k){
    leds2[i]=CHSV(gHue+i,255,255);
    leds2[NUM_LEDS2/2+i] = CHSV(gHue+i, 255,255);
    //NUM_LEDS-i is glitvhy
  }
  else {
    leds2[i].fadeToBlackBy(20);
    leds2[i+NUM_LEDS2/2].fadeToBlackBy(20);
  }
}
for(int i =0; i <NUM_LEDS3;i++){
  if(i < k){
    leds3[i]=CHSV(gHue+i,255,255);
  }
  else
    leds3[i].fadeToBlackBy(20);
}
  FastLED.show();
if(p > 500){
  gHue+= 1;
  delay(20);
}
  
 
}
int count;
int count2;
int count3;

void tick(){
  int p = abs(analogRead(MIC_PIN)-900);
  if(p > 200){
    count+=3;
    count2+=3;
    count3+=3;
    gHue+=3;
  }
  if(count > NUM_LEDS){
    count = 0;
    fadeall();
  }
  if(count2 > NUM_LEDS2){
    count2 = 0;
    fadeall2();
  }
  if(count3 >NUM_LEDS3){
    count3 = 0;
    fadeall3();
  }
  for(int i = 0; i < NUM_LEDS;i++){
   if( i < count){
    leds[i]=CHSV(gHue,255,255);
   }
   else leds[i].fadeToBlackBy(1);
  }
  for(int i = 0; i < NUM_LEDS2;i++){
   if( i < count2){
    leds2[i]=CHSV(gHue,255,255);
   }
   else leds2[i].fadeToBlackBy(1);
  }
  for(int i = 0; i <NUM_LEDS3;i++){
    if(i <count3){
      leds3[i]=CHSV(gHue,255,255);
    }
    else leds3[i].fadeToBlackBy(1);
  }
  FastLED.show();
  
}

int t = 0;

void fireworks(){
int p = abs(analogRead(MIC_PIN)-900);
  int f = random(NUM_LEDS-8);
  int c = random(NUM_LEDS2-8);
  int l = random(NUM_LEDS3-8);
  if(p>600){
    for(int i = 0; i < 8;i++){
      leds[f+i]=CHSV(gHue,255,255);
      leds2[c+i]=CHSV(gHue,255,255);
      leds3[l+i]=CHSV(gHue,255,255);
    }
  }
    for(int i = 0; i <NUM_LEDS;i++){
      leds[i].fadeToBlackBy(3);
    }
    for(int i = 0; i <NUM_LEDS2;i++){
      leds2[i].fadeToBlackBy(3);
    }
    for(int i =0; i <NUM_LEDS3;i++){
      leds3[i].fadeToBlackBy(3);
    }
    FastLED.show();
    gHue++;
}

int o=0;
void timedFade(){
  if(o==20){
    o=0;
    for(int j=0; j < 80;j++){
      for(int i = 0; i <NUM_LEDS-1 ; i++){
        leds[i] = CHSV(gHue,255,255);
      }
      for(int i = 0; i < NUM_LEDS2-1 ; i++){
        leds2[i] = CHSV(gHue,255,255);
      }
      for(int i = 0; i < NUM_LEDS3-1 ; i++){
        leds3[i] = CHSV(gHue,255,255);
      }
      gHue++;
      FastLED.show();
      delay(5);
    }
  }
  else{
  o++;
  delay(500);
  }
}


void loop() {
  uint8_t  i;
Serial.println(analogRead(MIC_PIN));
int previous;
//if(t < 2000) 
//timedFade(); 
if (t < 2000)
meterSim(); 
else if (t < 4000)
fireworks();
else if (t < 6000)
meterSplit();
else if (t < 8000)
tick();
else if (t <10000)
solidFlash();
else if (t < 12000)
 waterfall();
else t = 0;

t++; 
//timedFade();


}
