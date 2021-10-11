#include <Arduino.h>
#include "config.h"
#include "h_sensors.h"
#include "SHT2x.h"
#include <Wire.h>
#include <Sodaq_BMP085.h>
#include <ccs811.h>

SHT2x sht;
Sodaq_BMP085 bmp;
CCS811 ccs811(-1);

void setup_sensors() {
    if (!sht.begin()) {
        Serial.println("Temp/Hum-Sensor not found!");
        while (1) {};
    }

    bmp.begin(BMP085_STANDARD);

    if (!ccs811.begin()) {
        Serial.println("CCS811 not found!");
        while (1) {}
    }

    if (!ccs811.start(CCS811_MODE_1SEC)) {
        Serial.println("CCS811 start failed!");
        while (1) {}
    }
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

    uint16_t eco2, etvoc, errstat, raw;
    ccs811.read(&eco2, &etvoc, &errstat, &raw);
    if (errstat == CCS811_ERRSTAT_OK) {
        sv.eco2 = eco2;
    } else {
        Serial.println("Error reading eco2.");
        sv.eco2 = -999;
    }
    
    return sv;
}

void to_json(sensorvalues sv, char *buffer) {
    const char *tpl = "{"
        "\"deviceId\": \"%s\", "
        "\"location\": \"%s\", "
        "\"sensors\": ["
            "{\"type\": \"temperature\", \"unit\": \"Celsius\", \"value\": %.2f}, "
            "{\"type\": \"humidity\", \"unit\": \"%%\", \"value\": %.2f}, "
            "{\"type\": \"pressure\", \"unit\": \"Pa\", \"value\": %.2f}, "
            "{\"type\": \"eco2\", \"unit\": \"ppm\", \"value\": %d}"
        "]"
        "}";
    sprintf(
        buffer, tpl,
        HOMEY_DEVICE_ID, HOMEY_LOCATION, sv.temperature, sv.humidity, sv.pressure, sv.eco2
    );
}
