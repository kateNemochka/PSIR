const int TEMP_SENSOR_PIN = A0;

void setup() {
  pinMode(TEMP_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Time");
}

void loop() {
  float temperature = getTemperature(analogRead(TEMP_SENSOR_PIN));
  Serial.println(temperature);
  delay(250);
}

float getTemperature(int analogInput) {
  float voltage = (analogInput * 5.0) / 1024.0;
  return (voltage - 0.5) * 100;
}
