#include <Arduino.h>
#include "h_sensors.h"
#include "SHT2x.h"

SHT2x sht;

void setup_sensors() {
    if (!sht.begin()) {
        Serial.println("Temp/Hum-Sensor not found!");
        while (1) {};
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
    
    return sv;
}
