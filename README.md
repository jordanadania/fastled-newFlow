# FastLED + MSGEQ7
Audio reactive FastLED pattern using the MSGEQ7.

## Hardware
<b>MSGEQ7</b> breakout board (for audio)<br>This firmware uses the MONO version, not STEREO<br>(Notice the single OUT pin pictured below)

|<a href="https://www.ebay.com/itm/MSGEQ7-breakout-board-7-band-graphic-equalizer-for-audio-for-Arduino-or-RPi/353131861821?hash=item52384c973d:g:L3sAAOSwt4xfOK08">EBay</a>|<a href="https://smile.amazon.com/MSGEQ7-Spectrum-Analyzer-Breakout-Stereo/dp/B08GY46BY8/ref=sr_1_2?dchild=1&keywords=MSGEQ7&qid=1609786663&sr=8-2">Amazon</a>|
|-|-|
|<a href="https://www.ebay.com/itm/256192596092"><img src="https://i.ebayimg.com/images/g/JJUAAOSw40Vk5Qdl/s-l1600.webp" width="320"></a>|<a href="https://smile.amazon.com/MSGEQ7-Spectrum-Analyzer-Breakout-Stereo/dp/B08GY46BY8/ref=sr_1_2?dchild=1&keywords=MSGEQ7&qid=1609786663&sr=8-2"><img src="https://images-na.ssl-images-amazon.com/images/I/816hNJK1coL._SL1500_.jpg" width="320"></a>|

<b>ESP8266</b> dev board:<br>(<i>probably</i> any microcontroller supported by Arduino IDE will work)<br>(if you get one with pins pre-soldered, you can use jumper wires to connect the leds to the board)<br>(Otherwise, you will need to know how to solder on a connector of some kind)

|<a href="https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=nodemcu+12-e&qid=1609781366&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTFI4VlIyTDNPVlY5JmVuY3J5cHRlZElkPUEwODI4NDQ3MjVEMlY0NUtFN0lSNyZlbmNyeXB0ZWRBZElkPUEwNTYyNjkzMU5WTEI1SjdJUTlDJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==">NodeMCU 12-E</a>|<a href="https://www.amazon.com/AITRIP-NodeMcu-Internet-Development-Compatible/dp/B08C7FYM5T/ref=sr_1_2?dchild=1&keywords=d1+mini&qid=1609781610&sr=8-2">D1 Mini 12-F</a>|
|-|-|
|<a href="https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=nodemcu+12-e&qid=1609781366&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTFI4VlIyTDNPVlY5JmVuY3J5cHRlZElkPUEwODI4NDQ3MjVEMlY0NUtFN0lSNyZlbmNyeXB0ZWRBZElkPUEwNTYyNjkzMU5WTEI1SjdJUTlDJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ=="><img src="https://images-na.ssl-images-amazon.com/images/I/61GwRCoPxlL._AC_SL1035_.jpg" width="320"></a>|<a href="https://www.amazon.com/AITRIP-NodeMcu-Internet-Development-Compatible/dp/B08C7FYM5T/ref=sr_1_2?dchild=1&keywords=d1+mini&qid=1609781610&sr=8-2"><img src="https://images-na.ssl-images-amazon.com/images/I/61KNTnEWAXL._AC_SL1000_.jpg" width="320"></a>|

<b>Digital LEDs (WS2811/WS2812B/etc)</b>

|<a href="https://smile.amazon.com/ALITOVE-Diffused-Digital-Addressable-Waterproof/dp/B06XN66ZY6/ref=sr_1_18?dchild=1&keywords=ws2811&qid=1609792744&sr=8-18">WS2811</a>|<a href="https://www.amazon.com/ALITOVE-Addressable-Programmable-Waterproof-Raspberry/dp/B07FVPN3PH/ref=sr_1_5?dchild=1&keywords=ws2812b+30%2Fm&qid=1609793360&s=hi&sr=1-5">WS2812B</a>|
|-|-|
|<a href="https://smile.amazon.com/ALITOVE-Diffused-Digital-Addressable-Waterproof/dp/B06XN66ZY6/ref=sr_1_18?dchild=1&keywords=ws2811&qid=1609792744&sr=8-18"><img src="https://images-na.ssl-images-amazon.com/images/I/61X6jpId2nL._AC_SL1000_.jpg" width="320"></a>|<a href="https://www.amazon.com/ALITOVE-Addressable-Programmable-Waterproof-Raspberry/dp/B07FVPN3PH/ref=sr_1_5?dchild=1&keywords=ws2812b+30%2Fm&qid=1609793360&s=hi&sr=1-5"><img src="https://images-na.ssl-images-amazon.com/images/I/61jf8kqPMLL._AC_SL1000_.jpg" width="320"></a>|

## Features
* Automatic Channel Analysis (Finds Bass and Treble Channels in realtime)
* Bass/Beat Hits will spawn a white pixel that fades over time
* Bass/Beat Hits will 'refresh' all white pixels to full brightness
* Treble/Vocal/Instrumental 'Hits' will spawn a pixel from a palette (Rainbow_p by default)
* Spawns are marquee'd cross every frame

## Installing
### Step 1 - Get Arduino IDE
- <a href="https://www.arduino.cc/en/main/software">Download and Install - Arduino IDE</a>
### Step 2 - Tell Arduino About FastLED
- Click 'Sketch' > 'Include Library' > 'Manage libraries...'
- Search for "FastLED"
- Install "**FastLED** by **Daniel Garcia**"
- Click 'Close'
### Step 3 - Tell Arduino About ESP8266
- Click 'File' > 'Preferences'
- Paste the following url into the Additional Boards Manager field near the bottom of the dialog box:
- - http://arduino.esp8266.com/stable/package_esp8266com_index.json
- Click 'OK'
- Click 'Tools' > 'Board:' > 'Boards Manager...'
- Search for "8266"
- Install "**esp8266** by **ESP8266 Community**"
- Click 'Close'
### Step 4 - Select Your Board
- Click 'Tools' > 'Board:' > 'ESP8266 Boards (#.#.#)'
- Find your specific board
- - NodeMCU **1.0** (ESP-12**E** Module)
- - LOLIN WEMOS **D1 R2 & mini**
