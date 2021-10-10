typedef struct sensorvalues {
    float temperature;
    float humidity;
    float pressure;
    uint16_t eco2;
} sensorvalues;

void setup_sensors();

sensorvalues get_sensor_values();

void to_json(sensorvalues sv, char *buffer);
