#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <HCSR04.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;  

char mqtt_server[] = "broker.emqx.io";
int mqtt_port = 1883;
char topic[] = "SIT210/wave";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const int TRIG_PIN = 13;
const int ECHO_PIN = 12;

void setupWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ArduinoClient")) {
            Serial.println("Connected to MQTT!");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Trying again...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    setupWiFi();
    client.setServer(mqtt_server, mqtt_port);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;

    if (distance < 10) {
        Serial.println("Wave detected!");
        client.publish(topic, "wave");
        delay(2000); // Avoid multiple triggers
    }
}