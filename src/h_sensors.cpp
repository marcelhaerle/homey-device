#include <Arduino.h>
#include "config.h"
#include "h_sensors.h"
#include "SHT2x.h"
#include <Wire.h>
#include <Sodaq_BMP085.h>

SHT2x sht;
Sodaq_BMP085 bmp;

void setup_sensors() {
    if (!sht.begin()) {
        Serial.println("Temp/Hum-Sensor not found!");
        while (1) {};
    }

    bmp.begin(BMP085_STANDARD);
}

sensorvalues get_sensor_values() {
    sensorvalues sv;
    
    if (sht.read()) {
        sv.temperature = sht.getTemperature();
        sv.humidity = sht.getHumidity();
    } else {
        Serial.println("Error reading Temp/Hum values.");
        sv.temperature = -999.9;
        sv.humidity = -999.9; 
    }

    sv.pressure = bmp.readPressure();
    
    return sv;
}

void to_json(sensorvalues sv, char *buffer) {
      sprintf(buffer,
    "{\"deviceId\": \"%s\",\"sensors\":[{\"type\": \"temperature\", \"unit\": \"Celsius\", \"value\": %.2f}, {\"type\": \"humidity\", \"unit\": \"%%\", \"value\": %.2f}, {\"type\": \"pressure\", \"unit\": \"Pa\", \"value\": %.2f}]}",
    HOMEY_DEVICE_ID, sv.temperature, sv.humidity, sv.pressure);
}
