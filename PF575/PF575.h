/*
 * Dla wszystkich którzy mają problemy z ekspanderem PF575 dedyktuję tą bibliotekę
 *
 * Pozdrawiam Paweł Kaczmarczyk
 **/

#ifndef PF575_h
#define PF575_h
#include <Arduino.h>
#include <Wire.h>

class PF575
{
private:
    int address = 0x20;             // Domyślny adres ekspandera
    int8_t pinINT = -1;             // Pin do flagi INT na ekspanderze
    bool changeSave = true;         // Flaga na potrzeby save mode
    uint8_t counterSave = 100;      // Counter na potrzeby save mode
    uint16_t myData = 0xFF;         // Zmienna na dane 'do' i 'z' ekspandera
    uint16_t maskOfSetBits = 0x00;  // Maska ustawiony bitów jako DO ekspandera

    void refreshData();     // Aktualizuje WE & WY
    void writeData();       // Aktualizuje WY wyjścia DO ekspandera z ARD(po ustawieniu bitów z MaSzyny)
    void readData();        // Aktualizuje WE wejścia Z ekspandera do ARD(przed ustawieniem bitów do MaSzyny)

public:
    PF575(uint8_t address = 0x20);
    PF575(uint8_t address, uint8_t pinINT);

    void begin();               // Inicjalizacja komunikacji.
    void update();              // Aktualizuje stan eksopandera lub nie podejmuje działań jeśli nie trzeba

    uint16_t getData();         // Pobiera całość danych z ekspandera

    bool getValue(uint8_t whichBit);                    // Pobiera bit Z  ekspandera
    void setValue(uint8_t whichBit, bool value);        // Ustawia bit DO ekspandera 
    void toggleValue(uint8_t whichBit);                 // Przełącza wartość na przeciwną
    
    // Get
    bool getP00() { return getValue(0); }
    bool getP01() { return getValue(1); }
    bool getP02() { return getValue(2); }
    bool getP03() { return getValue(3); }
    bool getP04() { return getValue(4); }
    bool getP05() { return getValue(5); }
    bool getP06() { return getValue(6); }
    bool getP07() { return getValue(7); }
    bool getP10() { return getValue(8); }
    bool getP11() { return getValue(9); }
    bool getP12() { return getValue(10); }
    bool getP13() { return getValue(11); }
    bool getP14() { return getValue(12); }
    bool getP15() { return getValue(13); }
    bool getP16() { return getValue(14); }
    bool getP17() { return getValue(15); }

    // Set
    void setP00(bool value) { setValue(0, value); }
    void setP01(bool value) { setValue(1, value); }
    void setP02(bool value) { setValue(2, value); }
    void setP03(bool value) { setValue(3, value); }
    void setP04(bool value) { setValue(4, value); }
    void setP05(bool value) { setValue(5, value); }
    void setP06(bool value) { setValue(6, value); }
    void setP07(bool value) { setValue(7, value); }
    void setP10(bool value) { setValue(8, value); }
    void setP11(bool value) { setValue(9, value); }
    void setP12(bool value) { setValue(10, value); }
    void setP13(bool value) { setValue(11, value); }
    void setP14(bool value) { setValue(12, value); }
    void setP15(bool value) { setValue(13, value); }
    void setP16(bool value) { setValue(14, value); }
    void setP17(bool value) { setValue(15, value); }

    // Toggle
    void toggleP00() { toggleValue(0); }
    void toggleP01() { toggleValue(1); }
    void toggleP02() { toggleValue(2); }
    void toggleP03() { toggleValue(3); }
    void toggleP04() { toggleValue(4); }
    void toggleP05() { toggleValue(5); }
    void toggleP06() { toggleValue(6); }
    void toggleP07() { toggleValue(7); }
    void toggleP10() { toggleValue(8); }
    void toggleP11() { toggleValue(9); }
    void toggleP12() { toggleValue(10); }
    void toggleP13() { toggleValue(11); }
    void toggleP14() { toggleValue(12); }
    void toggleP15() { toggleValue(13); }
    void toggleP16() { toggleValue(14); }
    void toggleP17() { toggleValue(15); }
};
#endif