#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "h_sensors.h"
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HOMEY_DEVICE_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
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
   reconnect();
}

void publish_data(char *json) {
  if (!client.connected()) {
    reconnect();
  } else {
    client.publish(MQTT_TOPIC, json);
    Serial.println("MQTT: Published data");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  setup_sensors();
  setup_wifi();
  setup_mqtt();
}

void loop() {
  delay(5000);

  sensorvalues sv = get_sensor_values();

  Serial.println("\n#########################");
  Serial.printf("# Temperature: %.2f C\n", sv.temperature);
  Serial.printf("# Humidity:    %.2f %%\n", sv.humidity);
  Serial.println("#########################\n");

  char *json = (char *)malloc(sizeof(char) * 200);
  sprintf(json,
    "{\"deviceId\": \"%s\",\"sensors\":[{\"type\": \"temperature\", \"unit\": \"Celsius\", \"value\": %.2f}, {\"type\": \"humidity\", \"unit\": \"%%\", \"value\": %.2f}]}",
    HOMEY_DEVICE_ID, sv.temperature, sv.humidity);
  publish_data(json);
  free(json);
}
