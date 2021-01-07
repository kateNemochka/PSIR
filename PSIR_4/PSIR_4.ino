/*1.  Під’єднання елементів введення і виведення даних. Під’єднати до плати світлову гірлянду (мінімум на 30 RGB-діодів) і датчик за індивідуальним варіантом (див. табл. нижче).
2.  Реалізація веб-інтерфейсу. Засобами Arduino IDE ввімкнути Wi-Fi та згенерувати для передачі в мережу веб-сторінку, яка містить 3 кнопки.
3.  Кнопка 1 — «Діод». По натисненні на кнопку вбудований (або зовнішній) світлодіод починає блимати, передаючи Ваше прізвище, закодоване абеткою Морзе (див. ЛР № 1).
4.  Кнопка 2 — «Гірлянда». По натисненні на кнопку в під’єднаній світловій гірлянді загоряються N світлодіодів (N = Ваш номер у загальному списку групи).
Діоди блимають, загоряючись N разів по 1 с, паузи між загоряннями — довільні.
5.  Кнопка 3 — «Датчик». По натисненні на кнопку, на сторінці під кнопками починається виведення даних, які в цей час зчитуються з під’єднаного датчика.
Формат (вигляд) виведення інформації на сторінку — довільний.
*/

#include "ESP8266WiFi.h"
#include "Adafruit_NeoPixel.h"
#include "DHT.h"


#define LIGHTSPIN D1 // Гірлянда
#define LEDPIN D6 // Світлодіод для відображення імені за допомогою азбуки Морзе
#define DHTPIN D2 // Пін, до якого підключено датчик температури (DHT11)
#define DHTTYPE DHT11

#define NUMLIGHTS 30 // Кількість світлодіодів, що використовуються в гірлянді
#define LIGHTSDELAY 1000 // Пауза між роботою світлодіодів в гірлянді

const char* ssid = "MyPoint"; // Wi-Fi Name
const char* password = "abcde"; // Wi-Fi Password

// Часові інтервали для азбуки Морзе
const int DOT = 250;
const int DASH = DOT * 3;
const int WORDS_GAP = DASH * 2;
const int MAX_CODE_LEN = 5;

Adafruit_NeoPixel pixels(NUMLIGHTS, LIGHTSPIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);
WiFiClient client;

// Температура, що зчитується датчиком
double temperature = 100.0;



void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  
  Serial.begin(115200);
  dht.begin();
  pixels.begin();

  connectToWiFi();
}


void loop() {
  client = server.available();
  if (!client) {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available()) {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  temperature = 100.0;
  if(request.indexOf("/SENSOR") != -1) {
    temperature = dht.readTemperature();
  }
  
  generatePage(temperature);

  if(request.indexOf("/LED") != -1) {
    blinkMorse();
  } else if(request.indexOf("/LIGHTS") != -1) {
    startLights();
  }
    
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}



void connectToWiFi() {
  Serial.print("Connecting to the Network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();  // Starts the Server
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void generatePage(double temperatureValue) {
  //*-----------HTML Page Code--------------*//
  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang=\"uk\">");
  client.println("<head><meta charset=\"utf-8\"/></head>");
  client.println("<body>");
  client.println("<h1>Віддалене керування Arduino</h1>");
  client.println("<a href=\"/LED\"\"><button>Діод</button></a><br>");
  client.println("<a href=\"/LIGHTS\"\"><button>Гірлянда</button></a><br>");
  client.println("<a href=\"/SENSOR\"\"><button>Датчик</button></a><br>");

  if (temperatureValue != 100.0) {
    client.print("<p>Поточна температура: ");
    client.print(temperatureValue);
    client.print(" C<p>");
  }
  
  client.println("</body></html>");
}


void startLights() {
  pixels.clear(); // Set all pixel colors to 'off'

  int repeats = 10;
  for (int r=0; r<10; r++) {
    for (int p=0; p<NUMLIGHTS; p+=3) {
      pixels.setPixelColor(p + r % 3, pixels.Color(0, 150, 0));
    }
    pixels.show();
    delay(LIGHTSDELAY);
    pixels.clear();
    delay(LIGHTSDELAY);
  }
}


void blinkMorse() {
  int SURNAME[8][MAX_CODE_LEN] = {
    {DASH, DOT},              //Н
    {DOT},                    //Е
    {DASH, DASH},             //М
    {DASH, DASH, DASH, DOT},  //Ч
    {DOT},                    //Е
    {DASH, DOT},              //Н
    {DASH, DOT,  DASH},       //К
    {DASH, DASH, DASH}        //О
  };
  
  int NAME[8][MAX_CODE_LEN] = {
    {DASH, DOT,  DASH},       //К
    {DOT,  DASH},             //А
    {DASH},                   //Т
    {DOT},                    //Е
    {DOT,  DASH, DOT},        //Р
    {DASH, DOT,  DASH, DASH}, //И
    {DASH, DOT},              //Н
    {DOT,  DASH}              //А
  };

  blinkWordInMorse(SURNAME, sizeof(SURNAME) / sizeof(SURNAME[0]));
  blinkWordInMorse(NAME, sizeof(NAME) / sizeof(NAME[0]));
}

void blinkWordInMorse(int morseWord[][MAX_CODE_LEN], int wordLen) {
  for (int i = 0; i < wordLen; i++) {
    for (int j = 0; j < MAX_CODE_LEN; j++) {
      if (morseWord[i][j] > 0) {
        digitalWrite(LEDPIN, HIGH);
        delay(morseWord[i][j]);
        digitalWrite(LEDPIN, LOW);
        if (j < MAX_CODE_LEN - 1 && morseWord[i][j + 1] != 0) {
          delay(DOT);
        }
      }
    }
    delay(DASH);
  }
  delay(WORDS_GAP);
}
