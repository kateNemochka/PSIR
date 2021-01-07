#include "DHT.h"

#define DHTPIN 4 // Пін, до якого підключено датчик температури (DHT11)
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}


void loop() {
  Serial.println(dht.readTemperature());
  delay(1000);
}
