
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
  memcpy((s=="New Flow"?ledz:leds)+NUM_LEDS/k,
         (s=="New Flow"?ledz:leds),
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
  for(int i=0; i<320; ++i)
    out+=in[i];
  return out/320;
}
uint8_t getMax255(uint8_t in[]){
  uint8_t out = 0;
  for(int i=0; i<320; ++i)
    out=out<in[i]?in[i]:out;
  return out;
}
uint8_t getMaxBass(uint8_t in[]){
  uint8_t out = 0, mx = in[bassBand[0]!=0?0:1];
  for(int i=6; i>0; --i)
    if(mx<in[i]&&i!=bassBand[0]){
      mx = in[i];
      out = i;
    }
  return out;
}
/*
uint8_t getMaxTreb(uint8_t in[]){
  uint8_t out = 0, mx = 0;
  if(bassBand[0]==0||bassBand[1]==0){
    mx = 1;
    if(bassBand[0]==1||bassBand[1]==1)
      mx = 2;
  }
  for(int i=6; i>0; --i)
    if(mx<in[i]&&i!=bassBand[0]&&i!=bassBand[1]){
      mx = in[i];
      out = i;
    }
  return out;
}*/
uint8_t getMaxTreb(uint8_t in[]){
  uint8_t out = 0, mx = 0;
  if(bassBand[0]==0)
    mx = 1;
  for(int i=6; i>0; --i)
    if(mx<in[i]&&i!=bassBand[0]){
      mx = in[i];
      out = i;
    }
  return out;
}
/*
uint8_t getMaxTreb(uint8_t in[]){
  uint8_t out = 0, mx = 0;
  for(int i=6; i>0; --i)
    if(mx<in[i]){
      mx = in[i];
      out = i;
    }
  return out;
}
*/
uint16_t idx;
uint8_t bri[7], dfm[7], dfa[7],
        smp[7][320],dif[7][320], 
        bass[7], treb[7];
void saveBands(){
  for(byte b=0; b<7; ++b)
    smp[b][idx] = pow(spectrumByte[b]>>4,2);
  for(byte b=0; b<7; ++b)
    dif[b][idx] = abs(smp[b][idx+1]-smp[b][idx]);
}
void analyzeBands(){
  // Get Average
  for(byte b=0; b<7; ++b)
    bri[b] = getAverage255(smp[b]);
  // Get Average Difference
  for(byte b=0; b<7; ++b)
    dfa[b] = getAverage255(dif[b]);
  // Get Biggest Difference
  for(byte b=0; b<7; ++b)
    dfm[b] = getMax255(dif[b]);
  // Bass Analysis  
  for(byte b=0; b<7; ++b){
    bass[b] = bri[b]>dfm[b]? 0: dfm[b]-bri[b]>>1;
    if(b<2)
      bass[bassConf[b]] *= 9/8;
  }
  // Treb Analysis
  for(byte b=0; b<7; ++b){
    treb[b] = (dfa[b]+dfm[b]>>1+bri[b]>>2)/3;
    treb[trebConf] *= 9/8;
  }

}
void newFlow(){
  uint8_t bri, rate;
  bool bb = true, bt = true;
  EVERY_N_SECONDS(8){ if(mor) morph(); }
  EVERY_N_SECONDS(2){
    if(spectrumAvg>48){
      analyzeBands();
      trebBand = getMaxTreb(treb);
      trebConf = trebBand;
      for(byte b = 0; b<2; ++b){
        bassBand[b] = getMaxBass(bass);
        bassConf[b] = bassBand[b];
      }
    }
  }
   if(bfd) fadeWhites(16);
   if(tfd) fadeColors(1);
   if(mor) speed = morphs;
   // LIGHT SOURCE LOCATION
       bool zero = speed==0? true: false;
     uint16_t  k = edges[speed];
     uint16_t hk = k>>1;
     uint16_t ok = inv? (zero?k:hk): 0;
   
  saveBands();
  bri = smp[trebBand][idx];
  idx = idx==319? 0: idx+1;
  rate = (getAverage32(smp[trebBand])>>4)/3;
  rgbRate+=rate;
  if(rgbRate>3) rgbRate-=rgbRate/4;                         // Weighted Decay
  if(rgbRate>0) rgbRate--;                                  // Base Decay
  if(!bmd&&!sv2){                                           // webapp btn-Bass[Dots|Flash]-Flash Mode
    memcpy(ledz, last, NUM_LEDS*3);                         // copy last frame to this frame
    sv2 = !sv2;
  }
  inv? pull(zero? k: hk): push(zero? k: hk);                // webapp btn-Inversion decided push/pull
  if(beatDetect()){
    if(!bmd){                                               // webapp btn-Bass[Dots|Flash]-Flash Mode
      if(sv2){
        memcpy(last, ledz, NUM_LEDS*sizeof(CRGB));          // Copy this frame to placeholder
        fmd?goGray():fill_solid(ledz,NUM_LEDS,CRGB::White); // webapp btn-Flash[rgb|White]
        sv2 = !sv2;
      }
    } else{                                                 // webapp btn-Bass[Dots|Flash]-Dots Mode
      freshWhites();                                        // Refresh Bass Hit Pixels that faded
      if(!hup)                                              // webapp btn-Hue[Bass|Treb]-Treb Mode
        hmd?huey=gHue:(dir?huey+=rgbRate:huey-=rgbRate);    // webapp btn-Hue Speed decided hue set
      ledz[ok] = CRGB::White;                               // Paint the index
      if(rate>0)
        ledz[ok==0? ok+1: ok-1] = CRGB::White;              // IF 12 of 32 brightness thresholds were
    }                                                       //   reached increase bass hit width by 1
  } else{ // beatDetect()
    if(trebDetect()){ rgbRate+=9; }                            // "trebDetect" increases rgbRate by the most
    if(hup)                                                 // werbapp btn-Hue[Bass|Treb]-Treb Mode
      hmd? huey=gHue: dir? huey+=rgbRate: huey-=rgbRate;    // webapp btn-Hue Speed decided adjustment
    ledz[ok] = ColorFromPalette(pmd? palettes[currPalIdx]:  // Paint the index
                gCurrentPalette, huey, bri, LINEARBLEND);   // webapp btn-Palette Mode
  } // beatDetect()
  if(bri==0) ledz[ok] = CRGB(0,1,0);                        // Never let a pixel be black
  if(!zero){ mirror(k); copyIt(speed*3); }                  // Mirror from Index // copy mirrored section speed*3-1 times
}
