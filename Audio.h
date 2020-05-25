// Portions of this file are adapted from the work of Stefan Petrick:
// https://plus.google.com/u/0/115124694226931502095

// Portions of this file are adapted from RGB Shades Audio Demo Code by Garrett Mace:
// https://github.com/macetech/RGBShadesAudio

#define qsubd(x, b)  ((x>b)?b:0)
#define qsuba(x, b)  ((x>b)?x-b:0)

#define AUDIO_PIN  34 // A0 // 34
#define RESET_PIN  33 // D4 // 33
#define STROBE_PIN 26 // D3 // 26

#define AUDIODELAY 0

#define SPECTRUMSMOOTH 0.1
#define PEAKDECAY 0.25
#define NOISEFLOOR 65

#define AGCSMOOTH 0.004
#define GAINUPPERLIMIT 20.0
#define GAINLOWERLIMIT 0.1

#define SAMPLES 420

unsigned int spectrumValue[7];
float spectrumDecay[7] = {0};
float spectrumPeaks[7] = {0};
float audioAvg = 300.0;
float gainAGC = 1.0;

uint8_t spectrumByte[7];
uint8_t spectrumAvg;

unsigned long currentMillis;
unsigned long audioMillis;

void initializeAudio() {
  pinMode(AUDIO_PIN, INPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(STROBE_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  digitalWrite(STROBE_PIN, HIGH);
}

void readAudio() {
  static PROGMEM const byte spectrumFactors[7] = {6, 8, 8, 8, 7, 7, 10};
  digitalWrite(RESET_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(RESET_PIN, LOW);
  delayMicroseconds(10);
  int analogsum = 0;
  for (int i = 0; i < 7; i++) {
    digitalWrite(STROBE_PIN, LOW);
    delayMicroseconds(25);
    //spectrumValue[i] = (analogRead(AUDIO_PIN)+analogRead(AUDIO_PIN))/2;
    spectrumValue[i] = analogRead(AUDIO_PIN);
    digitalWrite(STROBE_PIN, HIGH);
    delayMicroseconds(30);
    if (spectrumValue[i] < NOISEFLOOR) {
      spectrumValue[i] = 0;
    } else {
      spectrumValue[i] -= NOISEFLOOR;
    }
    spectrumValue[i] = (spectrumValue[i] * pgm_read_byte_near(spectrumFactors + i)) / 10;
    analogsum += spectrumValue[i];
    spectrumValue[i] *= gainAGC;
    spectrumDecay[i] = (1.0 - SPECTRUMSMOOTH) * spectrumDecay[i] + SPECTRUMSMOOTH * spectrumValue[i];
    if (spectrumPeaks[i] < spectrumDecay[i]) spectrumPeaks[i] = spectrumDecay[i];
    spectrumPeaks[i] = spectrumPeaks[i] * (1.0 - PEAKDECAY);
    spectrumByte[i] = spectrumValue[i]>>2>255?255:spectrumValue[i]>>2;
  }
  audioAvg = (1.0 - AGCSMOOTH) * audioAvg + AGCSMOOTH * (analogsum / 7.0);
  spectrumAvg = (analogsum / 7.0) / 4;
  gainAGC = 300.0 / audioAvg;
  if (gainAGC > GAINUPPERLIMIT) gainAGC = GAINUPPERLIMIT;
  if (gainAGC < GAINLOWERLIMIT) gainAGC = GAINLOWERLIMIT;
}

// Attempt at beat detection
byte beatTriggered = 0;
#define beatLevel 64.0
#define beatDeadzone 32.0
#define beatDelay 0
float lastBeatVal = 0;
byte beatDetect() {
  static float beatAvg = 0;
  static unsigned long lastBeatMillis;
  uint8_t specCombo = spectrumByte[bassBand];
  //uint8_t specCombo = (spectrumByte[bassBand[0]] + spectrumByte[bassBand[1]]) / 2.0;
  //float specCombo = (spectrumDecay[bassBand[0] + spectrumDecay[bassBand[1]]) / 2.0;
  beatAvg = (1.0 - AGCSMOOTH) * beatAvg + AGCSMOOTH * specCombo;

  if (lastBeatVal < beatAvg) lastBeatVal = beatAvg;
  if ((specCombo - beatAvg) > beatLevel && beatTriggered == 0 && currentMillis - lastBeatMillis > beatDelay) {
    beatTriggered = 1;
    lastBeatVal = specCombo;
    lastBeatMillis = currentMillis;
      Serial.println("Bass Treble");
      Serial.print(bassBand);
      Serial.print(" ");
      Serial.println(trebBand);
    return 1;
  } else if ((lastBeatVal - specCombo) > beatDeadzone) {
    beatTriggered = 0;
    return 0;
  } else {
    return 0;
  }
}

byte trebTriggered = 0;
#define trebLevel 64.0
#define trebDeadzone 32.0
#define trebDelay 0
float lastTrebVal = 0;
byte trebDetect() {
  static float trebAvg = 0;
  static unsigned long lastTrebMillis;
  uint8_t specCombo = spectrumByte[trebBand] / 1.0;
  //float specCombo = spectrumDecay[trebBand] / 1.0;
  trebAvg = (1.0 - AGCSMOOTH) * trebAvg + AGCSMOOTH * specCombo;

  if (lastTrebVal < trebAvg) lastTrebVal = trebAvg;
  if ((specCombo - trebAvg) > trebLevel && trebTriggered == 0 && currentMillis - lastTrebMillis > trebDelay) {
    trebTriggered = 1;
    lastTrebVal = specCombo;
    lastTrebMillis = currentMillis;
      Serial.print(bassBand);
      Serial.print(" ");
      Serial.println(trebBand);
    return 1;
  } else if ((lastTrebVal - specCombo) > trebDeadzone) {
    trebTriggered = 0;
    return 0;
  } else {
    return 0;
  }
}
