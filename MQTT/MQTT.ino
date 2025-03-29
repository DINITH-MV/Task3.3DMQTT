#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;  

char mqttServer[] = "";
int mqttPort = 8883;
char mqttUser[] = "";
char mqttPass[] = "";

// char mqttTopic[] = "SIT210/wave";

char mqttServer[] = "broker.emqx.io";  // EMQX Public Broker
int mqttPort = 1883;  // Unsecured MQTT Port (No TLS)
char mqttClientID[] = "ArduinoClient";  // Unique client ID




