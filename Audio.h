// Portions of this file are adapted from the work of Stefan Petrick:
// https://plus.google.com/u/0/115124694226931502095

// Portions of this file are adapted from RGB Shades Audio Demo Code by Garrett Mace:
// https://github.com/macetech/RGBShadesAudio

#define qsubd(x, b)  ((x>b)?b:0)
#define qsuba(x, b)  ((x>b)?x-b:0)

#define MSGEQ7_AUDIO_PIN A0
#define MSGEQ7_RESET_PIN  D3
#define MSGEQ7_STROBE_PIN D4

#define AUDIODELAY 0

#define SPECTRUMSMOOTH 0.08
#define PEAKDECAY 0.01
#define NOISEFLOOR 70

#define AGCSMOOTH 0.003
#define GAINUPPERLIMIT 15.0
#define GAINLOWERLIMIT 0.1

unsigned int spectrumValue[7];
float spectrumDecay[7] = {0};
float spectrumPeaks[7] = {0};
float audioAvg = 270.0;
float gainAGC = 0.0;

uint8_t spectrumByte[7];

uint8_t spectrumAvg;

unsigned long currentMillis;
unsigned long audioMillis;

void initializeAudio() {
  pinMode(MSGEQ7_AUDIO_PIN, INPUT);
  pinMode(MSGEQ7_RESET_PIN, OUTPUT);
  pinMode(MSGEQ7_STROBE_PIN, OUTPUT);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
}

void readAudio() {
  static PROGMEM const byte spectrumFactors[7] = {9, 11, 13, 13, 12, 12, 13};
  digitalWrite(MSGEQ7_RESET_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  int analogsum = 0;
  for (int i = 0; i < 7; i++) {
    digitalWrite(MSGEQ7_STROBE_PIN, LOW);
    delayMicroseconds(50);
    spectrumValue[i] = analogRead(MSGEQ7_AUDIO_PIN);
    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
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
  gainAGC = 270.0 / audioAvg;
  if (gainAGC > GAINUPPERLIMIT) gainAGC = GAINUPPERLIMIT;
  if (gainAGC < GAINLOWERLIMIT) gainAGC = GAINLOWERLIMIT;
}

// Attempt at beat detection
byte beatTriggered = 0;
#define beatLevel 32.0
#define beatDeadzone 32.0
#define beatDelay 96
float lastBeatVal = 0;
byte beatDetect() {
  static float beatAvg = 0;
  static unsigned long lastBeatMillis;
  float specCombo = (spectrumDecay[0]+spectrumDecay[1]+spectrumDecay[2]) / 3.0;
  beatAvg = (1.0 - AGCSMOOTH) * beatAvg + AGCSMOOTH * specCombo;

  if (lastBeatVal < beatAvg) lastBeatVal = beatAvg;
  if ((specCombo - beatAvg) > beatLevel && beatTriggered == 0 && currentMillis - lastBeatMillis > beatDelay) {
    beatTriggered = 1;
    lastBeatVal = specCombo;
    lastBeatMillis = currentMillis;
    Serial.print("  bass hit ");
    bassSwap?Serial.println("0|1"):Serial.println("3|4");
    return 1;
  } else if ((lastBeatVal - specCombo) > beatDeadzone) {
    beatTriggered = 0;
    return 0;
  } else {
    return 0;
  }
}

byte trebTriggered = 0;
#define trebLevel 28.0
#define trebDeadzone 28.0
#define trebDelay 32
float lastTrebVal = 0;
byte trebDetect() {
  static float trebAvg = 0;
  static unsigned long lastTrebMillis;
  //uint8_t specCombo = spectrumByte[6] / 1.0;
  uint8_t specCombo = (spectrumByte[trebSwap?6:3]) / 1.0;
  trebAvg = (1.0 - AGCSMOOTH) * trebAvg + AGCSMOOTH * specCombo;

  if (lastTrebVal < trebAvg) lastTrebVal = trebAvg;
  if ((specCombo - trebAvg) > trebLevel && trebTriggered == 0 && currentMillis - lastTrebMillis > trebDelay) {
    trebTriggered = 1;
    lastTrebVal = specCombo;
    lastTrebMillis = currentMillis;
    Serial.print("treb hit ");
    trebSwap?Serial.println("6"):Serial.println("3");
    return 1;
  } else if ((lastTrebVal - specCombo) > trebDeadzone) {
    trebTriggered = 0;
    return 0;
  } else {
    return 0;
  }
}
