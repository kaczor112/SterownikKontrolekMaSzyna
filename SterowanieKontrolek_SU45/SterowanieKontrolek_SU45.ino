#include <ArduinoFrameCRC.h>
//#include <SoftwareSerial.h>
#include <SwitecX25.h>
#include "PF575.h"
#include "SlowTimerArduino.h"

#pragma region DefinicjeLamp
#define ShpLamp 6
#define CzuwakLamp 5
#define StycznikiLinioweLamp 4
#define PompaPaliwaLamp 3
// Pompa paliwa pwm // zalaczenie po wl baterii(must have) lub wylaczeniu silnika, gasnie od pompy paliwa lub wyłączenia baterii (must have), 
#define PompaWodyLamp 2
// pompa wody  // kiedy wskazanie na manometrze od temp. wody mniejsza niż 40 *C i załączenie baterii, wskazanie wieksze lub równe i brak baterii <- bateria w obu must have
#define WentylatorSilnikowTrakcyjnychLamp 14 // wentylator silnikow trakcyjnych
// powyzsza kiedy A i B sa jednoczesnie wlaczone
#define Bocznik1Lamp 15
#define Bocznik2Lamp 16
#define Bocznik3Lamp 17
#pragma endregion

#pragma region DefinicjeGuzikow
// Definicje guzików i przełączników
#define AktywacjaSyrenyWysokotonowejButton 22
#define AktywacjaSyrenyNiskotonowejButton 23
#define CzuwakShpButton 24
#define StartSilnikaButton 25
#define StopSilnikaButton 26
#define ZalaczenieOgrzewaniaPociaguButton 27
#define AktywacjaPodhamowaniaPrzeciwposlizgowegoButton 28
#define ZalaczeniePrzyciemnieniaReflektorowButton 29
#define OdluzniaczButton 30
#define BateriaButton 31 // reset do stanu zero
#define ZalaczenieCzerwonegoLewegoReflektoraButton 32
#define ZalaczenieCzerwonegoPrawegoReflektoraButton 33
#define ZalaczenieLewegoReflektoraButton 34
#define ZalaczenieGornegoReflektoraButton 35
#define ZalaczeniePrawegoReflektoraButton 36
#define TrybuHamulcaDlaPociaguTowarowegoButton 37
#define TrybuHamulcaDlaPociaguPospiesznegoButton 38
#define OdblokowaniaPrzekaznikaNadmiarowegoSilnikowButton 39 // silnikow trakcyjnych
#define PompaPaliwaButton 40
#define PompaOlejuButton 41
#define PompaWodyButton 42 // Przełącznik
#define PodgrzewaczWodyButton 43 // Przełącznik
#define ZalaczenieWentylatorAButton 44
#define ZalaczenieWentylatorBButton 45
#define WylaczenieWszystkichWentylatorowButton 46
#define NastawnikKierunkuPrzodButton 9
#define NastawnikKierunkuTylButton 56
#pragma endregion

#pragma region DefinicjePotencjometru
#define TrainBrakePotencjometr A0
#define IndependentBrakePotencjometr A1
//#define NastawnikPotencjometr A2
#pragma endregion

#pragma region DefinicjeNastawnik
#define NastawnikPozycja_1 7
#define NastawnikPozycja_2 8
#pragma endregion

#pragma region DefinicjePWM
#define PWM_LV_current 13
#define PWM_HV_current_1 12
#define PWM_HV_voltage 11
#define PWM_Obrotomierz 10

SlowTimerArduino PWM_ObrotomierzTimer(255);
#pragma endregion

#pragma region DefinicjeHasler
// Inicjalizacja SoftwareSerial dla Haslera, 
// rx nie potrzebujemy, więc pin -1 to wyłączony
//#define HaslerTxPin 18  // TX1/RX1 - D18 
//SoftwareSerial mySerial =  SoftwareSerial(-1, HaslerTxPin);
#pragma endregion

#pragma region DefinicjeSilnikiKrokowe
#define STEPS (315*6)
#define MaxScaleMaSzyna 2047

#define SilnikCylinder_A1 57 // A03
#define SilnikCylinder_A2 58
#define SilnikCylinder_B1 59
#define SilnikCylinder_B2 60 // A06

SwitecX25 motorCylinder(STEPS,
  SilnikCylinder_A1,
  SilnikCylinder_A2,
  SilnikCylinder_B1,
  SilnikCylinder_B2);

#define SilnikPrzewodGlowny_A1 61 // A07
#define SilnikPrzewodGlowny_A2 62
#define SilnikPrzewodGlowny_B1 63
#define SilnikPrzewodGlowny_B2 64 // A10

SwitecX25 motorPrzewodGlowny(STEPS,
  SilnikPrzewodGlowny_A1,
  SilnikPrzewodGlowny_A2,
  SilnikPrzewodGlowny_B1,
  SilnikPrzewodGlowny_B2);

#define SilnikZbiornikGlowny_A1 65 // A11
#define SilnikZbiornikGlowny_A2 66
#define SilnikZbiornikGlowny_B1 67
#define SilnikZbiornikGlowny_B2 68 // A14

SwitecX25 motorZbiornikGlowny(STEPS,
  SilnikZbiornikGlowny_A1,
  SilnikZbiornikGlowny_A2,
  SilnikZbiornikGlowny_B1,
  SilnikZbiornikGlowny_B2);

#define SilnikTemperaturaWody_A1 69  // A15
#define SilnikTemperaturaWody_A2 53
#define SilnikTemperaturaWody_B1 52
#define SilnikTemperaturaWody_B2 51

// temperatura wody d5-d2 // w 6 min osoagnal 45 *c po zalaczeniu podgrzewacza wody, po wlaczeniu silnika do 80*C w ciagu 8 minut, i w druga strone 
SwitecX25 motorTemperaturaWody(STEPS,
  SilnikTemperaturaWody_A1,
  SilnikTemperaturaWody_A2,
  SilnikTemperaturaWody_B1,
  SilnikTemperaturaWody_B2);

SlowTimerArduino motorTemperaturaWodyTimer(STEPS);

#define SilnikCisnienieOleju_A1 50
#define SilnikCisnienieOleju_A2 49
#define SilnikCisnienieOleju_B1 48
#define SilnikCisnienieOleju_B2 47

// cisnienie oleju d14-d17 // w domu Józef obczai zasade działania
SwitecX25 motorCisnienieOleju(STEPS,
  SilnikCisnienieOleju_A1,
  SilnikCisnienieOleju_A2,
  SilnikCisnienieOleju_B1,
  SilnikCisnienieOleju_B2);

SlowTimerArduino motorCisnienieOlejuTimer(STEPS);

/*#define SilnikObrotomierzSilnika_A1 6
#define SilnikObrotomierzSilnika_A2 7
#define SilnikObrotomierzSilnika_B1 8
#define SilnikObrotomierzSilnika_B2 9

// obrotomierz silnika d9-d6 // po wlczenou 750 (w ciagu 9 sek) po zalaczeniu ogrzewania 1200, zalezny od napiecia lokomotywy 1000v = 1500 obrotow
SwitecX25 motorObrotomierzSilnika(STEPS,
  SilnikObrotomierzSilnika_A1,
  SilnikObrotomierzSilnika_A2,
  SilnikObrotomierzSilnika_B1,
  SilnikObrotomierzSilnika_B2);*/
#pragma endregion

volatile uint8_t TablicaZPC[52] = { 0 };
volatile uint8_t TablicaDoPC[20] = { 0 };

PF575 ekspander1(0x20);     // Inicjuje ekspander o adresie 0x20

void setup() {  
  ekspander1.begin();  // ustawienie ustawień początkowych ekspandera

  // Ustawienie początkowe timerów
  motorTemperaturaWodyTimer.setData(150, 1000); // wskazanie 150 do 1sek
  motorCisnienieOlejuTimer.setData(150, 1000); // wskazanie 150 do 1sek

#pragma region InicjalizacjaSilnikowKrokowych
  motorCylinder.zero();
  motorPrzewodGlowny.zero();
  motorZbiornikGlowny.zero();
  motorTemperaturaWody.zero();
  //motorObrotomierzSilnika.zero();
  motorCisnienieOleju.zero();
#pragma endregion

#pragma region InicjalizacjaLamp
  // Konfiguracja lamp
  pinMode(ShpLamp, OUTPUT);
  digitalWrite(ShpLamp, LOW);
  pinMode(CzuwakLamp, OUTPUT);
  digitalWrite(CzuwakLamp, LOW);
  pinMode(StycznikiLinioweLamp, OUTPUT);
  digitalWrite(StycznikiLinioweLamp, LOW);
  pinMode(PompaPaliwaLamp, OUTPUT);
  digitalWrite(PompaPaliwaLamp, LOW);
  pinMode(PompaWodyLamp, OUTPUT);
  digitalWrite(PompaWodyLamp, LOW);
  pinMode(WentylatorSilnikowTrakcyjnychLamp, OUTPUT);
  digitalWrite(WentylatorSilnikowTrakcyjnychLamp, LOW);
  pinMode(Bocznik1Lamp, OUTPUT);
  digitalWrite(Bocznik1Lamp, LOW);
  pinMode(Bocznik2Lamp, OUTPUT);
  digitalWrite(Bocznik2Lamp, LOW);
  pinMode(Bocznik3Lamp, OUTPUT);
  digitalWrite(Bocznik3Lamp, LOW);
#pragma endregion

#pragma region InicjalizacjaGuzikow
  // Konfiguracja guzików i przełączników
  // INPUT_PULLUP - ustawienie pinu 2 jako wejście z podciągnięciem do zasilania
  pinMode(AktywacjaSyrenyWysokotonowejButton, INPUT_PULLUP);
  pinMode(AktywacjaSyrenyNiskotonowejButton, INPUT_PULLUP);
  pinMode(CzuwakShpButton, INPUT_PULLUP);
  pinMode(StartSilnikaButton, INPUT_PULLUP);
  pinMode(StopSilnikaButton, INPUT_PULLUP);
  pinMode(ZalaczenieOgrzewaniaPociaguButton, INPUT_PULLUP);
  pinMode(AktywacjaPodhamowaniaPrzeciwposlizgowegoButton, INPUT_PULLUP);
  pinMode(ZalaczeniePrzyciemnieniaReflektorowButton, INPUT_PULLUP);
  pinMode(OdluzniaczButton, INPUT_PULLUP);
  pinMode(BateriaButton, INPUT_PULLUP);
  pinMode(ZalaczenieCzerwonegoLewegoReflektoraButton, INPUT_PULLUP);
  pinMode(ZalaczenieCzerwonegoPrawegoReflektoraButton, INPUT_PULLUP);
  pinMode(ZalaczenieLewegoReflektoraButton, INPUT_PULLUP);
  pinMode(ZalaczenieGornegoReflektoraButton, INPUT_PULLUP);
  pinMode(ZalaczeniePrawegoReflektoraButton, INPUT_PULLUP);
  pinMode(TrybuHamulcaDlaPociaguTowarowegoButton, INPUT_PULLUP);
  pinMode(TrybuHamulcaDlaPociaguPospiesznegoButton, INPUT_PULLUP);
  pinMode(OdblokowaniaPrzekaznikaNadmiarowegoSilnikowButton, INPUT_PULLUP);
  pinMode(PompaPaliwaButton, INPUT_PULLUP);
  pinMode(PompaOlejuButton, INPUT_PULLUP);
  pinMode(PompaWodyButton, INPUT_PULLUP);
  pinMode(PodgrzewaczWodyButton, INPUT_PULLUP);
//  pinMode(WlaczenieRadiaButton, INPUT_PULLUP);
 // pinMode(RadioStopButton, INPUT_PULLUP);
//  pinMode(RadioZew3Button, INPUT_PULLUP);
  pinMode(ZalaczenieWentylatorAButton, INPUT_PULLUP);
  pinMode(ZalaczenieWentylatorBButton, INPUT_PULLUP);
  pinMode(WylaczenieWszystkichWentylatorowButton, INPUT_PULLUP);
  pinMode(NastawnikKierunkuPrzodButton, INPUT_PULLUP);
  pinMode(NastawnikKierunkuTylButton, INPUT_PULLUP);

  pinMode(NastawnikPozycja_1, INPUT_PULLUP);
  pinMode(NastawnikPozycja_2, INPUT_PULLUP);
#pragma endregion

#pragma region InicjalizacjaPWM
  pinMode(PWM_LV_current, OUTPUT);
  pinMode(PWM_HV_current_1, OUTPUT);
  pinMode(PWM_HV_voltage, OUTPUT);
  pinMode(PWM_Obrotomierz, OUTPUT);
#pragma endregion

#pragma region InicjalizacjaKomunikacjiHasler
  // Konfiguracja komunikacji - Hasler
  //pinMode(HaslerTxPin, OUTPUT);
  
  Serial1.begin(9600);
  Serial1.setTimeout(10);
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
#pragma endregion
}

#pragma region ZmienneGlobalne
// Zmienne globalne
unsigned long aktualnyCzas = 0;
unsigned long haslerCzas = 0;
ArduinoFrameCRC haslerFrame;
unsigned long RadioClickCzas = 0;
bool RadioClick = false;

// Na potrzeby wewnętrznych zależności
bool ZalaczonaBateria = false;
bool ZalaczonySilnikStan = false;
bool OgrzewanieWodyStan = false;
bool OgrzewaniePociaguStan = false;
bool PompaOlejuStan = false;
bool ZalaczenieWentylatorAStan = false;
bool ZalaczenieWentylatorBStan = false;

uint16_t TemperaturaWodyWskazanie = 0;
uint16_t TemperaturaWodyCzas = 0;

uint16_t CisnienieOlejuWskazanie = 0;
uint16_t CisnienieOlejuCzas = 0;

uint16_t ObrotomierzSilnikaWskazanie = 0;

// Lampy
bool PompaPaliwaOdznaczona = false;
#pragma endregion

void loop() {  
#pragma region KomunikacjaZasadnicza
  aktualnyCzas = millis();

  while (!Serial.available()) {
    UpdateSilnikowKrokowychOrazTimerow();
    ekspander1.update();

    if(millis() - aktualnyCzas > 1000)
    {
      Serial.write((char*)TablicaDoPC, 20);

      aktualnyCzas = millis();
    }
  }

  Serial.readBytes((char*)TablicaZPC, 52);
  Serial.write((char*)TablicaDoPC, 20);
#pragma endregion

  if(TablicaZPC[0] == 0xEF && TablicaZPC[1] == 0xEF && TablicaZPC[2] == 0xEF && TablicaZPC[3] == 0xEF)
  {
    // Operacje od MaSzyny do Arduino
#pragma region UstawianieLamp
    digitalWrite(ShpLamp, !(bitRead(TablicaZPC[8], 7)));
    digitalWrite(CzuwakLamp, !(bitRead(TablicaZPC[8], 6)));
    digitalWrite(StycznikiLinioweLamp, !(bitRead(TablicaZPC[9], 5)));
    //digitalWrite(WentylatorSilnikowTrakcyjnychLamp, !(bitRead(TablicaZPC[6], 1)));
    digitalWrite(WentylatorSilnikowTrakcyjnychLamp, !(ZalaczenieWentylatorAStan && ZalaczenieWentylatorBStan && ZalaczonySilnikStan));
    
    // Pompa paliwa - zalaczenie po wl baterii(must have) lub wylaczeniu silnika, gasnie od pompy paliwa lub wyłączenia baterii (must have), 
    bool PompaPaliwaLampStan = false;
    if(ZalaczonaBateria)
    {
      if(PompaPaliwaOdznaczona)
      {
        if(!digitalRead(StopSilnikaButton)) PompaPaliwaOdznaczona = false;
      }
      else
      {
        if(!digitalRead(PompaPaliwaButton)) PompaPaliwaOdznaczona = true;

        PompaPaliwaLampStan = true;
      }
    }
    digitalWrite(PompaPaliwaLamp, !(PompaPaliwaLampStan));

    // pompa wody - kiedy wskazanie na manometrze od temp. wody mniejsza niż 40 *C i załączenie baterii, wskazanie wieksze lub równe i brak baterii <- bateria w obu must have
    bool PompaWodyLampStan = false;
    if(ZalaczonaBateria)
    {
      if(TemperaturaWodyWskazanie < 500) PompaWodyLampStan = true; // nie wiem czy 500 to 45*C
    }
    digitalWrite(PompaWodyLamp, !(PompaWodyLampStan));
#pragma endregion

#pragma region UstawienieSygnalowPWM
  analogWrite(PWM_HV_current_1, TablicaZPC[19]);
  analogWrite(PWM_HV_voltage, TablicaZPC[17]);

  // Sztuczne ustawienie sygnału PWM_LV_current
  if(!(digitalRead(PompaPaliwaButton)) && !(digitalRead(BateriaButton)))
  {
    analogWrite(PWM_LV_current, 200);
  }
  else
  {
    analogWrite(PWM_LV_current, 127);
  }  

  // Zerowanie po wyłączeniu programu
  if((TablicaZPC[17] == 0) && (TablicaZPC[19] == 0))
  {
    analogWrite(PWM_LV_current, 0);
  }    
  
  // obrotomierz silnika po wlczenou 750  
  // po zalaczeniu ogrzewania 1200,
  // zalezny od napiecia lokomotywy 1000v = 1500 obrotow
    if(ZalaczonaBateria && ZalaczonySilnikStan)
    {
      // Po włączeniu 750 obrotów
      ObrotomierzSilnikaWskazanie = 100;

      // Po włączeniu ogrzewania do 1200 obrotów
      if(OgrzewaniePociaguStan) 
      {
        ObrotomierzSilnikaWskazanie = 150;
      }

      // Jeżeli chodzi o przekładnie między biegami to robie płynną
      // 180 to dal mnie 1500 obrotów
      if(TablicaDoPC[10] > 0) // TablicaDoPC[10] to pozycja nastawnika jazdy
        ObrotomierzSilnikaWskazanie = map(TablicaDoPC[10], 0, 14, ObrotomierzSilnikaWskazanie, 180);

      // Do każdej następnej nastawy daje 2sek
      PWM_ObrotomierzTimer.setData(ObrotomierzSilnikaWskazanie, 2000);
    }
    else
    {
      // po wygaszeniu 6sek do '0'
      PWM_ObrotomierzTimer.setData(0, 6000);
    }
#pragma endregion

#pragma region SilnikiKrokoweSetData
    motorCylinder.setPosition(map(word(TablicaZPC[12], TablicaZPC[11]), 0, MaxScaleMaSzyna, 0, STEPS));
    motorPrzewodGlowny.setPosition(map(word(TablicaZPC[14], TablicaZPC[13]), 0, MaxScaleMaSzyna, 0, STEPS));
    motorZbiornikGlowny.setPosition(map(word(TablicaZPC[16], TablicaZPC[15]), 0, MaxScaleMaSzyna, 0, STEPS));

    // temperatura wody w 5 min osiąga 45 *c po zalaczeniu podgrzewacza wody
    // po wlaczeniu silnika do 70*C w ciagu nastepnych 5 minut,
    /* Zakładam wskazanie:
     * 150 dla 20*C
     * 250 dla 45*C
     * 600 dla 70*C
     */
    if(ZalaczonaBateria && OgrzewanieWodyStan)
    {
      // Temperatura wody rośnie
      if(ZalaczonySilnikStan)
      {
        TemperaturaWodyWskazanie = 600;
        TemperaturaWodyCzas = 300000; // 5min
      }
      else
      {
        TemperaturaWodyWskazanie = 250;
        TemperaturaWodyCzas = 300000; // 5min
      }
    }
    else
    {
      // Temperatura wody maleje
      TemperaturaWodyWskazanie = 150;
      TemperaturaWodyCzas = 600000; // 10min 1000ms * 60sek * 10min
    }

    motorTemperaturaWodyTimer.setData(TemperaturaWodyWskazanie, TemperaturaWodyCzas);

    /* Ciśnienie oleju wskazanie:
     * stan 0 to 20*C
     * 0.3 tarczy po załączonej Pompie Oleju
     * 0.5 po starcie silnika
     * W zalżnoci od obroówo 0.75 doa 1500 obrotów
     */

    if(ZalaczonaBateria && PompaOlejuStan)
    {
      if(ZalaczonySilnikStan)
      {
        CisnienieOlejuWskazanie = 1000;
        CisnienieOlejuCzas = 2000; // 2sek
        
        if(TablicaDoPC[10] > 0) // TablicaDoPC[10] to pozycja nastawnika jazdy
          CisnienieOlejuWskazanie = map(TablicaDoPC[10], 0, 14, CisnienieOlejuWskazanie, 1500);
      }
      else
      {
        CisnienieOlejuWskazanie = 250;
        CisnienieOlejuCzas = 2000; // 2sek
      }
    }
    else
    {
      // po wyłączeniu 20*C w 6sek
      CisnienieOlejuWskazanie = 150;
      CisnienieOlejuCzas = 6000;
    }
    
    motorCisnienieOlejuTimer.setData(CisnienieOlejuWskazanie, CisnienieOlejuCzas);
#pragma endregion

#pragma region UstawienieHaslera
    if(Serial1 && millis() - haslerCzas > 500)
    {
      if(Serial1.available())
      {
        uint8_t tableHasler[30] = { 0 };
        Serial1.readBytes((char*)tableHasler, 30);
      }

      haslerFrame.Reset();
      haslerFrame.AddData(TablicaZPC[4]);
      haslerFrame.CreateFrame();
      
      Serial1.write(haslerFrame.fullFrame, haslerFrame.lengthFrame);

      haslerCzas = millis();
    }
#pragma endregion

#pragma region UstawKanalRadia
  uint8_t KanalRadiaTemp = 1;

  uint8_t i = 0;
  for(i = 0; i < 10; i++)
    if(!ekspander1.getValue(i)) KanalRadiaTemp = i + 1;

  if(millis() - RadioClickCzas > 100)
  {
    if(TablicaZPC[37] != KanalRadiaTemp)
    {
      if(!RadioClick)
      {
        if(TablicaZPC[37] < KanalRadiaTemp)
        {
          // Zwiekszam kanal
          RadioClick = true;
          bitWrite(TablicaDoPC[8], 1, true);
          bitWrite(TablicaDoPC[8], 2, false);
        }
        else
        {
          // Zmniejsz kanal
          RadioClick = true;
          bitWrite(TablicaDoPC[8], 1, false);
          bitWrite(TablicaDoPC[8], 2, true);
        }
      }
      else
      {
        // pauza zeby był click
        RadioClick = false;
        bitWrite(TablicaDoPC[8], 1, false);
        bitWrite(TablicaDoPC[8], 2, false);
      }
    }
    else
    {
      // Jest dobrze. Reset click.
      RadioClick = false;
      bitWrite(TablicaDoPC[8], 1, false);
      bitWrite(TablicaDoPC[8], 2, false);
    }
    
    RadioClickCzas = millis();
  }
#pragma endregion

#pragma region NastawnikLampyBocznikow
  //uint16_t nastawnik_temp = analogRead(NastawnikPotencjometr);

  bool Bocznik1temp = false;
  bool Bocznik2temp = false;
  bool Bocznik3temp = false;

  if(digitalRead(NastawnikPozycja_1))
  {
    TablicaDoPC[10] = 0;
  }
  else if(digitalRead(NastawnikPozycja_2))
  {
    TablicaDoPC[10] = 1;
  }
  else
  {
    uint8_t tempNastawnik = 0;

    bitWrite(tempNastawnik, 0, !ekspander1.getP12());   // 1x
    bitWrite(tempNastawnik, 1, !ekspander1.getP13());   // 2y
    bitWrite(tempNastawnik, 2, !ekspander1.getP14());   // 3z
    bitWrite(tempNastawnik, 3, !ekspander1.getP15());   // 4k

    if(tempNastawnik == 0)
    {
      TablicaDoPC[10] = 2;
    }
    else if(tempNastawnik == 10)
    {
      TablicaDoPC[10] = 3;
    }
    else if(tempNastawnik == 4)
    {
      TablicaDoPC[10] = 4;
    }
    else if(tempNastawnik == 14)
    {
      TablicaDoPC[10] = 5;
    }
    else if(tempNastawnik == 8)
    {
      TablicaDoPC[10] = 6;
    }
    else if(tempNastawnik == 3)
    {
      TablicaDoPC[10] = 7;
    }
    else if(tempNastawnik == 12)
    {
      TablicaDoPC[10] = 8;
      if(TablicaZPC[4] >= 40) Bocznik1temp = true;
      if(TablicaZPC[4] >= 50) Bocznik2temp = true;
    }
    else if(tempNastawnik == 7)
    {
      TablicaDoPC[10] = 9;
      if(TablicaZPC[4] >= 40) Bocznik1temp = true;
      if(TablicaZPC[4] >= 50) Bocznik2temp = true;
    }
    else if(tempNastawnik == 1)
    {
      TablicaDoPC[10] = 10;
      if(TablicaZPC[4] >= 40) Bocznik1temp = true;
      if(TablicaZPC[4] >= 50) Bocznik2temp = true;
    }
    else if(tempNastawnik == 11)
    {
      TablicaDoPC[10] = 11;
      if(TablicaZPC[4] >= 40) 
      {
        Bocznik1temp = true;
        Bocznik2temp = true;
        if(TablicaZPC[4] >= 50) Bocznik3temp = true;
      }
    }
    else if(tempNastawnik == 5)
    {
      TablicaDoPC[10] = 12;
      if(TablicaZPC[4] >= 40) 
      {
        Bocznik1temp = true;
        Bocznik2temp = true;
        if(TablicaZPC[4] >= 50) Bocznik3temp = true;
      }
    }
    else if(tempNastawnik == 15)
    {
      TablicaDoPC[10] = 13;
      if(TablicaZPC[4] >= 40) 
      {
        Bocznik1temp = true;
        Bocznik2temp = true;
        if(TablicaZPC[4] >= 50) Bocznik3temp = true;
      }
    }
    else if(tempNastawnik == 9)
    {
      TablicaDoPC[10] = 14;
      if(TablicaZPC[4] >= 40) 
      {
        Bocznik1temp = true;
        Bocznik2temp = true;
        if(TablicaZPC[4] >= 50) Bocznik3temp = true;
      }
    }    
  }

  /*if(nastawnik_temp <= 18)
  {
    TablicaDoPC[10] = 0;
  }
  else if(nastawnik_temp > 18 && nastawnik_temp <= 57)
  {
    TablicaDoPC[10] = 1;
  }
  else if(nastawnik_temp > 57 && nastawnik_temp <= 94)
  {
    TablicaDoPC[10] = 2;
  }
  else if(nastawnik_temp > 94 && nastawnik_temp <= 132)
  {
    TablicaDoPC[10] = 3;
  }
  else if(nastawnik_temp > 132 && nastawnik_temp <= 170)
  {
    TablicaDoPC[10] = 4;
  }
  else if(nastawnik_temp > 170 && nastawnik_temp <= 210)
  {
    TablicaDoPC[10] = 5;
  }
  else if(nastawnik_temp > 210 && nastawnik_temp <= 249)
  {
    TablicaDoPC[10] = 6;
  }
  else if(nastawnik_temp > 249 && nastawnik_temp <= 287)
  {
    TablicaDoPC[10] = 7;
  }
  else if(nastawnik_temp > 287 && nastawnik_temp <= 325)
  {
    TablicaDoPC[10] = 8;
    if(TablicaZPC[4] >= 40) Bocznik1temp = true;
    if(TablicaZPC[4] >= 50) Bocznik2temp = true;
  }
  else if(nastawnik_temp > 325 && nastawnik_temp <= 362)
  {
    TablicaDoPC[10] = 9;
    if(TablicaZPC[4] >= 40) Bocznik1temp = true;
    if(TablicaZPC[4] >= 50) Bocznik2temp = true;
  }
  else if(nastawnik_temp > 362 && nastawnik_temp <= 399)
  {
    TablicaDoPC[10] = 10;
    if(TablicaZPC[4] >= 40) Bocznik1temp = true;
    if(TablicaZPC[4] >= 50) Bocznik2temp = true;
  }
  else if(nastawnik_temp > 399 && nastawnik_temp <= 435)
  {
    TablicaDoPC[10] = 11;
    if(TablicaZPC[4] >= 40) 
    {
      Bocznik1temp = true;
      Bocznik2temp = true;
      if(TablicaZPC[4] >= 50) Bocznik3temp = true;
    }
  }
  else if(nastawnik_temp > 435 && nastawnik_temp <= 470)
  {
    TablicaDoPC[10] = 12;
    if(TablicaZPC[4] >= 40) 
    {
      Bocznik1temp = true;
      Bocznik2temp = true;
      if(TablicaZPC[4] >= 50) Bocznik3temp = true;
    }
  }
  else if(nastawnik_temp > 470 && nastawnik_temp <= 507)
  {
    TablicaDoPC[10] = 13;
    if(TablicaZPC[4] >= 40) 
    {
      Bocznik1temp = true;
      Bocznik2temp = true;
      if(TablicaZPC[4] >= 50) Bocznik3temp = true;
    }
  }
  else if(nastawnik_temp > 507)
  {
    TablicaDoPC[10] = 14;
    if(TablicaZPC[4] >= 40) 
    {
      Bocznik1temp = true;
      Bocznik2temp = true;
      if(TablicaZPC[4] >= 50) Bocznik3temp = true;
    }
  }*/
    
  digitalWrite(Bocznik1Lamp, !Bocznik1temp);
  digitalWrite(Bocznik2Lamp, !Bocznik2temp);
  digitalWrite(Bocznik3Lamp, !Bocznik3temp);
#pragma endregion
  }

  // Operacje od Arduino do MaSzyny
#pragma region OdczytGuzikow
  if(!digitalRead(StartSilnikaButton)) ZalaczonySilnikStan = true;
  bitWrite(TablicaDoPC[4], 1, !digitalRead(StartSilnikaButton));

  if(!digitalRead(StopSilnikaButton)) ZalaczonySilnikStan = false;
  bitWrite(TablicaDoPC[4], 2, !digitalRead(StopSilnikaButton));

  bitWrite(TablicaDoPC[4], 3, !digitalRead(OdblokowaniaPrzekaznikaNadmiarowegoSilnikowButton));
  bitWrite(TablicaDoPC[4], 4, !digitalRead(ZalaczenieCzerwonegoLewegoReflektoraButton));
  bitWrite(TablicaDoPC[4], 5, !digitalRead(ZalaczenieCzerwonegoPrawegoReflektoraButton));
  bitWrite(TablicaDoPC[4], 6, !digitalRead(PompaPaliwaButton));
  bitWrite(TablicaDoPC[4], 7, !digitalRead(CzuwakShpButton));
  if(!PompaOlejuStan) PompaOlejuStan = !digitalRead(PompaOlejuButton);
  bitWrite(TablicaDoPC[5], 0, !digitalRead(PompaOlejuButton));
  bitWrite(TablicaDoPC[5], 1, !digitalRead(PompaWodyButton));

  OgrzewanieWodyStan = !digitalRead(PodgrzewaczWodyButton);
  bitWrite(TablicaDoPC[5], 2, OgrzewanieWodyStan);

  bitWrite(TablicaDoPC[5], 3, !ekspander1.getP17()); // Wlaczenie radia

  OgrzewaniePociaguStan = !digitalRead(ZalaczenieOgrzewaniaPociaguButton);  
  bitWrite(TablicaDoPC[5], 4, OgrzewaniePociaguStan);

  bitWrite(TablicaDoPC[5], 5, !ekspander1.getP16());  // Radio stop
  //bitWrite(TablicaDoPC[5], 6, !digitalRead(RadioZew3Button));
  bitWrite(TablicaDoPC[5], 7, !digitalRead(ZalaczenieWentylatorAButton));
  bitWrite(TablicaDoPC[6], 0, !digitalRead(ZalaczenieWentylatorBButton));
  bitWrite(TablicaDoPC[6], 1, !digitalRead(WylaczenieWszystkichWentylatorowButton));
  if(!digitalRead(ZalaczenieWentylatorAButton)) ZalaczenieWentylatorAStan = true;
  if(!digitalRead(ZalaczenieWentylatorBButton)) ZalaczenieWentylatorBStan = true;
  if(!digitalRead(WylaczenieWszystkichWentylatorowButton))
  {
    ZalaczenieWentylatorAStan = false;
    ZalaczenieWentylatorBStan = false;
  }
  bitWrite(TablicaDoPC[6], 2, !digitalRead(AktywacjaPodhamowaniaPrzeciwposlizgowegoButton));
  bitWrite(TablicaDoPC[6], 3, !digitalRead(ZalaczeniePrzyciemnieniaReflektorowButton));
  bitWrite(TablicaDoPC[6], 5, !digitalRead(OdluzniaczButton));
  bitWrite(TablicaDoPC[6], 6, !digitalRead(AktywacjaSyrenyWysokotonowejButton));
  bitWrite(TablicaDoPC[6], 7, !digitalRead(AktywacjaSyrenyNiskotonowejButton));

  ZalaczonaBateria = !digitalRead(BateriaButton);
  bitWrite(TablicaDoPC[7], 0, ZalaczonaBateria);
  if(!ZalaczonaBateria)
  {
    // Reset ustawien
    PompaPaliwaOdznaczona = false;
    PompaOlejuStan = false;
  }

  bitWrite(TablicaDoPC[7], 1, !digitalRead(ZalaczenieLewegoReflektoraButton));
  bitWrite(TablicaDoPC[7], 2, !digitalRead(ZalaczenieGornegoReflektoraButton));
  bitWrite(TablicaDoPC[7], 3, !digitalRead(ZalaczeniePrawegoReflektoraButton));
  
  
  if(!(digitalRead(NastawnikKierunkuPrzodButton)))
  {
    bitWrite(TablicaDoPC[8], 3, 1);
    bitWrite(TablicaDoPC[8], 4, 0);
    bitWrite(TablicaDoPC[8], 5, 0);
  }
  else
  {
    if(!(digitalRead(NastawnikKierunkuTylButton)))
    {
      bitWrite(TablicaDoPC[8], 3, 0);
      bitWrite(TablicaDoPC[8], 4, 0);
      bitWrite(TablicaDoPC[8], 5, 1);
    }
    else
    {
      bitWrite(TablicaDoPC[8], 3, 0);
      bitWrite(TablicaDoPC[8], 4, 1);
      bitWrite(TablicaDoPC[8], 5, 0);    
    }
  }  

  if(!(digitalRead(TrybuHamulcaDlaPociaguTowarowegoButton)))
  {
    bitWrite(TablicaDoPC[7], 6, 1);
    bitWrite(TablicaDoPC[7], 7, 0);
    bitWrite(TablicaDoPC[8], 0, 0);
  }
  else
  {
    if(!(digitalRead(TrybuHamulcaDlaPociaguPospiesznegoButton)))
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

  //TablicaDoPC[11] = highByte(ekspander1.getData());
    
#pragma region OdczytHamulcow
  int tempTrainBrake = analogRead(TrainBrakePotencjometr);
  TablicaDoPC[12] = lowByte(tempTrainBrake);
  TablicaDoPC[13] = highByte(tempTrainBrake);

  int tempIndependentBrake = analogRead(IndependentBrakePotencjometr);
  TablicaDoPC[14] = lowByte(tempIndependentBrake);
  TablicaDoPC[15] = highByte(tempIndependentBrake);
#pragma endregion

ekspander1.update();

#pragma region SilnikiKrokoweAndTimeryUpdate
  UpdateSilnikowKrokowychOrazTimerow();
}

void UpdateSilnikowKrokowychOrazTimerow()
{
  motorTemperaturaWodyTimer.update();
  motorCisnienieOlejuTimer.update();
  PWM_ObrotomierzTimer.update();

  motorTemperaturaWody.setPosition(motorTemperaturaWodyTimer.getData());
  motorCisnienieOleju.setPosition(motorCisnienieOlejuTimer.getData());
  analogWrite(PWM_Obrotomierz, PWM_ObrotomierzTimer.getData());

  motorCylinder.update();
  motorPrzewodGlowny.update();
  motorZbiornikGlowny.update();
  motorTemperaturaWody.update();
  //motorObrotomierzSilnika.update();
  motorCisnienieOleju.update();  
}
#pragma endregion
