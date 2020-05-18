// Everything on this page, I wrote myself.

void fadeWhites(uint8_t value){
  for(int i=1; i<NUM_LEDS-1; ++i)                                 
    if((ledz[i].r== ledz[i].g && ledz[i].g== ledz[i].b)
    &&(max(max(ledz[i].r, ledz[i].g), ledz[i].b) > 32
    &&(ledz[i].r > max(max(ledz[i-1].r, ledz[i-1].g), ledz[i-1].b)      // Dont ask. lol
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
      gHue = max(max(ledz[i].r, ledz[i].g), ledz[i].b);
      ledz[i] = CRGB(gHue,gHue,gHue);
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
uint8_t getAverage(uint8_t in[]){
  uint16_t out = 0;
  for(int i=0; i<127; ++i)
    out+=in[i];
  return out/127;
}
uint8_t getMax(uint8_t in[]){
  uint8_t out = 0;
  for(int i=0; i<127; ++i)
    out=out<in[i]?in[i]:out;
  return out;
}


uint8_t idx, 
        smp0[127], smp1[127], smp2[127], smp3[127], smp4[127], smp5[127], smp6[127],
        dif0[127], dif1[127], dif2[127], dif3[127], dif4[127], dif5[127], dif6[127];  
void saveBands(){
  smp0[idx] = pow(spectrumByte[0]>>4,2);
  smp1[idx] = pow(spectrumByte[1]>>4,2);
  smp2[idx] = pow(spectrumByte[2]>>4,2);
  smp3[idx] = pow(spectrumByte[3]>>4,2);
  smp4[idx] = pow(spectrumByte[4]>>4,2);
  smp5[idx] = pow(spectrumByte[5]>>4,2);
  smp6[idx] = pow(spectrumByte[6]>>4,2);
  dif0[idx] = abs(smp0[idx+1]-smp0[idx]);
  dif1[idx] = abs(smp1[idx+1]-smp1[idx]);
  dif2[idx] = abs(smp2[idx+1]-smp2[idx]);
  dif3[idx] = abs(smp3[idx+1]-smp3[idx]);
  dif4[idx] = abs(smp4[idx+1]-smp4[idx]);
  dif5[idx] = abs(smp5[idx+1]-smp5[idx]);
  dif6[idx] = abs(smp6[idx+1]-smp6[idx]);
}
void newFlow(){
  static uint8_t oldRate, rgbRate, huey;
  uint8_t bri, bri0, bri1, bri2, bri3, bri4, bri5, bri6,
          rate,dff0, dff1, dff2, dff3, dff4, dff5, dff6;
  bool bb = true, bt = true;
  EVERY_N_SECONDS(2){
    bri0 = getAverage(smp0);
    bri1 = getAverage(smp1);
    //bri2 = getAverage(smp2);
    bri3 = getAverage(smp3);
    bri4 = getAverage(smp4);
    bri5 = getAverage(smp5);
    bri6 = getAverage(smp6);
    dff0 = getMax(dif0);
    dff1 = getMax(dif1);
    dff2 = getMax(dif2);
    dff3 = getMax(dif3);
    dff4 = getMax(dif4);
    //dff5 = getMax(dif5);
    dff6 = getMax(dif6); 
    bt = (dff6+bri6)/2*9/8>(dff3+bri3)/2? true:false;
    bb = (dff0+dff1)/2*9/8>(dff3+dff4)/2? true:false;
    if(trebSwap&&!bt||!trebSwap&&bt){
      trebSwap = !trebSwap;
      Serial.println(trebSwap?"TREB CH 6":"TREB CH 3");
    }
    if(bassSwap&&!bb&&trebSwap||!bassSwap&&bb){
      bassSwap = !bassSwap;
      Serial.println(bassSwap?"BASS CH 0|1":"BASS CH 3|4");
    }
    if(!trebSwap&&!bassSwap){
      bassSwap = true;
      Serial.println("BASS CH 0|1");
    }
  }
  fadeWhites(16);
uint8_t speed = 1; // SET TO 1 FOR CLASS ASSIGNMENT
    bool zero = speed==0? true:false;
  uint16_t  k = edges[speed];
  uint16_t hk = k>>1;
  uint16_t ok = hk;
  saveBands();
  bri = trebSwap? smp6[idx]:smp3[idx];
  ++idx;
  rate = getAverage(trebSwap?smp6:smp3)>>5;
  rgbRate+=rate;
  if(rate!=oldRate){ oldRate = rate; Serial.println(rate); }
  if(rgbRate>3) rgbRate-=rgbRate/4;                         // Weighted Decay
  if(rgbRate>0) rgbRate--;                                  // Base Decay
  pull(zero? k: hk);                                        // webapp btn-Inversion decided push/pull
  if(beatDetect()){                                         // if beatDetect()
    freshWhites();                                          // Refresh Bass Hit Pixels that faded
    huey+=rgbRate; 
    ledz[ok] = CRGB::White;                                 // Paint the index
    if(rate>0) ledz[ok==0? ok+1: ok-1] = CRGB::White;       //   reached increase bass hit width by 1                                                     
  } else{ // beatDetect()
    if(trebDetect()) rgbRate+=9;                            // "trebDetect" increases rgbRate by the most
    huey+=rgbRate;                                          // webapp btn-Hue Speed decided adjustment
    ledz[ok] = ColorFromPalette(RainbowColors_p, huey, bri, LINEARBLEND);   // Paint the index
  } // beatDetect()
  if(bri==0) ledz[ok] = CRGB(0,1,0);                        // Never let a pixel be black
  if(!zero){ mirror(k); copyIt(speed*NUM_STRIPS); }                  // Mirror from Index // copy mirrored section speed*3-1 times
}
