/*Українська азбука морзе
Стандартом довжини звукових (або світлових) сигналів вважається, 
коли довгий сигнал (що позначається тире) втричі довший за короткий (що позначався крапкою), пауза між звуковими сигналами одного символу дорівнює тривалості короткого сигналу, пауза між символами дорівнює тривалості довгого сигналу, пауза між словами — тривалості двох довгих сигналів.*/

const int LED_PIN = 13;

const int DOT = 250;            //крапка, пауза між символами морзе
const int DASH = DOT * 3;       //риска, пауза між кодами літер
const int WORDS_GAP = DASH * 2; //пауза між словами

const int MAX_CODE_LEN = 5;

int SURNAME[8][MAX_CODE_LEN] = {
  {DASH, DOT},              //Н -•
  {DOT},                    //Е •
  {DASH, DASH},             //М --
  {DASH, DASH, DASH, DOT},  //Ч ---•
  {DOT},                    //Е •
  {DASH, DOT},              //Н -•
  {DASH, DOT,  DASH},       //К -•-
  {DASH, DASH, DASH}        //О ---
};

int NAME[8][MAX_CODE_LEN] = {
  {DASH, DOT,  DASH},       //К -•-
  {DOT,  DASH},             //А •-
  {DASH},                   //Т -
  {DOT},                    //Е •
  {DOT,  DASH, DOT},        //Р •-•
  {DASH, DOT,  DASH, DASH}, //И -•--
  {DASH, DOT},              //Н -•
  {DOT,  DASH}              //А •-
};

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
  blinkWordInMorse(SURNAME, sizeof(SURNAME) / sizeof(SURNAME[0]));
  blinkWordInMorse(NAME, sizeof(NAME) / sizeof(NAME[0]));
}


void blinkWordInMorse(int morseWord[][MAX_CODE_LEN], int wordLen) {
  for (int i = 0; i < wordLen; i++) {
    for (int j = 0; j < MAX_CODE_LEN; j++) {
      if (morseWord[i][j] > 0) {
        digitalWrite(LED_PIN, HIGH);
        delay(morseWord[i][j]);
        digitalWrite(LED_PIN, LOW);
        if (j < MAX_CODE_LEN - 1 && morseWord[i][j + 1] != 0) {
          delay(DOT);
        }
      }
    }
    delay(DASH);
  }
  delay(WORDS_GAP);
}
