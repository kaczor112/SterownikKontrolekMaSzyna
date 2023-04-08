// Definicje lapmp sygnałowych
//#define LampaSHP 13
#define LampaCzuwak 13
//#define LampaJazdyNaOporach 11
//#define LampaWylacznikaSzybkiego 10

// Definicja przełączników 
#define PrzelacznikBateria 3
#define PrzelacznikPantografPrzod 5
#define PrzelacznikPantografTyl 6
#define PrzelacznikPrzetwornicy 10
#define PrzelacznikSprezarki 11

// Definicje guzików
#define GuzikZwiekszeniePozycjiNastawnikaKierunku 2
#define GuzikCzuwakSHP 4
#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow 7
#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoPrzetwornicy 8
#define GuzikWylacznikaSzybkiego 9
#define AddMasterController A5
#define SubtrackMasterController A4
#define AddSecondController A3
#define SubtrackSecondController A2

// Definicje potencjometru
//#define PotencjometrTrainBrake A5
//#define PotencjometrIndependentBrake A4


void setup() {
  // Konfiguracja lamp
  //pinMode(LampaSHP, OUTPUT);     //ustawienie pinu 13 (wbudowana w Arduino dioda LED) jako wyjście cyfrowe
  //digitalWrite(LampaSHP, LOW);     //ustawienie stanu niskiego na pinie 13
  pinMode(LampaCzuwak, OUTPUT);
  digitalWrite(LampaCzuwak, LOW);
  //pinMode(LampaJazdyNaOporach, OUTPUT);
  //digitalWrite(LampaJazdyNaOporach, LOW);
  //pinMode(LampaWylacznikaSzybkiego, OUTPUT);
  //digitalWrite(LampaWylacznikaSzybkiego, LOW);
  
  // Konfiguracja guzików i przełączników
  // INPUT_PULLUP - ustawienie pinu 2 jako wejście z podciągnięciem do zasilania
  pinMode(GuzikZwiekszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);     
  pinMode(PrzelacznikBateria, INPUT_PULLUP);     
  pinMode(GuzikCzuwakSHP, INPUT_PULLUP);
  pinMode(PrzelacznikPantografPrzod, INPUT_PULLUP);
  pinMode(PrzelacznikPantografTyl, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoPrzetwornicy, INPUT_PULLUP);
  pinMode(GuzikWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(PrzelacznikPrzetwornicy, INPUT_PULLUP);
  pinMode(PrzelacznikSprezarki, INPUT_PULLUP);
  pinMode(AddMasterController, INPUT_PULLUP);
  pinMode(SubtrackMasterController, INPUT_PULLUP);
  pinMode(AddSecondController, INPUT_PULLUP);
  pinMode(SubtrackSecondController, INPUT_PULLUP);

  // Konfiguracja komunikacji
  Serial.begin(9600);           //nawiązanie komunikacji z MaSzyna
  Serial.setTimeout(10);
  while(!Serial){};             //czekanie do nawiązania komunikacji z komputerem
}

#pragma region ZmienneGlobalne
volatile uint8_t TablicaZPC[52] = {0};
volatile uint8_t TablicaDoPC[16] = {0};

bool _blockAddMasterController = true;      // nastawnik
bool _blockSubtrackMasterController = true;
int currentPositionMasterController = 0;

bool _blockAddSecondController = true;      // bocznik
bool _blockSubtrackSecondController = true;
int currentPositionSecondController = 0;
#pragma endregion

void loop() {
  for(byte i=0; i<52; i++){     //pętla umożliwiająca wysłanie 31 bajtów danych
   while(!Serial.available()){};     //czekamy na odebranie bajtu danych

   TablicaZPC[i] = Serial.read();     //zapis danych do naszej tablicy

   if(i < 20){   //zabezpiecznie, żeby nie wysłać za dużo danych do komputera
    Serial.flush();     //sprawczanie czy już wysłano wcześniejsze dane (czekanie na wysłanie danych)
    if(i < 4)
    {
      Serial.write(0xEF); // Preambuła
    }
    else
    {
      Serial.write(TablicaDoPC[i-4]);     // wysłanie danych do komputera
    }
   }

  }

  delay(10);

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

      if(!(digitalRead(GuzikCzuwakSHP)))
        bitWrite(TablicaDoPC[0], 7, 1);
      else bitWrite(TablicaDoPC[0], 7, 0); 
      
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

#pragma region UstawianieLamp
      //digitalWrite(LampaJazdyNaOporach, (bitRead(TablicaZPC[8], 1))); 
      //digitalWrite(LampaSHP, (bitRead(TablicaZPC[8], 7)));
      digitalWrite(LampaCzuwak, (bitRead(TablicaZPC[8], 6)));
      //digitalWrite(LampaWylacznikaSzybkiego, (bitRead(TablicaZPC[9], 5)));
#pragma endregion

  /*int tempTrainBrake = analogRead(PotencjometrTrainBrake);
  TablicaDoPC[8] = lowByte(tempTrainBrake);
  TablicaDoPC[9] = highByte(tempTrainBrake);

  
  int tempIndependentBrake = analogRead(PotencjometrIndependentBrake);
  TablicaDoPC[10] = lowByte(tempIndependentBrake);
  TablicaDoPC[11] = highByte(tempIndependentBrake);*/
  
 // if(!(digitalRead(PrzelacznikBateria)))      // jeśli wejście na pinie 2 ma stan niski (zwarte przycikiem do masy)
 //   bitWrite(TablicaDoPC[3], 0, 1);           // ustawienie bitu 1 w bajcie 3 w TablicaDoPC na 1 ( zał. baterii)
 // else bitWrite(TablicaDoPC[3], 0, 0);        // w przeciwnym przypadku na 0

  
}