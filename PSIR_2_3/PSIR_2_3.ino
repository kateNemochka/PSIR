const int TEMP_SENSOR_PIN = A0;
int startTime;

void setup() {
  pinMode(TEMP_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  startTime = millis();
}

void loop() {
  float temperature = getTemperature(analogRead(TEMP_SENSOR_PIN));
  Serial.print("Time: ");
  Serial.print(millis() - startTime);
  Serial.print(" ms;\tTemperature: ");
  Serial.print(temperature);
  Serial.print("C\n");
  delay(1000);
}

float getTemperature(int analogInput) {
  float voltage = (analogInput * 5.0) / 1024.0;
  return (voltage - 0.5) * 100;
}
