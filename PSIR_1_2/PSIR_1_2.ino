/*ЗАВДАННЯ
Елемент виведення: Індикатор рівня
Сигнали: 100%; 80%; …; 0%.
Тривалість: по 600 мс, пауза 2 с.*/

const int LVL_IND[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; //піни для LED
const int IND_SIZE = 10; //к-ть LED, що використовуються для індикатора
const int DISPLAY_TIME = 600;
const int PAUSE_TIME = 2000;


void setup() {
  for (int i = 0; i < IND_SIZE; i++) {
    pinMode(LVL_IND[i], OUTPUT);
  }
}

void loop() {
  for (int p = 100; p >= 0; p -= 20) {
    displayLevel(p);
    delay(DISPLAY_TIME);
    turnOffIndicator();
    delay(PAUSE_TIME);
  }
}


void displayLevel(int percent) {
  for (int i = 0; i < IND_SIZE; i++) {
    if (i + 1 <= percent * IND_SIZE / 100) {
      digitalWrite(LVL_IND[i], HIGH);
    }
  }
}
void turnOffIndicator() {
  for (int i = 0; i < IND_SIZE; i++) {
    digitalWrite(LVL_IND[i], LOW);
  }
}
