#include <ArduinoFrameCRC.h>
#include <SwitecX25.h>

#pragma region DefinicjeLamp
// Definicje lapmp sygnałowych
#define LampaSHP 54
#define LampaCzuwak 55
#define LampaMotorOverloadThreshold 56 // nadmiarowy silników
#define LampaMotorResistors 57 //motor resistors (JazdaNaOporach)
#define LampaConverterOverload 58 //converter overload (nadmiarowy przetwornicy)
#define LampaLineBreaker 59 //line breaker (WylacznikaSzybkiego)
#pragma endregion

#pragma region DefinicjeGuzikow
// Definicje guzików i przełączników
#define GuzikWymuszeniaKomunikacji 13
#define GuzikAktywacjaSyrenyWysokotonowej 22
#define GuzikAktywacjaSyrenyNiskotonowej 24
#define GuzikCzuwakSHP 26
// #define GuzikOtwarcieWylacznikaSzybkiego 28
#define GuzikZamkniecieWylacznikaSzybkiego 28
#define GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow 30
#define GuzikOdblokowanieWylaczeniePrzetwornicy 61
// #define GuzikRozlaczenieStycznikowLiniowych 36
#define GuzikWylaczeniePrzetwornicy 34 //A7
#define GuzikZalaczeniePrzetwornicy 32
#define GuzikZalaczenieSprezarki 36
//#define GuzikAktywacjaPiasecznicy 42
// #define GuzikZalaczenieOgrzewaniaPociagu 42
// #define GuzikUstawienieRozruchuNiskiego 46
#define GuzikPodniesieniePantografuPrzedniego 38
#define GuzikPodniesieniePantografuTylnego 40
#define GuzikOpuszczeniePantografuPrzedniego 42
#define GuzikOpuszczeniePantografuWszystkich 44
// #define GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego 52
#define GuzikZalaczeniePrzyciemnieniaReflektorow 46
//#define GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny 19
// #define GuzikOdluzniacz 53
// #define GuzikBateria 66
#define GuzikZalaczenieLewegoReflektora 48
#define GuzikZalaczenieGornegoReflektora 50
#define GuzikZalaczeniePrawegoReflektora 49

// Specyficzne dla kibla
#define GuzikOdblokowanieSprezarki 52
#define GuzikRadioTelefon 47
#define GuzikBuczek 45
#define GuzikDrzwiLewe 43
#define GuzikDrzwiPrawe 41

// koniec specyficznych dla kibla

// #define GuzikTrybuHamulcaDlaPociaguTowarowego 18
// #define GuzikTrybuHamulcaDlaPociaguPospiesznego 19

//Dotyczy nastawnika kierunku
// #define GuzikZwiekszeniePozycjiNastawnikaKierunku 64
// #define GuzikZmniejszeniePozycjiNastawnikaKierunku 65

// Definicje bloku nastawnika
// #define AddShiftMasterController 54
// #define AddMasterController 55
// #define SubtrackMasterController 56
// #define SubtrackShiftMasterController 57

// #define AddShiftSecondController 58
// #define AddSecondController 59
// #define SubtrackSecondController 60
// #define SubtrackShiftSecondController 61


#define MasterControllerAnalog A11 //65

// Definicje potencjometru
// #define PotencjometrTrainBrake A8
// #define PotencjometrIndependentBrake A9
#define TrainBrake_high 53 //A9
#define TrainBrake_low 51 //A8
#pragma endregion


#pragma region SilnikiKrokoweManometry
#define SilnikCylinder_A1 39
#define SilnikCylinder_A2 37
#define SilnikCylinder_B1 35
#define SilnikCylinder_B2 33

#define SilnikPrzewodGlowny_A1 31
#define SilnikPrzewodGlowny_A2 29
#define SilnikPrzewodGlowny_B1 27
#define SilnikPrzewodGlowny_B2 25

#define SilnikZbiornikGlowny_A1 69 // A15
#define SilnikZbiornikGlowny_A2 68
#define SilnikZbiornikGlowny_B1 67
#define SilnikZbiornikGlowny_B2 66 // A12

#define STEPS (315*6)
#define MaxScaleMaSzyna 2047

SwitecX25 motorCylinder(STEPS,
  SilnikCylinder_A1,
  SilnikCylinder_A2,
  SilnikCylinder_B1,
  SilnikCylinder_B2);

SwitecX25 motorPrzewodGlowny(STEPS,
  SilnikPrzewodGlowny_A1,
  SilnikPrzewodGlowny_A2,
  SilnikPrzewodGlowny_B1,
  SilnikPrzewodGlowny_B2);

SwitecX25 motorZbiornikGlowny(STEPS,
  SilnikZbiornikGlowny_A1,
  SilnikZbiornikGlowny_A2,
  SilnikZbiornikGlowny_B1,
  SilnikZbiornikGlowny_B2);

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
#pragma region InicjalizacjaSilnikowKrokowych
motorCylinder.zero();
motorPrzewodGlowny.zero();
motorZbiornikGlowny.zero();
#pragma endregion

#pragma region InicjalizacjaLamp
  // Konfiguracja lamp
  pinMode(LampaSHP, OUTPUT);
  digitalWrite(LampaSHP, LOW);
  pinMode(LampaCzuwak, OUTPUT);
  digitalWrite(LampaCzuwak, LOW);
  // pinMode(LampaVentilatorOverload, OUTPUT);
  // digitalWrite(LampaVentilatorOverload, LOW);
  pinMode(LampaMotorOverloadThreshold, OUTPUT);
  digitalWrite(LampaMotorOverloadThreshold, LOW);
  // pinMode(LampaBattery, OUTPUT);
  // digitalWrite(LampaBattery, LOW);
  // pinMode(LampaTrainHeating, OUTPUT);
  // digitalWrite(LampaTrainHeating, LOW);
  pinMode(LampaMotorResistors, OUTPUT);
  digitalWrite(LampaMotorResistors, LOW);
  // pinMode(LampaWheelSlip, OUTPUT);
  // digitalWrite(LampaWheelSlip, LOW);
  // pinMode(LampaMotorConnectors, OUTPUT);
  // digitalWrite(LampaMotorConnectors, LOW);
  pinMode(LampaConverterOverload, OUTPUT);
  digitalWrite(LampaConverterOverload, LOW);
  // pinMode(LampaGroundRelay, OUTPUT);
  // digitalWrite(LampaGroundRelay, LOW);
  // pinMode(LampaMotorOverload, OUTPUT);
  // digitalWrite(LampaMotorOverload, LOW);
  pinMode(LampaLineBreaker, OUTPUT);
  digitalWrite(LampaLineBreaker, LOW);
  // pinMode(LampaCompressorOverload, OUTPUT);
  // digitalWrite(LampaCompressorOverload, LOW);
  // pinMode(LampaRadioStop, OUTPUT);
  // digitalWrite(LampaRadioStop, LOW);
#pragma endregion

#pragma region InicjalizacjaGuzikow
  // Konfiguracja guzików i przełączników
  // INPUT_PULLUP - ustawienie pinu 2 jako wejście z podciągnięciem do zasilania
  pinMode(GuzikWymuszeniaKomunikacji, INPUT_PULLUP);
  pinMode(GuzikAktywacjaSyrenyWysokotonowej, INPUT_PULLUP);
  pinMode(GuzikAktywacjaSyrenyNiskotonowej, INPUT_PULLUP);
  pinMode(GuzikCzuwakSHP, INPUT_PULLUP);
  // pinMode(GuzikOtwarcieWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(GuzikZamkniecieWylacznikaSzybkiego, INPUT_PULLUP);
  pinMode(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow, INPUT_PULLUP);
  pinMode(GuzikOdblokowanieWylaczeniePrzetwornicy, INPUT_PULLUP);
  // pinMode(GuzikRozlaczenieStycznikowLiniowych, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrzetwornicy, INPUT_PULLUP);
  pinMode(GuzikZalaczenieSprezarki, INPUT_PULLUP);
  pinMode(GuzikWylaczeniePrzetwornicy, INPUT_PULLUP);
 // pinMode(GuzikAktywacjaPiasecznicy, INPUT_PULLUP);
  // pinMode(GuzikZalaczenieOgrzewaniaPociagu, INPUT_PULLUP);
  // pinMode(GuzikUstawienieRozruchuNiskiego, INPUT_PULLUP);
  pinMode(GuzikPodniesieniePantografuPrzedniego, INPUT_PULLUP);
  pinMode(GuzikPodniesieniePantografuTylnego, INPUT_PULLUP);
  pinMode(GuzikOpuszczeniePantografuPrzedniego, INPUT_PULLUP);
  pinMode(GuzikOpuszczeniePantografuWszystkich, INPUT_PULLUP);
  // pinMode(GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrzyciemnieniaReflektorow, INPUT_PULLUP);
//  pinMode(GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny, INPUT_PULLUP);
  // pinMode(GuzikOdluzniacz, INPUT_PULLUP);
  // pinMode(GuzikBateria, INPUT_PULLUP);
  pinMode(GuzikZalaczenieLewegoReflektora, INPUT_PULLUP);
  pinMode(GuzikZalaczenieGornegoReflektora, INPUT_PULLUP);
  pinMode(GuzikZalaczeniePrawegoReflektora, INPUT_PULLUP);
  // pinMode(AddShiftMasterController, INPUT_PULLUP);
  // pinMode(AddMasterController, INPUT_PULLUP);
  // pinMode(SubtrackMasterController, INPUT_PULLUP);
  // pinMode(SubtrackShiftMasterController, INPUT_PULLUP);

  // pinMode(AddShiftSecondController, INPUT_PULLUP);
  // pinMode(AddSecondController, INPUT_PULLUP);
  // pinMode(SubtrackSecondController, INPUT_PULLUP);
  // pinMode(SubtrackShiftSecondController, INPUT_PULLUP);
  // pinMode(GuzikZwiekszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);
  // pinMode(GuzikZmniejszeniePozycjiNastawnikaKierunku, INPUT_PULLUP);
  // pinMode(GuzikTrybuHamulcaDlaPociaguTowarowego, INPUT_PULLUP);
  // pinMode(GuzikTrybuHamulcaDlaPociaguPospiesznego, INPUT_PULLUP);

  pinMode(GuzikOdblokowanieSprezarki, INPUT_PULLUP);
  pinMode(GuzikRadioTelefon, INPUT_PULLUP);
  pinMode(GuzikBuczek, INPUT_PULLUP);
  pinMode(GuzikDrzwiLewe, INPUT_PULLUP);
  pinMode(GuzikDrzwiPrawe, INPUT_PULLUP);

  pinMode(TrainBrake_low, INPUT_PULLUP);
  pinMode(TrainBrake_high, INPUT_PULLUP);
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
  
  // Serial3.begin(9600);  // Hasler
  // Serial3.setTimeout(10);
#pragma endregion
}

#pragma region ZmienneGlobalne


bool _blockGuzikWymuszeniaKomunikacji = true; // awaryjne wymuszenie komunikacji

//ArduinoFrameCRC haslerFrame;
#pragma endregion

void loop() {  
#pragma region KomunikacjaZasadnicza
  // Send/refresh data
  while (!Serial.available()) {
    motorUpdate();

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
          // digitalWrite(LampaVentilatorOverload, LOW);
          digitalWrite(LampaMotorOverloadThreshold, LOW);
          // digitalWrite(LampaBattery, LOW);
          // digitalWrite(LampaTrainHeating, LOW);
          digitalWrite(LampaMotorResistors, LOW);
          // digitalWrite(LampaWheelSlip, LOW);
          // digitalWrite(LampaMotorConnectors, LOW);
          digitalWrite(LampaConverterOverload, LOW);
          // digitalWrite(LampaGroundRelay, LOW);
          // digitalWrite(LampaMotorOverload, LOW);
          digitalWrite(LampaLineBreaker, LOW);
          // digitalWrite(LampaCompressorOverload, LOW);
          // digitalWrite(LampaRadioStop, LOW);
          delay(3000);          
        }
      }
    }
  }
#pragma endregion

#pragma region UstawianieLamp
  digitalWrite(LampaSHP, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 7)));
  digitalWrite(LampaCzuwak, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 6)));
  // digitalWrite(LampaVentilatorOverload, !(bitRead(TablicaZPC[6 - PrzesunieciePreambuly], 1)));
  digitalWrite(LampaMotorOverloadThreshold, !(bitRead(TablicaZPC[6 - PrzesunieciePreambuly], 2)));
  // digitalWrite(LampaBattery, !(bitRead(TablicaZPC[7 - PrzesunieciePreambuly], 6)));
  // digitalWrite(LampaTrainHeating, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 0)));
  digitalWrite(LampaMotorResistors, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 1)));
  // digitalWrite(LampaWheelSlip, !(bitRead(TablicaZPC[8 - PrzesunieciePreambuly], 2)));
  // digitalWrite(LampaMotorConnectors, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 0)));
  digitalWrite(LampaConverterOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 2)));
  // digitalWrite(LampaGroundRelay, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 3)));
  // digitalWrite(LampaMotorOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 4)));
  digitalWrite(LampaLineBreaker, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 5)));
  // digitalWrite(LampaCompressorOverload, !(bitRead(TablicaZPC[9 - PrzesunieciePreambuly], 6)));
  // digitalWrite(LampaRadioStop, !(bitRead(TablicaZPC[10 - PrzesunieciePreambuly], 5)));
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
  
  // if (!(digitalRead(GuzikOtwarcieWylacznikaSzybkiego)))
  //   bitWrite(TablicaDoPC[4], 1, 1);
  // else bitWrite(TablicaDoPC[4], 1, 0);
  
  if (!(digitalRead(GuzikZamkniecieWylacznikaSzybkiego)))
    bitWrite(TablicaDoPC[4], 2, 1);
  else bitWrite(TablicaDoPC[4], 2, 0);
  
  if (!(digitalRead(GuzikOdblokowaniaPrzekaznikaNadmiarowegoSilnikow)))
    bitWrite(TablicaDoPC[4], 3, 1);
  else bitWrite(TablicaDoPC[4], 3, 0);
  
  if (!(digitalRead(GuzikOdblokowanieWylaczeniePrzetwornicy)))
    bitWrite(TablicaDoPC[4], 5, 1);
  else bitWrite(TablicaDoPC[4], 5, 0);
  
  // if (!(digitalRead(GuzikRozlaczenieStycznikowLiniowych)))
  //   bitWrite(TablicaDoPC[4], 6, 1);
  // else bitWrite(TablicaDoPC[4], 6, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrzetwornicy)))
    bitWrite(TablicaDoPC[5], 1, 1);
  else bitWrite(TablicaDoPC[5], 1, 0);
  
  if (!(digitalRead(GuzikWylaczeniePrzetwornicy)))
    bitWrite(TablicaDoPC[5], 0, 1);
  else bitWrite(TablicaDoPC[5], 0, 0);

  if (!(digitalRead(GuzikZalaczenieSprezarki)))
    bitWrite(TablicaDoPC[5], 2, 1);
  else bitWrite(TablicaDoPC[5], 2, 0);
  
//  if (!(digitalRead(GuzikAktywacjaPiasecznicy)))
//    bitWrite(TablicaDoPC[5], 3, 1);
//  else bitWrite(TablicaDoPC[5], 3, 0);
  
  // if (!(digitalRead(GuzikZalaczenieOgrzewaniaPociagu)))
  //   bitWrite(TablicaDoPC[5], 4, 1);
  // else bitWrite(TablicaDoPC[5], 4, 0);
  
  // if (!(digitalRead(GuzikUstawienieRozruchuNiskiego)))
  //   bitWrite(TablicaDoPC[5], 7, 1);
  // else bitWrite(TablicaDoPC[5], 7, 0);
  
  if (!(digitalRead(GuzikPodniesieniePantografuPrzedniego)))
    bitWrite(TablicaDoPC[8], 1, 1);
  else bitWrite(TablicaDoPC[8], 1, 0);
  
  if (!(digitalRead(GuzikPodniesieniePantografuTylnego)))
    bitWrite(TablicaDoPC[8], 2, 1);
  else bitWrite(TablicaDoPC[8], 2, 0);
  
  if (!(digitalRead(GuzikOpuszczeniePantografuPrzedniego)))
    bitWrite(TablicaDoPC[8], 3, 1);
  else bitWrite(TablicaDoPC[8], 3, 0);
  
  if (!(digitalRead(GuzikOpuszczeniePantografuWszystkich)))
    bitWrite(TablicaDoPC[8], 4, 1);
  else bitWrite(TablicaDoPC[8], 4, 0);
  
  // if (!(digitalRead(GuzikAktywacjaPodhamowaniaPrzeciwposlizgowego)))
  //   bitWrite(TablicaDoPC[6], 2, 1);
  // else bitWrite(TablicaDoPC[6], 2, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrzyciemnieniaReflektorow)))
    bitWrite(TablicaDoPC[6], 3, 1);
  else bitWrite(TablicaDoPC[6], 3, 0);
  
//  if (!(digitalRead(GuzikZalaczeniePrzyciemnieniaOswietleniaKabiny)))
//    bitWrite(TablicaDoPC[6], 4, 1);
//  else bitWrite(TablicaDoPC[6], 4, 0);
  
  // if (!(digitalRead(GuzikOdluzniacz)))
  //   bitWrite(TablicaDoPC[6], 5, 1);
  // else bitWrite(TablicaDoPC[6], 5, 0);
  
  // if (!(digitalRead(GuzikBateria)))
  //   bitWrite(TablicaDoPC[7], 0, 1);
  // else bitWrite(TablicaDoPC[7], 0, 0);
  
  if (!(digitalRead(GuzikZalaczenieLewegoReflektora)))
    bitWrite(TablicaDoPC[7], 1, 1);
  else bitWrite(TablicaDoPC[7], 1, 0);
  
  if (!(digitalRead(GuzikZalaczenieGornegoReflektora)))
    bitWrite(TablicaDoPC[7], 2, 1);
  else bitWrite(TablicaDoPC[7], 2, 0);
  
  if (!(digitalRead(GuzikZalaczeniePrawegoReflektora)))
    bitWrite(TablicaDoPC[7], 3, 1);
  else bitWrite(TablicaDoPC[7], 3, 0);
  
  // if (!(digitalRead(GuzikZwiekszeniePozycjiNastawnikaKierunku)))
  //   bitWrite(TablicaDoPC[7], 4, 1);
  // else bitWrite(TablicaDoPC[7], 4, 0);
  
  // if (!(digitalRead(GuzikZmniejszeniePozycjiNastawnikaKierunku)))
  //   bitWrite(TablicaDoPC[7], 5, 1);
  // else bitWrite(TablicaDoPC[7], 5, 0);

  
  if (!(digitalRead(GuzikOdblokowanieSprezarki)))
    bitWrite(TablicaDoPC[8], 5, 1);
  else bitWrite(TablicaDoPC[8], 5, 0);
  
  if (!(digitalRead(GuzikRadioTelefon)))
    bitWrite(TablicaDoPC[8], 6, 1);
  else bitWrite(TablicaDoPC[8], 6, 0);
  
  if (!(digitalRead(GuzikBuczek)))
    bitWrite(TablicaDoPC[8], 7, 1);
  else bitWrite(TablicaDoPC[8], 7, 0);
  
  if (!(digitalRead(GuzikDrzwiLewe)))
    bitWrite(TablicaDoPC[9], 0, 1);
  else bitWrite(TablicaDoPC[9], 0, 0);
  
  if (!(digitalRead(GuzikDrzwiPrawe)))
    bitWrite(TablicaDoPC[9], 1, 1);
  else bitWrite(TablicaDoPC[9], 1, 0);


  // if(!(digitalRead(GuzikTrybuHamulcaDlaPociaguTowarowego)))
  // {
  //   bitWrite(TablicaDoPC[7], 6, 1);
  //   bitWrite(TablicaDoPC[7], 7, 0);
  //   bitWrite(TablicaDoPC[8], 0, 0);
  // }
  // else
  // {
  //   if(!(digitalRead(GuzikTrybuHamulcaDlaPociaguPospiesznego)))
  //   {
  //     bitWrite(TablicaDoPC[7], 6, 0);
  //     bitWrite(TablicaDoPC[7], 7, 0);
  //     bitWrite(TablicaDoPC[8], 0, 1);
  //   }
  //   else
  //   {
  //     bitWrite(TablicaDoPC[7], 6, 0);
  //     bitWrite(TablicaDoPC[7], 7, 1);
  //     bitWrite(TablicaDoPC[8], 0, 0);      
  //   }
  // }
#pragma endregion
    
#pragma region OdczytHamulcow

  if(digitalRead(TrainBrake_low) == LOW)
  {
    TablicaDoPC[12] = 0;
  }
  else if(digitalRead(TrainBrake_high) == LOW)
  {
    TablicaDoPC[12] = 127;
  }
  else
  {
    TablicaDoPC[12] = 40;
  }

  // int tempTrainBrake = analogRead(PotencjometrTrainBrake);
  // TablicaDoPC[12] = lowByte(tempTrainBrake);
  // TablicaDoPC[13] = highByte(tempTrainBrake);

  // int tempIndependentBrake = analogRead(PotencjometrIndependentBrake);
  // TablicaDoPC[14] = lowByte(tempIndependentBrake);
  // TablicaDoPC[15] = highByte(tempIndependentBrake);
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
  if(!(digitalRead(GuzikZalaczeniePrzetwornicy)))
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
  // if(Serial3)
  // {
  //   if(Serial3.available())
  //   {
  //     uint8_t tableHasler[30] = { 0 };
  //     Serial3.readBytes((char*)tableHasler, 30);
  //   }

  //   haslerFrame.Reset();
  //   haslerFrame.AddData(TablicaZPC[4 - PrzesunieciePreambuly]);
  //   haslerFrame.CreateFrame();
    
  //   Serial3.write(haslerFrame.fullFrame, haslerFrame.lengthFrame);
  //   //Serial3.write(TablicaZPC[4 - PrzesunieciePreambuly]);
  // }
#pragma endregion

#pragma region Nastawnik
  // Nastawnik w kiblu :) [na rezystorach]
  uint16_t nastawnik_temp = analogRead(MasterControllerAnalog);

  if(nastawnik_temp <= 35)
  {
    TablicaDoPC[10] = 0;
  }
  else if(nastawnik_temp > 35 && nastawnik_temp <= 83)
  {
    TablicaDoPC[10] = 1;
  }
  else if(nastawnik_temp > 83 && nastawnik_temp <= 100)
  {
    TablicaDoPC[10] = 2;
  }
  else if(nastawnik_temp > 100 && nastawnik_temp <= 127)
  {
    TablicaDoPC[10] = 3;
  }
  else if(nastawnik_temp > 127 && nastawnik_temp <= 173)
  {
    TablicaDoPC[10] = 4;
  }
  else if(nastawnik_temp > 173 && nastawnik_temp <= 277)
  {
    TablicaDoPC[10] = 5;
  }
  else if(nastawnik_temp > 277)
  {
    TablicaDoPC[10] = 6;
  }
#pragma endregion

#pragma region SilnikiKrokoweManometry
  motorCylinder.setPosition(map(((TablicaZPC[12 - PrzesunieciePreambuly] << 8) + TablicaZPC[11 - PrzesunieciePreambuly]), 0, MaxScaleMaSzyna, 0, STEPS));
  motorPrzewodGlowny.setPosition(map(((TablicaZPC[14 - PrzesunieciePreambuly] << 8) + TablicaZPC[13 - PrzesunieciePreambuly]), 0, MaxScaleMaSzyna, 0, STEPS));
  motorZbiornikGlowny.setPosition(map(((TablicaZPC[16 - PrzesunieciePreambuly] << 8) + TablicaZPC[15 - PrzesunieciePreambuly]), 0, MaxScaleMaSzyna, 0, STEPS));

  motorUpdate();
}

void motorUpdate()
{
  motorCylinder.update();
  motorPrzewodGlowny.update();
  motorZbiornikGlowny.update();
}
#pragma endregion