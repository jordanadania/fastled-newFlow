void fadeWhites(uint8_t value){
  for(int i=1; i<NUM_LEDS-1; ++i)                                 
    if((leds[i].r==leds[i].g && leds[i].g==leds[i].b           // if LED is white AND
      &&(max(max(leds[i].r, leds[i].g), leds[i].b) > 32))               // is at least 32 brightness
      &&((leds[i].r > max(max(leds[i-1].r, leds[i-1].g), leds[i-1].b)  // AND LED is brighter than left neighbor OR
      ||(leds[i-1].r== leds[i-1].g && leds[i-1].g== leds[i-1].b))
      &&(leds[i].r > max(max(leds[i+1].r, leds[i+1].g), leds[i+1].b)
      ||(leds[i+1].r== leds[i+1].g && leds[i+1].g== leds[i+1].b)))
    || leds[i].r > 96)
       leds[i].fadeToBlackBy(value);
}
void freshWhites(){
  for(int i=0; i<NUM_LEDS; ++i)
    if(leds[i].r==leds[i].g && leds[i].g==leds[i].b)
      leds[i] = CRGB::White;
}
void goGray(){
  for(uint16_t i=0; i<NUM_LEDS; ++i)
    if(leds[i]){
      newHue = max(max(leds[i].r, leds[i].g), leds[i].b);
      leds[i] = CRGB(newHue,newHue,newHue);
    }
}
void push(uint16_t k){
    for(uint16_t i=k; i>0; --i)
      leds[i] = leds[i-1];
}
void pull(uint16_t k){
    for(uint16_t i=0; i<k; ++i)
      leds[i] = leds[i+1];
}
void mirror(uint16_t k){
    for(int i=0; i<k; ++i)
      leds[k-i]=leds[i];
}
void copyIt(byte k){
  memcpy(leds+NUM_LEDS/k, leds,
    modus[k]<1?
      NUM_LEDS/k*sizeof(CRGB):
      NUM_LEDS/k*sizeof(CRGB)+sizeof(CRGB));
}

uint8_t getAverage255(uint8_t in[]){
  uint16_t out = 0;
  for(int i=0; i<SAMPLES; ++i)
    out+=in[i];
  return out/SAMPLES;
}
uint8_t getMax255(uint8_t in[]){
  uint8_t out = 0;
  for(int i=0; i<SAMPLES; ++i)
    out=out<in[i]?in[i]:out;
  return out;
}
uint8_t getMaxBass(uint8_t in[]){
  uint8_t out = 0, mx = in[out];
  for(int i=out+1; i<=6; ++i)
    if(mx<in[i]){
      mx = in[i];
      out = i;
    }
  return out;
}
uint8_t getMaxTreb(uint8_t in[]){
  uint8_t out = 6, mx = in[out];
  for(int i=out-1; i>=0; --i)
    if(mx<in[i]){
      mx = in[i];
      out = i;
    }
  return out;
}
uint16_t idx;
uint8_t bri[NUM_BANDS], dfm[NUM_BANDS], dfa[NUM_BANDS],
        smp[NUM_BANDS][SAMPLES],dif[NUM_BANDS][SAMPLES], 
        bass[NUM_BANDS], treb[NUM_BANDS];
void saveBands(uint16_t in){
  for(byte b=0; b<7; ++b)
    //smp[b][idx] = pow(spectrumByte[b]>>4,2);
    smp[b][idx] = spectrumByte[b];
  for(byte b=0; b<NUM_BANDS; ++b)
    dif[b][in] = abs(smp[b][in+1]-smp[b][in]);
}
void analyzeBands(){
  
  bool bb = true;
  bool bt = true;
  byte newBass = 0;
  byte newTreb = 6;
  
  // Get Average
  for(byte b=0; b<NUM_BANDS; ++b)
    bri[b] = getAverage255(smp[b]);
  // Get Average Difference
  for(byte b=0; b<NUM_BANDS; ++b)
    dfa[b] = getAverage255(dif[b]);
  // Get Biggest Difference
  for(byte b=0; b<NUM_BANDS; ++b)
    dfm[b] = getMax255(dif[b]);
    
  // Bass Analysis
  for(byte b=0; b<NUM_BANDS; ++b)
    bass[b] = bri[b]>dfm[b]? 0: dfm[b]-bri[b];
  // Treb Analysis
  for(byte b=0; b<NUM_BANDS; ++b)
    treb[b] = dfa[b]>>2+bri[b]>>3+dfm[b]>>1;
  
  // Weighted Current Band
  //bass[bassBand] *= 9>>3;
  //treb[trebBand] *= 9>>3;
  
  // Weighted Default Band
  bass[0] *= 9>>3;
  treb[6] *= 9>>3;
  
  // Band Assignment
  newTreb = getMaxTreb(treb);
  newBass = getMaxBass(bass);
  
  // 2 Strikes Against The Treb Band
 /* if(bt&&(newTreb!=trebBand)) bt = false;
  else if(!bt&&newTreb!=trebBand){
    trebBand = newTreb;
    bt = true;
  }*/
  
  // 2 Strikes Against The Bass Band
  if(bb&&(newBass!=bassBand)) bb = false;
  else if(!bb&&newBass!=bassBand){
    bassBand = newBass;
    bb = true;
  }
}
void newFlow(){
  static uint16_t idx;
  static uint8_t rgbRate;
  uint8_t bri;
  EVERY_N_SECONDS(2){ if(spectrumAvg>32) analyzeBands(); else bassBand=0,trebBand=6; }
  fadeWhites(16);
    bool zero = speed==0? true: false;
  uint16_t  k = edges[speed];
  uint16_t hk = k>>1;
  uint16_t ok = inv? (zero?k:hk): 0;
  saveBands(idx);
  bri = smp[trebBand][idx];
  idx = idx==SAMPLES-1? 0: idx+1;
  if(rgbRate>3) rgbRate-=rgbRate>>2;
  if(rgbRate>0)        --rgbRate;
  push(zero? k: hk);
  if(beatDetect()){
    freshWhites();
    rgbRate+=16;
    huey+=rgbRate;
    leds[ok] = CRGB::White;
  } else{ // beatDetect()
    if(trebDetect()){ rgbRate+=8; }
    huey+=rgbRate;
    leds[ok] = ColorFromPalette(RainbowColors_p, huey, bri, LINEARBLEND);
  } // beatDetect()
  if(bri==0) leds[ok] = CRGB(0,1,0);
  if(!zero){ mirror(k); copyIt(speed*NUM_STRIPS); }
}
