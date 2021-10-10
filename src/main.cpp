#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

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

  setup_wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
}
