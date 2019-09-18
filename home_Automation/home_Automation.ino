// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"
#include <Servo.h>
// DHT 11 sensor
#define DHTPIN 14
#define DHTTYPE DHT11

Servo myservo;  // create servo object to control a servo

// Lamp pin
const int lamp_pin = 2;
const int fan_pin = 16;
int pirPin = 13;    //the digital pin connected to the PIR sensor's output
int doorPin = 15;
// WiFi parameters
#define WLAN_SSID       "DT_LAB"
#define WLAN_PASS       "fthu@050318"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "siddhesh"
#define AIO_KEY         "402abd741aae4a42b3847b22c71e6693"  // Obtained from account info on io.adafruit.com

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

WiFiClient client;
 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
/****************************** Feeds ***************************************/
 
// Setup a feed called 'lamp' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe lamp = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lamp");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fan");
Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/slider");
Adafruit_MQTT_Subscribe rgb = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/rgb");
//Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&mqtt, SLIDER_FEED);
// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
 
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish motion = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/motion");
Adafruit_MQTT_Publish doorState = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/doorState");

unsigned long currentMillis = 0;
int interval = 20000;

int pirState =0;
int doorPinState =0;
/*************************** Sketch Code ************************************/

void setup() {

  // Set lamp pin to output
  pinMode(lamp_pin, OUTPUT);
  pinMode(fan_pin, OUTPUT); 
  pinMode(pirPin, INPUT);
  pinMode(doorPin, INPUT_PULLUP);
  // Init sensor
  dht.begin();
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

  // listen for events on the lamp feed
  mqtt.subscribe(&lamp);
  mqtt.subscribe(&fan);
  mqtt.subscribe(&slider);
  //mqtt.subscribe(&rgb);
  // connect to adafruit io
  connect();

}

void loop() {
  
  Adafruit_MQTT_Subscribe *subscription;
  pirState = digitalRead(pirPin);
  doorPinState = digitalRead(doorPin);
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  // Grab the current state of the sensor
  int humidity_data = (int)dht.readHumidity();
  int temperature_data = (int)dht.readTemperature();

  // By default, the temperature report is in Celsius, for Fahrenheit uncomment
  //    following line.
  // temperature_data = temperature_data*(9.0/5.0) + 32.0;
 if(millis() - currentMillis > interval){
  // Publish data
  if (! temperature.publish(temperature_data))
    Serial.println(F("Failed to publish temperature"));
  else
    Serial.println(F("Temperature published!"));

  if (! humidity.publish(humidity_data))
    Serial.println(F("Failed to publish humidity"));
  else
    Serial.println(F("Humidity published!"));
  
  if(doorPinState == 1){
   if (! doorState.publish("CLOSED"))
    Serial.println(F("Failed to publish Door State"));
   else
    Serial.println(F("Door State published!"));
  }
  if(doorPinState == 0){
   if (! doorState.publish("OPEN"))
    Serial.println(F("Failed to publish Door State"));
   else
    Serial.println(F("Door State published!"));
  } 
   if (! motion.publish(pirState))
    Serial.println(F("Failed to publish PIR State"));
   else
    Serial.println(F("PIR State published!")); 
   Serial.print(F("PIR State: "));
   Serial.println(pirState); 
  //}
  currentMillis = millis();  
 }   
 
  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(1000)) {

    // we only care about the lamp events
    if (subscription == &lamp) {

      // convert mqtt ascii payload to int
      char *value = (char *)lamp.lastread;
      Serial.print(F("Received: "));
      Serial.println(value);

      // Apply message to lamp
      String message = String(value);
      message.trim();
      if (message == "ON") {digitalWrite(lamp_pin, LOW);}
      if (message == "OFF") {digitalWrite(lamp_pin, HIGH);}

    }

    // we only care about the lamp events
    if (subscription == &fan) {

      // convert mqtt ascii payload to int
      char *value = (char *)fan.lastread;
      Serial.print(F("Received: "));
      Serial.println(value);

      // Apply message to lamp
      String message = String(value);
      message.trim();
      if (message == "ON") {digitalWrite(fan_pin, LOW);}
      if (message == "OFF") {digitalWrite(fan_pin, HIGH);}

    }

    if (subscription == &slider) {
      Serial.print(F("Slider: "));
      Serial.println((char *)slider.lastread);
      uint16_t sliderval = atoi((char *)slider.lastread);  // convert to a number
      //analogWrite(PWMOUT, sliderval); myservo.write(pos);
      if(sliderval < 50)  myservo.write(0);
      else if(sliderval > 120)  myservo.write(90);
      else  myservo.write(180);
    }

    if (subscription == &rgb) {
      Serial.print(F("RGB: "));
      Serial.println((char *)rgb.lastread);
      //uint16_t sliderval = atoi((char *)slider.lastread);  // convert to a number
      
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
