#include <SwitecX25.h>

#define STEPS (315*6)
#define GuzikWymuszeniaKomunikacji 13

SwitecX25 motor1(STEPS,62,63,64,65);

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

void setup() {
  // run the motor against the stops
  motor1.zero();
  // start moving towards the center of the range
  //  motor1.setPosition(STEPS/2);

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

#pragma region ZmienneGlobalne
bool _blockGuzikWymuszeniaKomunikacji = true; // awaryjne wymuszenie komunikacji
#pragma endregion

void loop() {  
  
#pragma region KomunikacjaZasadnicza
  // Send/refresh data
  while (!Serial.available()) {
    motor1.update();
    
    if(!(digitalRead(GuzikWymuszeniaKomunikacji))) 
    {
      if(_blockGuzikWymuszeniaKomunikacji)
        Serial.write((char*)TablicaDoPC, 20);	
      
      _blockGuzikWymuszeniaKomunikacji = false;
    }
    else _blockGuzikWymuszeniaKomunikacji = true;
  }

  Serial.readBytes((char*)TablicaZPC, 52);		// Read data from PC
  Serial.write((char*)TablicaDoPC, 20);		// Send data from controller to PC

  // Sprawdzenie czy nie gubie preambuły
  int PrzesunieciePreambuly = 0;
  if(!(TablicaZPC[0] == 0xEF && TablicaZPC[1] == 0xEF && TablicaZPC[2] == 0xEF && TablicaZPC[3] == 0xEF))
  {
    if(TablicaZPC[0] == 0xEF && TablicaZPC[1] == 0xEF && TablicaZPC[2] == 0xEF)
    {
      PrzesunieciePreambuly = 1;
    }
    else
    {
      if(TablicaZPC[0] == 0xEF && TablicaZPC[1] == 0xEF)
      {
        PrzesunieciePreambuly = 2;
      }
      else
      {
        if(TablicaZPC[0] == 0xEF)
        {
          PrzesunieciePreambuly = 3;
        }
      }
    }
  }
#pragma endregion

  motor1.setPosition(map(((TablicaZPC[14 - PrzesunieciePreambuly] << 8) + TablicaZPC[13 - PrzesunieciePreambuly]), 0, 2047, 0, STEPS));
  motor1.update();

}