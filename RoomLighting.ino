#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    9
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    326
#define NUM_LEDS2 240
#define NUM_LEDS3 158
#define NUM_LEDS4 157
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];


#define BRIGHTNESS          200
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,8,COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,11,COLOR_ORDER>(leds3,NUM_LEDS3).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,4,COLOR_ORDER>(leds4,NUM_LEDS4).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
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

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  fadeToBlackBy( leds2, NUM_LEDS2, 10);
  int pos2 = random16(NUM_LEDS2);
  leds2[pos2] += CHSV( gHue + random8(64), 200, 255);
  fadeToBlackBy( leds3, NUM_LEDS3, 10);
  int pos3 = random16(NUM_LEDS3);
  leds3[pos3] += CHSV( gHue + random8(64), 200, 255);
    fadeToBlackBy( leds4, NUM_LEDS4, 10);
    int pos4 = random16(NUM_LEDS4);
  leds4[pos4] += CHSV( gHue + random8(64), 200, 255);
  
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
    fadeToBlackBy( leds2, NUM_LEDS2, 20);
  int pos2 = beatsin16( 13, 0, NUM_LEDS2-1 );
  leds2[pos2] += CHSV( gHue, 255, 192);
      fadeToBlackBy( leds3, NUM_LEDS3, 20);
  int pos3 = beatsin16( 13, 0, NUM_LEDS3-1 );
  leds3[pos3] += CHSV( gHue, 255, 192);
        fadeToBlackBy( leds4, NUM_LEDS4, 20);
  int pos4 = beatsin16( 13, 0, NUM_LEDS4-1 );
  leds4[pos4] += CHSV( gHue, 255, 192);
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
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  fadeToBlackBy( leds2, NUM_LEDS2, 20);
  byte dothue2 = 0;
  for( int i = 0; i < 8; i++) {
    leds2[beatsin16( i+7, 0, NUM_LEDS2-1 )] |= CHSV(dothue2  , 200, 255);
    dothue2 += 32;
  }
    fadeToBlackBy( leds3, NUM_LEDS3, 20);
  byte dothue3 = 0;
  for( int i = 0; i < 8; i++) {
    leds3[beatsin16( i+7, 0, NUM_LEDS3-1 )] |= CHSV(dothue3  , 200, 255);
    dothue3 += 32;
  }
      fadeToBlackBy( leds4, NUM_LEDS4, 20);
  byte dothue4 = 0;
  for( int i = 0; i < 8; i++) {
    leds4[beatsin16( i+7, 0, NUM_LEDS4-1 )] |= CHSV(dothue4  , 200, 255);
    dothue4 += 32;
  }
}
