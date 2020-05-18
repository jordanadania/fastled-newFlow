#include "FastLED.h"

#define LED_TYPE  WS2811
#define DATA_PIN       8
#define COLOR_ORDER  RGB

#define NUM_LED      300  
#define NUM_STRIPS     3 
#define NUM_LEDS (NUM_LED * NUM_STRIPS)

#define MILLI_AMPS 40000
#define BRIGHTNESS   255

uint16_t edges[26];   // Array used to mirror and divide
uint8_t  modus[26];   // Array that stores division remainders
uint8_t  gHue;        // Global Hue Index

CRGB leds[NUM_LEDS];  // Main Array
CRGB ledz[NUM_LEDS];  // Behind the scenes array

bool trebSwap = true;
bool bassSwap = true;

#include "Audio.h"
#include "Pattern.h"

void setup() {
  //Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);
  Serial.println();
  initializeAudio();
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LED*2, NUM_LED/2);
  LEDS.addLeds<WS2811_PORTA, NUM_STRIPS>(leds, NUM_LED);            // Used for parallel output
  LEDS.setDither(false);
  LEDS.setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);
  LEDS.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  memset(leds, 0, sizeof(leds));
  LEDS.show();
  for(byte b=0; b<=25; ++b)                                         // Load division array
    edges[b] = b==0? NUM_LEDS-1: NUM_LEDS/(b*NUM_STRIPS)-1;
  for(byte b=0; b<=25; ++b)                                         // Load remainder array
    modus[b] = b==0? 0: NUM_LEDS%(b*NUM_STRIPS);  

}

void loop() {
  readAudio();
  EVERY_N_MILLISECONDS(40){ ++gHue; }
  newFlow();
  for(uint16_t i=0; i<NUM_LEDS; ++i) // This copies/mirrors based on numstrips and index
    leds[NUM_STRIPS>1?i<NUM_LED?NUM_LED-i-1:i<NUM_LED*2?i:NUM_LED*3-i+NUM_LED*2-1:i] = ledz[i];
  LEDS.show();
}
