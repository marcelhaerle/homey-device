#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "h_sensors.h"

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

void setup() {
  Serial.begin(115200);
  delay(100);

  setup_sensors();
  setup_wifi();
}

void loop() {
  delay(1000);

  sensorvalues sv = get_sensor_values();
  Serial.printf("Temperature: %.2f\nHumidity: %.2f\n\n", sv.temperature, sv.humidity);
}
