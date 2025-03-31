#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include <HCSR04.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
float Distance = 0.0;

UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char waveTopic[]  = "SIT210/wave4";
const char patTopic[]  = "SIT210/pat";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
    
    Serial.println("Checking the distance");

    Distance = distanceSensor.measureDistanceCm();

    if(Distance < 20.00) {

      Serial.println("Wave detected");
      Serial.print("Sending message to topic: ");
      Serial.println(waveTopic);
      Serial.println("DinithWave");

      // send message, the Print interface can be used to set the message contents
      mqttClient.beginMessage(waveTopic);
      mqttClient.print("DinithWave");
      mqttClient.endMessage();

      Serial.println();

    } else if (Distance < 40) {

      Serial.println("Pat detected");
      Serial.print("Sending message to topic: ");
      Serial.println(patTopic);
      Serial.println("DinithPat");

      // send message, the Print interface can be used to set the message contents
      mqttClient.beginMessage(patTopic);
      mqttClient.print("DinithPat");
      mqttClient.endMessage();

      Serial.println();

    }
  }
}