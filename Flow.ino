#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#else
//  #define FASTLED_ESP32_I2S
  #include <ESPmDNS.h>
  #include <WiFi.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include "FastLED.h"
#define FASTLED_SHOW_CORE 1

#ifdef ESP8266
  #define DATA_PIN 8 
#else
  #define DATA_PIN 4
#endif

#define LED_TYPE  WS2812B
#define COLOR_ORDER  GRB

#define NUM_LED      300  
#define NUM_STRIPS     1 
#define NUM_LEDS (NUM_LED * NUM_STRIPS)
#define MILLI_AMPS 32000
#define BRIGHTNESS   255


const char* ssid = "This";
const char* password = "youngearth850";

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
CRGB grbleds[NUM_LEDS];

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
  // Parallel output: 13, 12, 27, 33, 15, 32, 14, SCL

  #ifdef ESP8266
    FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, 600, 150);
   // FastLED.addLeds<WS2811_PORTA, NUM_STRIPS>(leds, NUM_LED);
  #else
    FastLED.addLeds<WS2811, 33, RGB>(leds, NUM_LED/2);
    FastLED.addLeds<WS2812, 13, GRB>(grbleds, NUM_LED);
    FastLED.addLeds<WS2812, 12, GRB>(grbleds, NUM_LED);
    FastLED.addLeds<WS2812, 27, GRB>(grbleds, NUM_LED);
  #endif
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
  #include "OtA.h"
}

void loop() {
  ArduinoOTA.handle();
  currentMillis = millis();
  newFlow();
  memcpy(grbleds, leds, sizeof(leds));
  swapColors();
  LEDS.show();
  //LEDS.delay(10);
}
void swapColors(){ CRGB temp;
  for(uint16_t i=0; i<NUM_LEDS; ++i){
    temp = grbleds[i];
    grbleds[i].r = temp.g;
    grbleds[i].g = temp.r;
  }
}
