#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(A0, INPUT);

  lcd.begin(16, 2);
  lcd.print("Temp: ");
}

void loop() {
  float temperature = getTemperature(analogRead(A0));
  
  lcd.setCursor(6, 0);
  lcd.print(temperature);
  lcd.print("C  ");
  
  lcd.setCursor(0, 1);
  
  if (temperature < 0) {
    lcd.print("Below zero");
  } else {
    lcd.print("Above zero");
  }
  
  delay(50);
}

float getTemperature(int analogInput) {
  float voltage = (analogInput * 5.0) / 1024.0;
  return (voltage - 0.5) * 100;
}
