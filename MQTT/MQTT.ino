#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <HCSR04.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;  

// char mqttTopic[] = "SIT210/wave";

char mqttServer[] = "broker.emqx.io";  // EMQX Public Broker
int mqttPort = 1883;  // Unsecured MQTT Port (No TLS)
char mqttClientID[] = "ArduinoClient";  // Unique client ID

// MQTT Topic (Same for Publishing & Subscribing)
char mqttTopic[] = "wave/detect";

// Define Pins for UltraSonic Sensor
#define TRIG_PIN 13
#define ECHO_PIN 12

// LED Pin
#define LED_PIN 5

// Initialize the ultrasonic sensor with the specified pins
UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);

WiFiClient net;
PubSubClient client(net);

// Function to Connect to Wi-Fi
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
}

// Function to Connect to EMQX MQTT Broker
void connectMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  Serial.print("Connecting to MQTT...");
  while (!client.connected()) {
    if (client.connect(mqttClientID)) {  // No username/password needed
      Serial.println("\nMQTT Connected!");
      client.subscribe(mqttTopic);  // Subscribe to the same topic
    } else {
      Serial.print(".");
      delay(2000);  // Wait for 2 seconds before retrying
    }
  }
}

// Function to Handle Incoming MQTT Messages
void callback(char topic[], byte payload[], unsigned int length) {
  char message[10];  // Buffer to store received message
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';  // Null-terminate the string

  Serial.print("Received message: ");
  Serial.println(message);
  
  if (strcmp(message, "wave") == 0) {  // Compare received message
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
}

// Function to Detect Wave & Publish MQTT Message
void detectWave() {
  int distance = distanceSensor.measureDistanceCm();  // Measure the distance in cm
  
  if (distance > 0 && distance < 10) {  // Wave detected if object is within 10 cm
    Serial.println("Wave detected! Sending MQTT message...");
    client.publish(mqttTopic, "wave");
    delay(2000);  // Prevent spamming the topic
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  connectWiFi();
  connectMQTT();
}

void loop() {
  // Ensure that the connection is only attempted if it is disconnected
  if (!client.connected()) {
    connectMQTT();
  }
  
  client.loop();  // Handle incoming messages

  detectWave();  // Continuously detect wave
}