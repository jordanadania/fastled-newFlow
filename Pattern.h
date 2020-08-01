void fadeWhites(uint8_t value){
  for(int i=1; i<NUM_LEDS-1; ++i)                                 
    if(
        (ledz[i].r==ledz[i].g && ledz[i].g==ledz[i].b           // if LED is white AND
        &&(max(max(ledz[i].r, ledz[i].g), ledz[i].b) > 32))               // is at least 32 brightness
    
      &&((ledz[i].r > max(max(ledz[i-1].r, ledz[i-1].g), ledz[i-1].b)  // AND LED is brighter than left neighbor OR
        ||(ledz[i-1].r== ledz[i-1].g && ledz[i-1].g== ledz[i-1].b))     // left neighbor is WHITE
        &&(ledz[i].r > max(max(ledz[i+1].r, ledz[i+1].g), ledz[i+1].b)  // AND led is brighter than right neighbor OR
          ||(ledz[i+1].r== ledz[i+1].g && ledz[i+1].g== ledz[i+1].b)))     // 
    
    || ledz[i].r > 96)
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
  memcpy((s=="New Flow"?ledz:leds)+NUM_LEDS/k,
         (s=="New Flow"?ledz:leds),NUM_LEDS*sizeof(CRGB)*(k-1)/k);
}
void goUp()  { mrs = !mrs;             morphs+=13; }
void goDown(){ mrs = !mrs; morphs!=25? morphs-=12:morphs=0; }
void morph() { mrs? goUp():goDown(); }

uint8_t getAverage32(uint8_t in[]){
  uint16_t out = 0;
  for(int i=0; i<32; ++i)
    out+=in[i];
  return out>>5;
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
uint8_t bri[NUM_BANDS], dfm[NUM_BANDS], dfa[NUM_BANDS],
        smp[NUM_BANDS][SAMPLES],dif[NUM_BANDS][SAMPLES], 
        bass[NUM_BANDS], treb[NUM_BANDS];
void saveBands(uint16_t in){
  for(byte b=0; b<NUM_BANDS; ++b)
    smp[b][in] = pow(spectrumByte[b]>>4,2);
  for(byte b=0; b<NUM_BANDS; ++b)
    dif[b][in] = abs(smp[b][in+1]-smp[b][in]);
}
void analyzeBands(){
  
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
  for(byte b=0; b<7; ++b){
    bass[b] = bri[b]>dfm[b]? 0: dfm[b]-bri[b];
    //if(b<3&&bassBand!=b)
    //  bass[b] *= 9>>3;
  }
  bass[bassBand] *= 9>>3;
  
  // Treb Analysis
  for(byte b=0; b<7; ++b){
    treb[b] = dfm[b];
   // if(b>2)
   //  treb[b] *= 9>>3;
  }
  // Band Assignment
  trebBand = getMaxTreb(treb);
  bassBand = getMaxBass(bass);
  
}
void newFlow(){ audio = true;
  static uint16_t idx;
  uint8_t bri, rate;
  bool bb = true, bt = true;
  EVERY_N_SECONDS(8){ if(mor) morph(); }
  //EVERY_N_SECONDS(2){ if(spectrumAvg>48) analyzeBands(); else bassBand=0,trebBand=6; }
  EVERY_N_SECONDS(2){ analyzeBands(); }
    fadeWhites(16);
   // if(tfd) fadeColors(32);
    if(mor) speed = morphs;
        bool zero = speed==0? true: false;
      uint16_t  k = edges[speed];
      uint16_t hk = k>>1;
      uint16_t ok = inv? (zero?k:hk): 0;
  saveBands(idx);
   bri = smp[trebBand][idx];
   idx = idx==SAMPLES-1? 0: idx+1;
  //rate = getAverage32(smp[trebBand])>>4;
  //if(   rate>1)        --rate;
  //              rgbRate+=rate;
  if(rgbRate>3) rgbRate-=rgbRate>>2;
  if(rgbRate>0)        --rgbRate;
  if(!bmd&&!sv2){
    memcpy(ledz, last, NUM_LEDS*3);
    sv2 = !sv2;
  }
  inv? pull(zero? k: hk): push(zero? k: hk);
  if(beatDetect()){
    if(!bmd){
      if(sv2){
        memcpy(last, ledz, NUM_LEDS*sizeof(CRGB));
        fmd?goGray():fill_solid(ledz,NUM_LEDS,CRGB::White);
        sv2 = !sv2;
      }
    } else{
      freshWhites();
      if(!hup){ rgbRate+=16; 
        hmd?huey=gHue:(dir?huey+=rgbRate:huey-=rgbRate);
      }
      ledz[ok] = CRGB::White;
      if(rate>0)
        ledz[ok==0? ok+1: ok-1] = CRGB::White;
    }
  } else{ // beatDetect()
    if(trebDetect()&&hup){ rgbRate+=8; }
    if(hup)
      hmd? huey=gHue: dir? huey+=rgbRate: huey-=rgbRate;
    ledz[ok] = ColorFromPalette(pmd? palettes[currPalIdx]:
                gCurrentPalette, huey, bri, LINEARBLEND);
  } // beatDetect()
  if(bri==0) ledz[ok] = CRGB(0,1,0);
  if(!zero){ mirror(k); copyIt(speed*3); }
}
