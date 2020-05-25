#include "FastLED.h"

#define LED_TYPE  WS2812
#define DATA_PIN      13 // 6 // 13
#define COLOR_ORDER  GRB

#define NUM_LED      144  
#define NUM_STRIPS     1 
#define NUM_LEDS (NUM_LED * NUM_STRIPS)

#define MILLI_AMPS   700
#define BRIGHTNESS   255

uint16_t edges[26];   // Array used to mirror and divide
uint8_t  modus[26];   // Array that stores division remainders
uint8_t  gHue,        // Global Hue Index
       newHue,
      speed=1;

bool
  pmd = true,
  dir = true,
  inv = true,
  bmd = true,
  fmd = true,
  mor = false;

CRGB leds[NUM_LEDS];  // Main Array

byte trebConf;
byte bassConf;
byte bassBand = 0;
byte trebBand = 6;

#include "Audio.h"
#include "Pattern.h"

void setup() {
  //Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);
  Serial.println();
  initializeAudio();
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LED);
  LEDS.setDither(false);
  LEDS.setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);
  LEDS.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  memset(leds, 0, sizeof(leds));
  LEDS.show();
  for(byte b=0; b<=25; ++b)
    edges[b] = b==0? NUM_LEDS-1: NUM_LEDS/(b*NUM_STRIPS)-1;
  for(byte b=0; b<=25; ++b)
    modus[b] = b==0? 0: NUM_LEDS%(b*NUM_STRIPS);  
}

void loop() {
  currentMillis = millis();
  EVERY_N_MILLISECONDS(40){ ++gHue; }
  readAudio();
  newFlow();
  LEDS.show();
}
