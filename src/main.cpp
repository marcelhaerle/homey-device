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
   reconnect();
}

void publish_data(char *json) {
  if (!client.connected()) {
    reconnect();
  } else {
    client.publish_P(MQTT_TOPIC, json, false);
    if (DEBUG) {
      Serial.println("> MQTT: Published data");
    }
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
  
  char *json = (char *)malloc(sizeof(char) * 350);
  to_json(sv, json);
  publish_data(json);
  free(json);
}
