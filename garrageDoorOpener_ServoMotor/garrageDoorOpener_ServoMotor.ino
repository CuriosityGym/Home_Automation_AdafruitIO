// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// WiFi parameters
#define WLAN_SSID       "DT_LAB"
#define WLAN_PASS       "fthu@050318"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "siddhesh"
#define AIO_KEY         "402abd741aae4a42b3847b22c71e6693"  // Obtained from account info on io.adafruit.com


WiFiClient client;
 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
/****************************** Feeds ***************************************/
 
// Setup a feed called 'lamp' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/slider");
 
/*************************** Sketch Code ************************************/

void setup() {

  myservo.attach(12);  // attaches the servo on GIO2 to the servo object
  myservo.write(0);
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&slider);
  // connect to adafruit io
  connect();

}

void loop() {
  
  Adafruit_MQTT_Subscribe *subscription;
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
 
  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(1000)) {


    if (subscription == &slider) {
      Serial.print(F("Slider: "));
      Serial.println((char *)slider.lastread);
      uint16_t sliderval = atoi((char *)slider.lastread);  // convert to a number
      //analogWrite(PWMOUT, sliderval); myservo.write(pos);
      if(sliderval < 50)  myservo.write(0);
      else if(sliderval > 120)  myservo.write(90);
      else  myservo.write(180);
    }

  }
  // Repeat every 10 seconds
  //delay(10000);

}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
