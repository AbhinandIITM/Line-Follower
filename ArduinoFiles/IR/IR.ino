#define IR_SENSOR A0  

void setup() {
    Serial.begin(9600);
}

int getStableReading() {
    const int numSamples = 10;  // More samples = smoother data
    int total = 0;

    for (int i = 0; i < numSamples; i++) {
        total += analogRead(IR_SENSOR);
        delay(2);  // Small delay to reduce noise
    }

    return total / numSamples;  // Return averaged value
}

void loop() {
    int sensorValue = getStableReading();
    int mappedValue = map(sensorValue, 0, 1023, 0, 255);  

    Serial.print("Raw Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print(" | Mapped Value: ");
    Serial.println(mappedValue);

    delay(100);
}
