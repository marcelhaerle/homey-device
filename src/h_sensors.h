typedef struct sensorvalues {
    float temperature;
    float humidity;
} sensorvalues;

void setup_sensors();

sensorvalues get_sensor_values();
