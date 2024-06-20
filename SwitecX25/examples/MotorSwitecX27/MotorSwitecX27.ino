#include "SwitecX25.h"

#define STEPS (315*6)

SwitecX25 motor1(STEPS,62,63,64,65);

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

void setup() {
  motor1.zero();  // Ustawienie silniczka do pozycji początkowej

#pragma region InicjalizacjaKomunikacji
  // Inicjalizacja preambuły
  TablicaDoPC[0] = 0xEF;
  TablicaDoPC[1] = 0xEF;
  TablicaDoPC[2] = 0xEF;
  TablicaDoPC[3] = 0xEF;

  // Konfiguracja komunikacji
  Serial.begin(115200);  //nawiązanie komunikacji z MaSzyna
  Serial.setTimeout(10);
  while (!Serial) {};  //czekanie do nawiązania komunikacji z komputerem  
  
#pragma endregion
}

// Zmienne globalne
unsigned long aktualnyCzas = 0;

void loop()
{
#pragma region KomunikacjaPodstawowa
  poczatekLoop:
  aktualnyCzas = millis();

  while (!Serial.available()) {
    motor1.update();    //WAŻNE!! <- aktualizuje stan silniczka w oczekiwaniu na ramke 

    if(millis() - aktualnyCzas > 1000)
    {
      Serial.write((char*)TablicaDoPC, 20);

      aktualnyCzas = millis();
    }
  }

  Serial.readBytes((char*)TablicaZPC, 52);    // Odczyt danych z MaSzyny
  Serial.write((char*)TablicaDoPC, 20);       // Wysłanie danych do MaMszyny

  if(TablicaZPC[0] != 0xEF || TablicaZPC[1] != 0xEF || TablicaZPC[2] != 0xEF || TablicaZPC[3] != 0xEF)
    goto poczatekLoop;
#pragma endregion

  motor1.setPosition(map(word(TablicaZPC[14], TablicaZPC[13]), 0, 2047, 0, STEPS));

  motor1.update();
}