typedef struct sensorvalues {
    float temperature;
    float humidity;
    float pressure;
} sensorvalues;

void setup_sensors();

sensorvalues get_sensor_values();
