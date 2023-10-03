#define FASTLED_INTERNAL
#include <FastLED.h>

#define DATA_PIN      D4
#define LED_TYPE WS2812B
#define COLOR_ORDER  GRB
#define NUM_LEDS     100
#define MILLI_AMPS  1200
#define BRIGHTNESS   255


uint16_t edges[26];
uint8_t  modus[26],
              gHue,
            newHue,
           speed=0;

CRGB leds[NUM_LEDS];  // Main Array

byte trebConf,
     bassConf,
     bassBand = 0,
     trebBand = 6;

#include "Audio.h"
#include "Pattern.h"

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);
  Serial.println();
  initializeAudio();
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setDither(false);
  LEDS.setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);
  LEDS.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  memset(leds, 0, sizeof(leds));
  LEDS.show();
  for(byte b=0; b<=25; ++b)
    edges[b] = b==0? NUM_LEDS-1: NUM_LEDS/b-1;
  for(byte b=0; b<=25; ++b)
    modus[b] = b==0? 0: NUM_LEDS%b;
}

void loop() {  
  currentMillis = millis();
  readAudio();
  newFlow();
  FastLED.delay(1000 / 120);
}
