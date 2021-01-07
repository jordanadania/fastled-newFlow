// Uncomment to include Over the Air updates
//#define ENABLE_OTA

#ifdef ENABLE_OTA
  #ifdef ESP8266
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
  #else
    #include <ESPmDNS.h>
    #include <WiFi.h>
  #endif

  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>

  // Wifi Settings for Over The Air Updates
  const char* ssid = "YOURssidHERE";
  const char* password = "YOURpassHERE";
#endif

#ifdef ENABLE_OTA
  #define FASTLED_INTERRUPT_RETRY_COUNT 0
#endif
#include "FastLED.h"
//#define FASTLED_SHOW_CORE 1 // not used (yet)

#define DATA_PIN       4
#define LED_TYPE WS2812B
#define COLOR_ORDER  GRB
#define NUM_LEDS     300
#define MILLI_AMPS  1200
#define BRIGHTNESS   255


uint16_t edges[26];   // Array used to mirror and divide
uint8_t  modus[26];   // Array that stores division remainders
uint8_t  gHue,        // Global Hue Index
       newHue,
      speed=2;

bool
  pmd = true,
  dir = true,
  inv = false,
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
  #ifdef ESP8266
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);       // for WS2812 (Neopixel)
    // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  #else
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);       // for WS2812 (Neopixel)
    // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  #endif
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
  #ifdef ENABLE_OTA
    #include "OtA.h"
  #endif
}

void loop() {
  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif
  currentMillis = millis();
  newFlow();
  LEDS.show();
  #ifdef ENABLE_OTA
    yield();
  #endif
}
