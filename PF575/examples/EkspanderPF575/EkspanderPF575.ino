#include "PF575.h"

#define IntExp 13          // Pin na którym mam INT w ekspanderze

PF575 ekspander1(0x20, IntExp);     // Inicjuje ekspander o adresie 0x20

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

void setup()
{
  ekspander1.begin();  // ustawienie ustawień początkowych ekspandera
  
#pragma region InicjalizacjaKomunikacji
  // Inicjalizacja preambuły
  TablicaDoPC[0] = 0xEF;
  TablicaDoPC[1] = 0xEF;
  TablicaDoPC[2] = 0xEF;
  TablicaDoPC[3] = 0xEF;

  // Konfiguracja komunikacji
  Serial.begin(115200);    // Nawiązanie komunikacji z MaSzyna. 115200 to baud rate.
  Serial.setTimeout(10);
  while (!Serial) {};      // Czekanie do nawiązania komunikacji z komputerem
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
    ekspander1.update();    //WAŻNE!! <- aktualizuje stan ekspandera w oczekiwaniu na ramke 

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

  /* Opis obsługi biblioteki PF575
   * 
   * Mamy trzy konstruktory:
   * - PF575();  <- ustawia domyślny adres 0x20 oraz odświeża za każdym update();
   * - PF575(uint8_t address); <- ustawia adres oraz odświeża za każdym update();
   * - PF575(uint8_t address, uint8_t pinINT); <- ustawia adres oraz odświeża przy wystawieniu INT
   * W tym trzecim ustawiamy numer pinu na którym podłaczymy INT od ekspandera.
   *
   * W void setup() musimy wywołać begin();
   * 
   * W 'KomunikacjaPodstawowa' dodajemu update(); w pętli while (!Serial.available())
   *
   * W pętli zasadniczej też dodajemy update();
   */

  // Pod P00 mam podpiętą diodę. 
  ekspander1.setP00(!bitRead(TablicaZPC[8], 6));

  // Pod P10 odczytuje guzik
  bitWrite(TablicaDoPC[4], 0, !ekspander1.getP10());  

  ekspander1.update();
}