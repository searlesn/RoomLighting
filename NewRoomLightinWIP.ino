#include <ClickEncoder.h>
#include <TimerOne.h>
#include <FastLED.h>

int16_t oldEncPos, encPos;
uint8_t buttonState;

#define pinA 10
#define pinB 8
#define pinSw 12 //switch
#define STEPS 4
#define NUM_LEDS 157
#define NUM_LEDS2  326
#define NUM_LEDS3 158
#define NUM_LEDS4 240
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120



int BRIGHTNESS = 185;

#define FRAMES_PER_SECOND  120


ClickEncoder encoder(pinA, pinB, pinSw, STEPS);
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];


typedef void (*SimplePatternList[])();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int col = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))




void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, BRIGHTNESS);
    fadeToBlackBy( leds2, NUM_LEDS2, 10);
  int pos2 = random16(NUM_LEDS2);
  leds2[pos2] += CHSV( gHue + random8(64), 255, BRIGHTNESS);
  fadeToBlackBy( leds3, NUM_LEDS3, 10);
  int pos3 = random16(NUM_LEDS3);
  leds3[pos3] += CHSV( gHue + random8(64), 255, BRIGHTNESS);
    fadeToBlackBy( leds4, NUM_LEDS4, 10);
    int pos4 = random16(NUM_LEDS4);
  leds4[pos4] += CHSV( gHue + random8(64), 255, BRIGHTNESS);
}

void solid(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV (col, 255, BRIGHTNESS);
  }
    for(int i = 0; i < NUM_LEDS2; i++){
    leds2[i] = CHSV (col, 255, BRIGHTNESS);
  }
    for(int i = 0; i < NUM_LEDS3; i++){
    leds3[i] = CHSV (col, 255, BRIGHTNESS);
  }
    for(int i = 0; i < NUM_LEDS4; i++){
    leds4[i] = CHSV (col, 255, BRIGHTNESS);
    }
  
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, BRIGHTNESS);
      fadeToBlackBy( leds2, NUM_LEDS2, 20);
  int pos2 = beatsin16( 13, 0, NUM_LEDS2-1 );
  leds2[pos2] += CHSV( gHue, 255, BRIGHTNESS);
      fadeToBlackBy( leds3, NUM_LEDS3, 20);
  int pos3 = beatsin16( 13, 0, NUM_LEDS3-1 );
  leds3[pos3] += CHSV( gHue, 255, BRIGHTNESS);
        fadeToBlackBy( leds4, NUM_LEDS4, 20);
  int pos4 = beatsin16( 13, 0, NUM_LEDS4-1 );
  leds4[pos4] += CHSV( gHue, 255, BRIGHTNESS);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
    fill_rainbow( leds2, NUM_LEDS2, gHue, 7);
  fill_rainbow( leds3, NUM_LEDS3, gHue, 7);
  fill_rainbow( leds4, NUM_LEDS4, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
        leds2[ random16(NUM_LEDS2) ] += CRGB::White;
    leds3[random16(NUM_LEDS3)] += CRGB::White;
    leds4[random16(NUM_LEDS4)] += CRGB::White;
  }
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
}
   for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  for( int i = 0; i < NUM_LEDS3; i++) { //9948
    leds3[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
    for( int i = 0; i < NUM_LEDS4; i++) { //9948
    leds4[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 255, BRIGHTNESS);
    dothue += 32;
  }
    fadeToBlackBy( leds2, NUM_LEDS2, 20);
  byte dothue2 = 0;
  for( int i = 0; i < 8; i++) {
    leds2[beatsin16( i+7, 0, NUM_LEDS2-1 )] |= CHSV(dothue2  , 255, BRIGHTNESS);
    dothue2 += 32;
  }
    fadeToBlackBy( leds3, NUM_LEDS3, 20);
  byte dothue3 = 0;
  for( int i = 0; i < 8; i++) {
    leds3[beatsin16( i+7, 0, NUM_LEDS3-1 )] |= CHSV(dothue3  , 255, BRIGHTNESS);
    dothue3 += 32;
  }
      fadeToBlackBy( leds4, NUM_LEDS4, 20);
  byte dothue4 = 0;
  for( int i = 0; i < 8; i++) {
    leds4[beatsin16( i+7, 0, NUM_LEDS4-1 )] |= CHSV(dothue4  , 255, BRIGHTNESS);
    dothue4 += 32;
  }
}

void turnOff(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
     for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i] = CRGB::Black;
  }
  for( int i = 0; i < NUM_LEDS3; i++) { //9948
    leds3[i] = CRGB::Black;
  }
    for( int i = 0; i < NUM_LEDS4; i++) { //9948
    leds4[i] = CRGB::Black;
  }
  FastLED.show();
}

SimplePatternList gPatterns = { solid, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void setup() {
  Serial.begin(9600);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  encoder.setAccelerationEnabled(true);

  Serial.print("Acceleration is ");
  Serial.println((encoder.getAccelerationEnabled()) ? "enabled" : "disabled");
  FastLED.addLeds<LED_TYPE,7,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,14,COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,3,COLOR_ORDER>(leds3,NUM_LEDS3).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,15,COLOR_ORDER>(leds4,NUM_LEDS4).setCorrection(TypicalLEDStrip);
  oldEncPos = -1;
  encoder.setAccelerationEnabled(0);
}

int offState=0;
int bright;

void loop() {

  
  encPos += encoder.getValue();
 

if(encPos != oldEncPos){
  if(bright == 1){ 
    BRIGHTNESS += (encPos - oldEncPos)*10; 
    if(BRIGHTNESS > 255)
      BRIGHTNESS = 0;
    if(BRIGHTNESS < 0)
      BRIGHTNESS = 255;
  }
  else{
    col += (encPos - oldEncPos)*4;
    if(col > 255)
      col = 0;
    if(col < 0);
      col = 255;
     
  }
  oldEncPos = encPos;
  Serial.print("Encoder Value: ");
  Serial.println(encPos);
}


//  if (encPos > oldEncPos) {
//    oldEncPos = encPos;
//    BRIGHTNESS+=15;
//        Serial.print("Encoder Value: ");
//    Serial.println(encPos);
//  }
//  else if (encPos < oldEncPos){
//    oldEncPos = encPos;
//    gHue+=6;
//        Serial.print("Encoder Value: ");
//    Serial.println(encPos);
//  }
  FastLED.setBrightness(BRIGHTNESS);
  buttonState = encoder.getButton();

//  if (buttonState != 0) {
//    Serial.print("Button: "); Serial.println(buttonState);
//    switch (buttonState) {
//      case ClickEncoder::Open:          //0
//        break;
//
//      case ClickEncoder::Closed:        //1
//        break;
//
//      case ClickEncoder::Pressed:       //2
//        break;
//
//      case ClickEncoder::Held:          //3
//        break;
//
//      case ClickEncoder::Released:      //4
//        break;
//
//      case ClickEncoder::Clicked:       //5
//        break;
//
//      case ClickEncoder::DoubleClicked: //6
//        break;
//    }
//  }

    // Call the current pattern function once, updating the 'leds' array
  if(offState == 0){
    gPatterns[gCurrentPatternNumber]();
  }
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if(buttonState == 5){
    nextPattern();
    offState = 0;
  }
  if(buttonState == 3||offState==1){
    turnOff();
    offState = 1;
  }
  if(buttonState==6){
    if(bright == 1){
      bright = 0;
    }
    else
      bright = 1;
  }
}

void timerIsr() {
  encoder.service();
}
