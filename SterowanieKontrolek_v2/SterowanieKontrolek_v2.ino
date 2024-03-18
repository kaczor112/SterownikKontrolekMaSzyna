#include <ArduinoFrameCRC.h>

#pragma region DefinicjeLamp
// Definicje lapmp sygnałowych
#define LampaSHP 23
#define LampaCzuwak 25
#define LampaVentilatorOverload 27
#define LampaMotorOverloadThreshold 29
#define LampaBattery 31
#define LampaTrainHeating 33
#define LampaMotorResistors 35
#define LampaWheelSlip 37
#define LampaMotorConnectors  39
#define LampaConverterOverload 41
#define LampaGroundRelay 43
#define LampaMotorOverload  45
#define LampaLineBreaker 47
#define LampaCompressorOverload  49
#define LampaRadioStop 51
#pragma endregion

#pragma region DefinicjeGuzikow
// Definicje guzików i przełączników
#define GuzikWymuszeniaKomunikacji 13
#define GuzikAktywacjaSyrenyWysokotonowej 22
#define GuzikAktywacjaSyrenyNiskotonowej 26
#define GuzikCzuwakSHP 24
#define GuzikOtwarcieWylacznikaSzybkiego 28
#define GuzikZamkniecieWylacznikaSzybkiego 30
#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow 32
#define GuzikOdblokowaniePrzekaznikaNadmiarowegoPrzetwornicy 34
#define GuzikRozlaczenieStycznikowLiniowych 36
#define GuzikZalaczeniePrzetwornicy 38
#define GuzikZalaczenieSprezarki 40
//#define GuzikAktywacjaPiasecznicy 42
#define GuzikZalaczenieOgrzewaniaPociagu 42
#define GuzikUstawienieRozruchuNiskiego 46
#define GuzikPodniesieniePantografuPrzedniego 48
#define GuzikPodniesieniePantografuTylnego 50
#define GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego 52
#define GuzikZalaczeniePrzyciemnieniaReflektorow 44
//#define GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny 19
#define GuzikOdluzniacz 53
#define GuzikBateria 66
#define GuzikZalaczenieLewegoReflektora 67
#define GuzikZalaczenieGornegoReflektora 68
#define GuzikZalaczeniePrawegoReflektora 69
#define GuzikTrybuHamulcaDlaPociaguTowarowego 18
#define GuzikTrybuHamulcaDlaPociaguPospiesznego 19

//Dotyczy nastawnika kierunku
#define GuzikZwiekszeniePozycjiNastawnikaKierunku 64
#define GuzikZmniejszeniePozycjiNastawnikaKierunku 65

// Definicje bloku nastawnika
#define AddShiftMasterController 54
#define AddMasterController 55
#define SubtrackMasterController 56
#define SubtrackShiftMasterController 57

#define AddShiftSecondController 58
#define AddSecondController 59
#define SubtrackSecondController 60
#define SubtrackShiftSecondController 61

// Definicje potencjometru
#define PotencjometrTrainBrake A8
#define PotencjometrIndependentBrake A9
#pragma endregion

#pragma region DefinicjePWM
#define PWM_LV_current 12
#define PWM_LV_voltage 11
#define PWM_HV_current_1 10
#define PWM_HV_current_2 9
#define PWM_HV_voltage 8
#define PWM_BreakPress 7
#define PWM_PipePress 6
#define PWM_TankPress 5
#pragma endregion

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

void setup() {
#pragma region InicjalizacjaLamp
  // Konfiguracja lamp
  pinMode(LampaSHP, OUTPUT);
  digitalWrite(LampaSHP, LOW);
  pinMode(LampaCzuwak, OUTPUT);
  digitalWrite(LampaCzuwak, LOW);
  pinMode(LampaVentilatorOverload, OUTPUT);
  digitalWrite(LampaVentilatorOverload, LOW);
  pinMode(LampaMotorOverloadThreshold, OUTPUT);
  digitalWrite(LampaMotorOverloadThreshold, LOW);
  pinMode(LampaBattery, OUTPUT);
  digitalWrite(LampaBattery, LOW);
  pinMode(LampaTrainHeating, OUTPUT);
  digitalWrite(LampaTrainHeating, LOW);
  pinMode(LampaMotorResistors, OUTPUT);
  digitalWrite(LampaMotorResistors, LOW);
  pinMode(LampaWheelSlip, OUTPUT);
  digitalWrite(LampaWheelSlip, LOW);
  pinMode(LampaMotorConnectors, OUTPUT);
  digitalWrite(LampaMotorConnectors, LOW);
  pinMode(LampaConverterOverload, OUTPUT);
  digitalWrite(LampaConverterOverload, LOW);
  pinMode(LampaGroundRelay, OUTPUT);
  digitalWrite(LampaGroundRelay, LOW);
  pinMode(LampaMotorOverload, OUTPUT);
  digitalWrite(LampaMotorOverload, LOW);
  pinMode(LampaLineBreaker, OUTPUT);
  digitalWrite(LampaLineBreaker, LOW);
  pinMode(LampaCompressorOverload, OUTPUT);
  digitalWrite(LampaCompressorOverload, LOW);
  pinMode(LampaRadioStop, OUTPUT);
  digitalWrite(LampaRadioStop, LOW);
#pragma endregion

#pragma region InicjalizacjaGuzikow
  // Konfiguracja guzików i przełączników
  // INPUT_PULLUP - ustawienie pinu 2 jako wejście z podciągnięciem do zasilania
  pinMode(GuzikWymuszeniaKomunikacji, INPUT_PULLUP);
  pinMode(GuzikAktywacjaSyrenyWysokotonowej, INPUT_PULLUP);
  pinMode(GuzikAktywacjaSyrenyNiskotonowej, INPUT_PULLUP);
  pinMode(GuzikCzuwakSHP, INPUT_PULLUP);
  pinMode(GuzikOtwarcieWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(GuzikZamkniecieWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniePrzekaznikaNadmiarowegoPrzetwornicy, INPUT_PULLUP);
  pinMode(GuzikRozlaczenieStycznikowLiniowych, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrzetwornicy, INPUT_PULLUP);
  pinMode(GuzikZalaczenieSprezarki, INPUT_PULLUP);
 // pinMode(GuzikAktywacjaPiasecznicy, INPUT_PULLUP);
  pinMode(GuzikZalaczenieOgrzewaniaPociagu, INPUT_PULLUP);
  pinMode(GuzikUstawienieRozruchuNiskiego, INPUT_PULLUP);
  pinMode(GuzikPodniesieniePantografuPrzedniego, INPUT_PULLUP);
  pinMode(GuzikPodniesieniePantografuTylnego, INPUT_PULLUP);
  pinMode(GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrzyciemnieniaReflektorow, INPUT_PULLUP);
//  pinMode(GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny, INPUT_PULLUP);
  pinMode(GuzikOdluzniacz, INPUT_PULLUP);
  pinMode(GuzikBateria, INPUT_PULLUP);
  pinMode(GuzikZalaczenieLewegoReflektora, INPUT_PULLUP);
  pinMode(GuzikZalaczenieGornegoReflektora, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrawegoReflektora, INPUT_PULLUP);
  pinMode(AddShiftMasterController, INPUT_PULLUP);
  pinMode(AddMasterController, INPUT_PULLUP);
  pinMode(SubtrackMasterController, INPUT_PULLUP);
  pinMode(SubtrackShiftMasterController, INPUT_PULLUP);
  pinMode(AddShiftSecondController, INPUT_PULLUP);
  pinMode(AddSecondController, INPUT_PULLUP);
  pinMode(SubtrackSecondController, INPUT_PULLUP);
  pinMode(SubtrackShiftSecondController, INPUT_PULLUP);
  pinMode(GuzikZwiekszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);
  pinMode(GuzikZmniejszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);
  pinMode(GuzikTrybuHamulcaDlaPociaguTowarowego, INPUT_PULLUP);
  pinMode(GuzikTrybuHamulcaDlaPociaguPospiesznego, INPUT_PULLUP);
#pragma endregion

#pragma region InicjalizacjaPWM
  pinMode(PWM_LV_current, OUTPUT);
  pinMode(PWM_LV_voltage, OUTPUT);
  pinMode(PWM_HV_current_1, OUTPUT);
  pinMode(PWM_HV_current_2, OUTPUT);
  pinMode(PWM_HV_voltage, OUTPUT);
  pinMode(PWM_BreakPress, OUTPUT);
  pinMode(PWM_PipePress, OUTPUT);
  pinMode(PWM_TankPress, OUTPUT);
#pragma endregion

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
  
  Serial3.begin(9600);  // Hasler
  Serial3.setTimeout(10);
#pragma endregion
}

#pragma region ZmienneGlobalne

bool _blockAddMasterController = true;  // nastawnik
bool _blockSubtrackMasterController = true;
int currentPositionMasterController = 0;

bool _blockAddSecondController = true;  // bocznik
bool _blockSubtrackSecondController = true;
int currentPositionSecondController = 0;

bool _blockGuzikWymuszeniaKomunikacji = true; // awaryjne wymuszenie komunikacji

ArduinoFrameCRC haslerFrame;
#pragma endregion

void loop() {  
#pragma region KomunikacjaZasadnicza
  // Send/refresh data
  while (!Serial.available()) {
    SprawdzNastawnikBocznik();

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
        else
        {
          digitalWrite(LampaSHP, LOW);
          digitalWrite(LampaCzuwak, LOW);
          digitalWrite(LampaVentilatorOverload, LOW);
          digitalWrite(LampaMotorOverloadThreshold, LOW);
          digitalWrite(LampaBattery, LOW);
          digitalWrite(LampaTrainHeating, LOW);
          digitalWrite(LampaMotorResistors, LOW);
          digitalWrite(LampaWheelSlip, LOW);
          digitalWrite(LampaMotorConnectors, LOW);
          digitalWrite(LampaConverterOverload, LOW);
          digitalWrite(LampaGroundRelay, LOW);
          digitalWrite(LampaMotorOverload, LOW);
          digitalWrite(LampaLineBreaker, LOW);
          digitalWrite(LampaCompressorOverload, LOW);
          digitalWrite(LampaRadioStop, LOW);
          delay(3000);          
        }
      }
    }
  }
#pragma endregion

#pragma region UstawianieLamp
  digitalWrite(LampaSHP, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 7)));
  digitalWrite(LampaCzuwak, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 6)));
  digitalWrite(LampaVentilatorOverload, !(bitRead(TablicaZPC[6 - PrzesunieciePreambuly], 1)));
  digitalWrite(LampaMotorOverloadThreshold, !(bitRead(TablicaZPC[6 - PrzesunieciePreambuly], 2)));
  digitalWrite(LampaBattery, !(bitRead(TablicaZPC[7 - PrzesunieciePreambuly], 6)));
  digitalWrite(LampaTrainHeating, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 0)));
  digitalWrite(LampaMotorResistors, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 1)));
  digitalWrite(LampaWheelSlip, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 2)));
  digitalWrite(LampaMotorConnectors, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 0)));
  digitalWrite(LampaConverterOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 2)));
  digitalWrite(LampaGroundRelay, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 3)));
  digitalWrite(LampaMotorOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 4)));
  digitalWrite(LampaLineBreaker, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 5)));
  digitalWrite(LampaCompressorOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 6)));
  digitalWrite(LampaRadioStop, !(bitRead(TablicaZPC[10 - PrzesunieciePreambuly], 5)));
#pragma endregion

#pragma region OdczytGuzikow
  if (!(digitalRead(GuzikAktywacjaSyrenyWysokotonowej)))
    bitWrite(TablicaDoPC[6], 6, 1);
  else bitWrite(TablicaDoPC[6], 6, 0);
  
  if (!(digitalRead(GuzikAktywacjaSyrenyNiskotonowej)))
    bitWrite(TablicaDoPC[6], 7, 1);
  else bitWrite(TablicaDoPC[6], 7, 0);
  
  if (!(digitalRead(GuzikCzuwakSHP)))
    bitWrite(TablicaDoPC[4], 7, 1);
  else bitWrite(TablicaDoPC[4], 7, 0);
  
  if (!(digitalRead(GuzikOtwarcieWylacznikaSzybkiego)))
    bitWrite(TablicaDoPC[4], 1, 1);
  else bitWrite(TablicaDoPC[4], 1, 0);
  
  if (!(digitalRead(GuzikZamkniecieWylacznikaSzybkiego)))
    bitWrite(TablicaDoPC[4], 2, 1);
  else bitWrite(TablicaDoPC[4], 2, 0);
  
  if (!(digitalRead(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow)))
    bitWrite(TablicaDoPC[4], 3, 1);
  else bitWrite(TablicaDoPC[4], 3, 0);
  
  if (!(digitalRead(GuzikOdblokowaniePrzekaznikaNadmiarowegoPrzetwornicy)))
    bitWrite(TablicaDoPC[4], 5, 1);
  else bitWrite(TablicaDoPC[4], 5, 0);
  
  if (!(digitalRead(GuzikRozlaczenieStycznikowLiniowych)))
    bitWrite(TablicaDoPC[4], 6, 1);
  else bitWrite(TablicaDoPC[4], 6, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrzetwornicy)))
    bitWrite(TablicaDoPC[5], 1, 1);
  else bitWrite(TablicaDoPC[5], 1, 0);
  
  if (!(digitalRead(GuzikZalaczenieSprezarki)))
    bitWrite(TablicaDoPC[5], 2, 1);
  else bitWrite(TablicaDoPC[5], 2, 0);
  
//  if (!(digitalRead(GuzikAktywacjaPiasecznicy)))
//    bitWrite(TablicaDoPC[5], 3, 1);
//  else bitWrite(TablicaDoPC[5], 3, 0);
  
  if (!(digitalRead(GuzikZalaczenieOgrzewaniaPociagu)))
    bitWrite(TablicaDoPC[5], 4, 1);
  else bitWrite(TablicaDoPC[5], 4, 0);
  
  if (!(digitalRead(GuzikUstawienieRozruchuNiskiego)))
    bitWrite(TablicaDoPC[5], 7, 1);
  else bitWrite(TablicaDoPC[5], 7, 0);
  
  if (!(digitalRead(GuzikPodniesieniePantografuPrzedniego)))
    bitWrite(TablicaDoPC[6], 0, 1);
  else bitWrite(TablicaDoPC[6], 0, 0);
  
  if (!(digitalRead(GuzikPodniesieniePantografuTylnego)))
    bitWrite(TablicaDoPC[6], 1, 1);
  else bitWrite(TablicaDoPC[6], 1, 0);
  
  if (!(digitalRead(GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego)))
    bitWrite(TablicaDoPC[6], 2, 1);
  else bitWrite(TablicaDoPC[6], 2, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrzyciemnieniaReflektorow)))
    bitWrite(TablicaDoPC[6], 3, 1);
  else bitWrite(TablicaDoPC[6], 3, 0);
  
//  if (!(digitalRead(GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny)))
//    bitWrite(TablicaDoPC[6], 4, 1);
//  else bitWrite(TablicaDoPC[6], 4, 0);
  
  if (!(digitalRead(GuzikOdluzniacz)))
    bitWrite(TablicaDoPC[6], 5, 1);
  else bitWrite(TablicaDoPC[6], 5, 0);
  
  if (!(digitalRead(GuzikBateria)))
    bitWrite(TablicaDoPC[7], 0, 1);
  else bitWrite(TablicaDoPC[7], 0, 0);
  
  if (!(digitalRead(GuzikZalaczenieLewegoReflektora)))
    bitWrite(TablicaDoPC[7], 1, 1);
  else bitWrite(TablicaDoPC[7], 1, 0);
  
  if (!(digitalRead(GuzikZalaczenieGornegoReflektora)))
    bitWrite(TablicaDoPC[7], 2, 1);
  else bitWrite(TablicaDoPC[7], 2, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrawegoReflektora)))
    bitWrite(TablicaDoPC[7], 3, 1);
  else bitWrite(TablicaDoPC[7], 3, 0);
  
  if (!(digitalRead(GuzikZwiekszeniePozycjiNastawnikaKierunku)))
    bitWrite(TablicaDoPC[7], 4, 1);
  else bitWrite(TablicaDoPC[7], 4, 0);
  
  if (!(digitalRead(GuzikZmniejszeniePozycjiNastawnikaKierunku)))
    bitWrite(TablicaDoPC[7], 5, 1);
  else bitWrite(TablicaDoPC[7], 5, 0);

  if(!(digitalRead(GuzikTrybuHamulcaDlaPociaguTowarowego)))
  {
    bitWrite(TablicaDoPC[7], 6, 1);
    bitWrite(TablicaDoPC[7], 7, 0);
    bitWrite(TablicaDoPC[8], 0, 0);
  }
  else
  {
    if(!(digitalRead(GuzikTrybuHamulcaDlaPociaguPospiesznego)))
    {
      bitWrite(TablicaDoPC[7], 6, 0);
      bitWrite(TablicaDoPC[7], 7, 0);
      bitWrite(TablicaDoPC[8], 0, 1);
    }
    else
    {
      bitWrite(TablicaDoPC[7], 6, 0);
      bitWrite(TablicaDoPC[7], 7, 1);
      bitWrite(TablicaDoPC[8], 0, 0);      
    }
  }
#pragma endregion
    
#pragma region OdczytHamulcow
  int tempTrainBrake = analogRead(PotencjometrTrainBrake);
  TablicaDoPC[12] = lowByte(tempTrainBrake);
  TablicaDoPC[13] = highByte(tempTrainBrake);

  int tempIndependentBrake = analogRead(PotencjometrIndependentBrake);
  TablicaDoPC[14] = lowByte(tempIndependentBrake);
  TablicaDoPC[15] = highByte(tempIndependentBrake);
#pragma endregion
  
#pragma region UstawienieSygnalowPWM
  analogWrite(PWM_LV_voltage, TablicaZPC[35 - PrzesunieciePreambuly]);
  analogWrite(PWM_HV_current_1, TablicaZPC[19 - PrzesunieciePreambuly]);
  analogWrite(PWM_HV_current_2, TablicaZPC[21 - PrzesunieciePreambuly]);
  analogWrite(PWM_HV_voltage, TablicaZPC[17 - PrzesunieciePreambuly]);
  analogWrite(PWM_BreakPress, TablicaZPC[11 - PrzesunieciePreambuly]);
  analogWrite(PWM_PipePress, TablicaZPC[13 - PrzesunieciePreambuly]);
  analogWrite(PWM_TankPress, TablicaZPC[15 - PrzesunieciePreambuly]);

  // Sztuczne ustawienie sygnału PWM_LV_current
  if(!(digitalRead(GuzikZalaczeniePrzetwornicy)) && !(digitalRead(GuzikBateria)))
  {
    analogWrite(PWM_LV_current, 200);
  }
  else
  {
    analogWrite(PWM_LV_current, 127);
  }  

  // Zerowanie po wyłączeniu programu
  if((TablicaZPC[11 - PrzesunieciePreambuly] == 0) 
  && (TablicaZPC[13 - PrzesunieciePreambuly] == 0) 
  && (TablicaZPC[15 - PrzesunieciePreambuly] == 0))
  {
    analogWrite(PWM_LV_current, 0);
  }
#pragma endregion

#pragma region UstawienieHaslera
  if(Serial3)
  {
    if(Serial3.available())
    {
      uint8_t tableHasler[30] = { 0 };
      Serial3.readBytes((char*)tableHasler, 30);
    }

    haslerFrame.Reset();
    haslerFrame.AddData(TablicaZPC[4 - PrzesunieciePreambuly]);
    haslerFrame.CreateFrame();
    
    Serial3.write(haslerFrame.fullFrame, haslerFrame.lengthFrame);
    //Serial3.write(TablicaZPC[4 - PrzesunieciePreambuly]);
  }
#pragma endregion

  SprawdzNastawnikBocznik();
}

void SprawdzNastawnikBocznik()
{
#pragma region Nastawnik
  if(!(digitalRead(AddMasterController)))
  {
    if(_blockAddMasterController)
    {
      currentPositionMasterController++;
      if(currentPositionMasterController > 43) currentPositionMasterController = 43;
      TablicaDoPC[10] = currentPositionMasterController;
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
      TablicaDoPC[10] = currentPositionMasterController;
      _blockSubtrackMasterController = false;
    }
  }
  else _blockSubtrackMasterController = true;
  
  if(!(digitalRead(AddShiftMasterController)))
  {
    currentPositionMasterController = 43;
    TablicaDoPC[10] = currentPositionMasterController;
  }
  
  if(!(digitalRead(SubtrackShiftMasterController)))
  {
    currentPositionMasterController = 0;
    TablicaDoPC[10] = currentPositionMasterController;
  }
#pragma endregion

#pragma region Bocznik
  if(!(digitalRead(AddSecondController)))
  {
    if(_blockAddSecondController)
    {
      currentPositionSecondController++;
      if(currentPositionSecondController > 7) currentPositionSecondController = 7;
      TablicaDoPC[11] = currentPositionSecondController;
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
      TablicaDoPC[11] = currentPositionSecondController;
      _blockSubtrackSecondController = false;
    }
  }
  else _blockSubtrackSecondController = true;
    
  if(!(digitalRead(AddShiftSecondController)))
  {
    currentPositionSecondController = 7;
    TablicaDoPC[11] = currentPositionSecondController;
  }
  
  if(!(digitalRead(SubtrackShiftSecondController)))
  {
    currentPositionSecondController = 0;
    TablicaDoPC[11] = currentPositionSecondController;
  }
#pragma endregion
}