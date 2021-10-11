#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "h_sensors.h"
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("> Attempting MQTT connection...");
    if (client.connect(HOMEY_DEVICE_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("> MQTT: failed, rc=");
      Serial.print(client.state());
      Serial.println("");
    }
  }
}

void setup_wifi() {
  Serial.print("* Connecting to WiFi ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("* WiFi connected");
  Serial.print("* IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
}

void setup_mqtt() {
   client.setServer(MQTT_HOST, MQTT_PORT);
   client.setKeepAlive(DELAY_SECONDS + 2);
   reconnect();
}

void publish_data(char *json, unsigned int len) {
  if (!client.connected()) {
    reconnect();
  }
  client.beginPublish(MQTT_TOPIC, len, false);
  for (int i = 0; i < len; i++) {
    client.write(json[i]);
  }
  client.endPublish();
  if (DEBUG) {
    Serial.println("> MQTT: Published data");
  }
}

void print_config() {
  Serial.println("## HOMEY CONFIGURATION");
  Serial.printf("## WiFi SSID: %s\n", WIFI_SSID);
  Serial.printf("## WiFi Password: %s\n", WIFI_PASSWORD);
  Serial.printf("## MQTT Host: %s\n", MQTT_HOST);
  Serial.printf("## MQTT Port: %d\n", MQTT_PORT);
  Serial.printf("## Device ID: %s\n", HOMEY_DEVICE_ID);
  Serial.printf("## Location: %s\n", HOMEY_LOCATION);
  Serial.printf("## Debug: %d\n", DEBUG);
  Serial.printf("## Delay seconds: %d", DELAY_SECONDS);
  Serial.println("");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  print_config();

  setup_sensors();
  setup_wifi();
  setup_mqtt();
}

void loop() {
  delay(DELAY_SECONDS * 1000);

  sensorvalues sv = get_sensor_values();

  if (DEBUG) {
    Serial.println("\n############################");
    Serial.printf("# Temperature: %.2f C\n", sv.temperature);
    Serial.printf("# Humidity:    %.2f %%\n", sv.humidity);
    Serial.printf("# Pressure:    %.2f Pa\n", sv.pressure);
    Serial.printf("# eCO2:        %d ppa\n", sv.eco2);
    Serial.println("############################\n");
  }
  
  int json_len = 300;
  char *json = (char *)malloc(sizeof(char) * json_len);
  to_json(sv, json);
  if (DEBUG) {
    Serial.println(json);
  }
  publish_data(json, strlen(json));
  free(json);
}
