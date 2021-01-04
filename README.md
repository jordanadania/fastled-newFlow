FastLED + MSGEQ7
=========

Audio reactive FastLED pattern using the MSGEQ7.

Hardware
--------

MSGEQ7  breakout board (7-band graphic equalizer for audio):<br>You only need the mono version for this project, buy stereo if you wish.<br>
[![MSGEQ7  breakout board](https://i.ebayimg.com/images/g/XMoAAOSw0e9UwT9y/s-l300.jpg)](https://www.ebay.com/itm/MSGEQ7-breakout-board-7-band-graphic-equalizer-for-audio-for-Arduino-RPi-PIC-/301210655107)

An ESP8266 development board:<br>(other boards may work, this is just the chip I prefer)<br><br>
<a href="https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=nodemcu+12-e&qid=1609781366&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTFI4VlIyTDNPVlY5JmVuY3J5cHRlZElkPUEwODI4NDQ3MjVEMlY0NUtFN0lSNyZlbmNyeXB0ZWRBZElkPUEwNTYyNjkzMU5WTEI1SjdJUTlDJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==">
 
|NodeMCU 12-E</a>|<a href="https://www.amazon.com/AITRIP-NodeMcu-Internet-Development-Compatible/dp/B08C7FYM5T/ref=sr_1_2?dchild=1&keywords=d1+mini&qid=1609781610&sr=8-2">D1 Mini 12-F|
|------------|------------|
|<a href="https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=nodemcu+12-e&qid=1609781366&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTFI4VlIyTDNPVlY5JmVuY3J5cHRlZElkPUEwODI4NDQ3MjVEMlY0NUtFN0lSNyZlbmNyeXB0ZWRBZElkPUEwNTYyNjkzMU5WTEI1SjdJUTlDJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ=="><img src="https://images-na.ssl-images-amazon.com/images/I/61GwRCoPxlL._AC_SL1035_.jpg" width="320"></a>|<a href="https://www.amazon.com/AITRIP-NodeMcu-Internet-Development-Compatible/dp/B08C7FYM5T/ref=sr_1_2?dchild=1&keywords=d1+mini&qid=1609781610&sr=8-2"><img src="https://images-na.ssl-images-amazon.com/images/I/61KNTnEWAXL._AC_SL1000_.jpg" width="320"></a>|

Digital LEDs:<br>(WS2811/WS2812B/etc)

Features
--------
* Automatic Channel Analysis (Finds Bass and Treble Channels in realtime)
* Bass/Beat Hits will spawn a white pixel that fades over time
* Bass/Beat Hits will 'refresh' all white pixels to full brightness
* Treble/Vocal/Instrumental 'Hits' will spawn a pixel from a palette (Rainbow_p by default)
* Spawns are marquee'd cross every frame

Installing
-----------
The code is uploaded via the Arduino IDE which can be [downloaded here](https://www.arduino.cc/en/main/software). The ESP8266 boards will need to be added to the Arduino IDE which is achieved as follows. Click File > Preferences and copy and paste the URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into the Additional Boards Manager URLs field. Click OK. Click Tools > Boards: ... > Boards Manager. Find and click on ESP8266 (using the Search function may expedite this). Click on Install. After installation, click on Close and then select your ESP8266 board from the Tools > Board: ... menu.

The app depends on the following libraries. They must either be downloaded from GitHub and placed in the Arduino 'libraries' folder, or installed as [described here](https://www.arduino.cc/en/Guide/Libraries) by using the Arduino library manager.

* [FastLED](https://github.com/FastLED/FastLED)
