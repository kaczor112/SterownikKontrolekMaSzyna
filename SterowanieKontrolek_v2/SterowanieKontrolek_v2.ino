#include <Wire.h>

// Definicje lapmp sygnałowych
#define LampaSHP 13
#define LampaCzuwak 12
#define LampaJazdyNaOporach 11
//#define LampaWylacznikaSzybkiego 10

// Definicja przełączników
//#define PrzelacznikBateria 3
//#define PrzelacznikPantografPrzod 5
//#define PrzelacznikPantografTyl 6
//#define PrzelacznikPrzetwornicy 10
//#define PrzelacznikSprezarki 11

// Definicje guzików
#define GuzikWymuszeniaKomunikacji 3
//#define GuzikZwiekszeniePozycjiNastawnikaKierunku 2
#define GuzikCzuwakSHP 2
//#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow 7
//#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoPrzetwornicy 8
//#define GuzikWylacznikaSzybkiego 9
//#define AddMasterController A5
//#define SubtrackMasterController A4
//#define AddSecondController A3
//#define SubtrackSecondController A2

// Definicje potencjometru
#define PotencjometrTrainBrake A5
#define PotencjometrIndependentBrake A4

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

void setup() {
  // Konfiguracja lamp
  pinMode(LampaSHP, OUTPUT);    //ustawienie pinu 13 (wbudowana w Arduino dioda LED) jako wyjście cyfrowe
  digitalWrite(LampaSHP, LOW);  //ustawienie stanu niskiego na pinie 13
  pinMode(LampaCzuwak, OUTPUT);
  digitalWrite(LampaCzuwak, LOW);
  pinMode(LampaJazdyNaOporach, OUTPUT);
  digitalWrite(LampaJazdyNaOporach, LOW);
  //pinMode(LampaWylacznikaSzybkiego, OUTPUT);
  //digitalWrite(LampaWylacznikaSzybkiego, LOW);

  // Konfiguracja guzików i przełączników
  // INPUT_PULLUP - ustawienie pinu 2 jako wejście z podciągnięciem do zasilania
  pinMode(GuzikWymuszeniaKomunikacji, INPUT_PULLUP);
  //pinMode(GuzikZwiekszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);
  //pinMode(PrzelacznikBateria, INPUT_PULLUP);
  pinMode(GuzikCzuwakSHP, INPUT_PULLUP);
  /*(pinMode(PrzelacznikPantografPrzod, INPUT_PULLUP);
  pinMode(PrzelacznikPantografTyl, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoPrzetwornicy, INPUT_PULLUP);
  pinMode(GuzikWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(PrzelacznikPrzetwornicy, INPUT_PULLUP);
  pinMode(PrzelacznikSprezarki, INPUT_PULLUP);
  pinMode(AddMasterController, INPUT_PULLUP);
  pinMode(SubtrackMasterController, INPUT_PULLUP);
  pinMode(AddSecondController, INPUT_PULLUP);
  pinMode(SubtrackSecondController, INPUT_PULLUP);*/

  // Inicjalizacja preambuły
  TablicaDoPC[0] = 0xEF;
  TablicaDoPC[1] = 0xEF;
  TablicaDoPC[2] = 0xEF;
  TablicaDoPC[3] = 0xEF;

  // Konfiguracja komunikacji
  Serial.begin(115200);  //nawiązanie komunikacji z MaSzyna
  Serial.setTimeout(10);
  while (!Serial) {};  //czekanie do nawiązania komunikacji z komputerem
}

#pragma region ZmienneGlobalne

bool _blockAddMasterController = true;  // nastawnik
bool _blockSubtrackMasterController = true;
int currentPositionMasterController = 0;

bool _blockAddSecondController = true;  // bocznik
bool _blockSubtrackSecondController = true;
int currentPositionSecondController = 0;

bool _blockGuzikWymuszeniaKomunikacji = true; // awaryjne wymuszenie komunikacji

int prevPotencjometrTrainBrake = 0; // Hamulec glowny
int prevPotencjometrIndependentBrake = 0; // Hamulec lokomotywy
#pragma endregion

void loop() {
  // Send/refresh data
  while (!Serial.available()) {
      if (!(digitalRead(GuzikWymuszeniaKomunikacji))) 
      {
        if(_blockGuzikWymuszeniaKomunikacji)
          Serial.write((char*)TablicaDoPC, 20);	
        
        _blockGuzikWymuszeniaKomunikacji = false;
      }
      else _blockGuzikWymuszeniaKomunikacji = true;
    }

  Serial.readBytes((char*)TablicaZPC, 52);		// Read data from PC
  Serial.write((char*)TablicaDoPC, 20);		// Send data from controller to PC

  int PrzesunieciePreambuly = 0;
  // Sprawdzenie czy nie gubie preambuły
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
        else
        {
          digitalWrite(LampaJazdyNaOporach, HIGH);
          digitalWrite(LampaSHP, HIGH);
          digitalWrite(LampaCzuwak, HIGH);
        }
      }
    }
  }

#pragma region UstawianieLamp
  digitalWrite(LampaJazdyNaOporach, (bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 1)));
  digitalWrite(LampaSHP, (bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 7)));
  digitalWrite(LampaCzuwak, (bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 6)));
  //digitalWrite(LampaWylacznikaSzybkiego, (bitRead(TablicaZPC[9], 5)));
#pragma endregion

  if (!(digitalRead(GuzikCzuwakSHP)))
    bitWrite(TablicaDoPC[4], 7, 1);
  else bitWrite(TablicaDoPC[4], 7, 0);
    
  int tempTrainBrake = analogRead(PotencjometrTrainBrake);
  if(((tempTrainBrake - prevPotencjometrTrainBrake) > 3) || ((prevPotencjometrTrainBrake - tempTrainBrake) > 3))
  {
    TablicaDoPC[12] = lowByte(tempTrainBrake);
    TablicaDoPC[13] = highByte(tempTrainBrake);
    prevPotencjometrTrainBrake = tempTrainBrake;
  }


  int tempIndependentBrake = analogRead(PotencjometrIndependentBrake);
  if(((tempIndependentBrake - prevPotencjometrIndependentBrake) > 3) || ((prevPotencjometrIndependentBrake - tempIndependentBrake) > 3))
  {
    TablicaDoPC[14] = lowByte(tempIndependentBrake);
    TablicaDoPC[15] = highByte(tempIndependentBrake);
    prevPotencjometrIndependentBrake = tempIndependentBrake;
  }
  
  /*
  #pragma region Nastawnik
    if(!(digitalRead(AddMasterController)))
      {
        if(_blockAddMasterController)
        {
          currentPositionMasterController++;
          if(currentPositionMasterController > 43) currentPositionMasterController = 43;
          TablicaDoPC[6] = currentPositionMasterController;
          _blockAddMasterController = false;
        }
      }
      else _blockAddMasterController = true;
      
      if(!(digitalRead(SubtrackMasterController)))
      {
        if(_blockSubtrackMasterController)
        {
          currentPositionMasterController--;
          if(currentPositionMasterController < 0 ) currentPositionMasterController = 0;
          TablicaDoPC[6] = currentPositionMasterController;
          _blockSubtrackMasterController = false;
        }
      }
      else _blockSubtrackMasterController = true;
#pragma endregion
 
      if(!(digitalRead(GuzikZwiekszeniePozycjiNastawnikaKierunku)))
        bitWrite(TablicaDoPC[0], 0, 1);
      else bitWrite(TablicaDoPC[0], 0, 0); 
      
      if(!(digitalRead(PrzelacznikBateria)))
        bitWrite(TablicaDoPC[3], 0, 1);
      else bitWrite(TablicaDoPC[3], 0, 0);
      
#pragma region Bocznik
    if(!(digitalRead(AddSecondController)))
      {
        if(_blockAddSecondController)
        {
          currentPositionSecondController++;
          if(currentPositionSecondController > 7) currentPositionSecondController = 7;
          TablicaDoPC[7] = currentPositionSecondController;
          _blockAddSecondController = false;
        }
      }
      else _blockAddSecondController = true;
      
      if(!(digitalRead(SubtrackSecondController)))
      {
        if(_blockSubtrackSecondController)
        {
          currentPositionSecondController--;
          if(currentPositionSecondController < 0 ) currentPositionSecondController = 0;
          TablicaDoPC[7] = currentPositionSecondController;
          _blockSubtrackSecondController = false;
        }
      }
      else _blockSubtrackSecondController = true;
#pragma endregion
      if(!(digitalRead(PrzelacznikPantografPrzod)))
        bitWrite(TablicaDoPC[2], 0, 1);
      else bitWrite(TablicaDoPC[2], 0, 0); 
      
      if(!(digitalRead(PrzelacznikPantografTyl)))
        bitWrite(TablicaDoPC[2], 1, 1);
      else bitWrite(TablicaDoPC[2], 1, 0); 
      
      if(!(digitalRead(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow)))
        bitWrite(TablicaDoPC[0], 3, 1);
      else bitWrite(TablicaDoPC[0], 3, 0);
      
      if(!(digitalRead(GuzikOdblokowaniaPrzekaznikaNadmiarowegoPrzetwornicy)))
        bitWrite(TablicaDoPC[0], 5, 1);
      else bitWrite(TablicaDoPC[0], 5, 0); 
      
      if(!(digitalRead(GuzikWylacznikaSzybkiego)))
        bitWrite(TablicaDoPC[0], 1, 1);
      else bitWrite(TablicaDoPC[0], 1, 0); 
      
      if(!(digitalRead(PrzelacznikPrzetwornicy)))
        bitWrite(TablicaDoPC[1], 1, 1);
      else bitWrite(TablicaDoPC[1], 1, 0); 
      
      if(!(digitalRead(PrzelacznikSprezarki)))
        bitWrite(TablicaDoPC[1], 2, 1);
      else bitWrite(TablicaDoPC[1], 2, 0); 
*/

  // if(!(digitalRead(PrzelacznikBateria)))      // jeśli wejście na pinie 2 ma stan niski (zwarte przycikiem do masy)
  //   bitWrite(TablicaDoPC[3], 0, 1);           // ustawienie bitu 1 w bajcie 3 w TablicaDoPC na 1 ( zał. baterii)
  // else bitWrite(TablicaDoPC[3], 0, 0);        // w przeciwnym przypadku na 0
}