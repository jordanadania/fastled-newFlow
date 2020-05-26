void fadeWhites(uint8_t value){
  for(int i=1; i<NUM_LEDS-1; ++i)                                 
    if((ledz[i].r == ledz[i].g && ledz[i].g == ledz[i].b)
    &&(max(max(ledz[i].r, ledz[i].g), ledz[i].b) > 32
    &&(ledz[i].r > max(max(ledz[i-1].r, ledz[i-1].g), ledz[i-1].b)
    ||(ledz[i-1].r== ledz[i-1].g && ledz[i-1].g== ledz[i-1].b))
    &&(ledz[i].r > max(max(ledz[i+1].r, ledz[i+1].g), ledz[i+1].b)
    ||(ledz[i+1].r== ledz[i+1].g && ledz[i+1].g== ledz[i+1].b))
    || ledz[i].r > 96))
       ledz[i].fadeToBlackBy(value);
}
void freshWhites(){
  for(int i=0; i<NUM_LEDS; ++i)
    if(ledz[i].r==ledz[i].g && ledz[i].g==ledz[i].b)
      ledz[i] = CRGB::White;
}
void goGray(){
  for(uint16_t i=0; i<NUM_LEDS; ++i)
    if(ledz[i]){
      newHue = max(max(ledz[i].r, ledz[i].g), ledz[i].b);
      ledz[i] = CRGB(newHue,newHue,newHue);
    }
}
void push(uint16_t k){
    for(uint16_t i=k; i>0; --i)
      ledz[i] = ledz[i-1];
}
void pull(uint16_t k){
    for(uint16_t i=0; i<k; ++i)
      ledz[i] = ledz[i+1];
}
void mirror(uint16_t k){
    for(int i=0; i<k; ++i)
      ledz[k-i]=ledz[i];
}
void copyIt(byte k){
  memcpy(ledz+NUM_LEDS/k, ledz,
    modus[k]<1?(NUM_LEDS*sizeof(CRGB)*(k-1)/k):
 (NUM_LEDS*sizeof(CRGB)*(k-1)/k)+sizeof(CRGB));
}
uint8_t getAverage32(uint8_t in[]){
  uint16_t out = 0;
  for(int i=0; i<32; ++i)
    out+=in[i];
  return out/32;
}
uint8_t getAverage255(uint8_t in[]){
  uint16_t out = 0;
  for(int i=0; i<SAMPLES; ++i)
    out+=in[i];
  return out/320;
}
uint8_t getMax255(uint8_t in[]){
  uint8_t out = 0;
  for(int i=0; i<SAMPLES; ++i)
    out=out<in[i]?in[i]:out;
  return out;
}
uint8_t getMaxBass(uint8_t in[]){
  uint8_t out = 0, mx = 0;
  for(int i=6; i>0; --i)
    if(mx<in[i]){
      mx = in[i];
      out = i;
    }
  return out;
}
uint8_t getMaxTreb(uint8_t in[]){
  uint8_t out = 0, mx = 0;
  if(bassBand==0)
    mx = 1;
  for(int i=6; i>0; --i)
    if(mx<in[i]&&i!=bassBand){
      mx = in[i];
      out = i;
    }
  return out;
}
uint16_t idx;
uint8_t bri[7], dfm[7], dfa[7],
        smp[7][SAMPLES],dif[7][SAMPLES], 
        bass[7], treb[7];
void saveBands(){
  for(byte b=0; b<7; ++b)
    smp[b][idx] = pow(spectrumByte[b]>>4,2);
  for(byte b=0; b<7; ++b)
    dif[b][idx] = abs(smp[b][idx+1]-smp[b][idx]);
}
void analyzeBands(){
  for(byte b=0; b<7; ++b)
    bri[b] = getAverage255(smp[b]);
  for(byte b=0; b<7; ++b)
    dfa[b] = getAverage255(dif[b]);
  for(byte b=0; b<7; ++b)
    dfm[b] = getMax255(dif[b]);
  for(byte b=0; b<7; ++b){
    bass[b] = bri[b]>dfm[b]? 0: dfm[b]-bri[b];
    if(b<3&&bassConf!=b)
      bass[b] *= 5>>2;
  }
  bass[bassConf] *= 5>>2;
  // Treb Analysis
  for(byte b=0; b<7; ++b)
    treb[b] = (dfm[b]<<2/3+bri[b]>>1)>>2;
  treb[trebConf] *= 9>>3;
  if(trebConf!=6)
    treb[6] *= 9>>3;

  trebConf = trebBand = getMaxTreb(treb);
  bassConf = bassBand = getMaxBass(bass);
}

void newFlow(){  
    EVERY_N_SECONDS(2){ if(spectrumAvg>48) analyzeBands(); else bassConf=bassBand=0,trebConf=trebBand=6; }
    static uint8_t rgbRate, huey;
    uint8_t bri, rate;
    bool bb = true, bt = true;
    
    fadeWhites(16);
      bool zero = speed==0? true: false;
    uint16_t  k = edges[speed];
    uint16_t hk = k>>1;
    uint16_t ok = inv? (zero?k:hk): 0;
    
    saveBands();
    bri = smp[trebBand][idx];
    idx = idx==SAMPLES-1? 0: idx+1;
    
    rate = (getAverage32(smp[trebBand])>>5);
    if(rgbRate>rate+1) rgbRate-=rate;
    
    if(rgbRate>3)      rgbRate-=rgbRate/4;
    if(rgbRate>0)      rgbRate--;
    
    inv? pull(zero? k: hk): push(zero? k: hk);
    
    if(beatDetect()){
      freshWhites();
      ledz[ok] = CRGB::White;
      if(rate>0) ledz[ok==0? ok+1: ok-1] = CRGB::White;              // IF 12 of 32 brightness thresholds were
    } else{ // beatDetect()
      if(trebDetect()){ rgbRate+=9; }                            // "trebDetect" increases rgbRate by the most
      dir? huey+=rgbRate: huey-=rgbRate;    // webapp btn-Hue Speed decided adjustment
      ledz[ok] = ColorFromPalette(RainbowColors_p, huey, bri, LINEARBLEND);   // webapp btn-Palette Mode
    } // beatDetect()
    if(bri==0) ledz[ok] = CRGB(0,1,0);                        // Never let a pixel be black
    if(!zero){ mirror(k); copyIt(speed*NUM_STRIPS); }                  // Mirror from Index // copy mirrored section speed*3-1 times
  //}
}
