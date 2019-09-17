# Home_Automation_AdafruitIO
Home Automation using Adafruit IO platform



Prerequisites
Arduino 1.6.5, get it from Arduino website. Arduino 1.6.6 has several issues, so we recommend to stick with 1.6.5 for now.
Internet connection
Instructions
To install the ESP8266 board in your Arduino IDE, follow these next instructions:

1.In your Arduino IDE, go to File> Preferences
![alt text](https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Install-ESP8266-Board-add-on-in-Arduino-IDE-open-preferences.png?w=197&ssl=1)
2.Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into the “Additional Boards Manager URLs” field as shown in the figure below. Then, click the “OK” button:
![alt text](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Install-ESP8266-Board-add-on-in-Arduino-IDE-enter-URL.png?w=722&ssl=1)
Note: if you already have the ESP32 boards URL, you can separate the URLs with a comma as follows:
https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Open the Boards Manager. Go to Tools > Board > Boards Manager…
![alt text](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Install-ESP8266-Board-add-on-in-Arduino-IDE-open-boards-manager.png?w=671&ssl=1)
4. Search for ESP8266 and press install button for the “ESP8266 by ESP8266 Community“:
![alt text](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Install-ESP8266-Board-add-on-in-Arduino-IDE-search-ESP8266.png?w=786&ssl=1)
5. That’s it. It should be installed after a few seconds.
![alt text](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/ESP8266-Board-add-on-in-Arduino-IDE-installed.png?w=786&ssl=1)
