/*
 * Dla wszystkich którzy mają problemy z ekspanderem PF575 dedyktuję tą bibliotekę
 *
 * Pozdrawiam Paweł Kaczmarczyk
 **/

#include <Arduino.h>
#include <Wire.h>
#include "PF575.h"

PF575::PF575(uint8_t address)
{
    this->address = address;
    pinINT = -1;
}

PF575::PF575(uint8_t address, uint8_t pinINT)
{
    this->address = address;
    this->pinINT = pinINT;
    changeSave = true;
}

// Inicjalizacja komunikacji.
void PF575::begin()
{
    if(pinINT != (-1)) pinMode(pinINT, INPUT_PULLUP);
    Wire.begin();
    myData = 0xFF;
    maskOfSetBits = 0x00;
    writeData();
}

// Aktualizuje WY wyjścia DO ekspandera z ARD(po ustawieniu bitów z MaSzyny)
void PF575::writeData()
{
    Wire.beginTransmission(address);
    Wire.write(lowByte(myData));
    Wire.write(highByte(myData));
    Wire.endTransmission();
}

// Aktualizuje WE wejścia Z ekspandera do ARD(przed ustawieniem bitów do MaSzyny)
void PF575::readData()
{
    Wire.requestFrom(address, 2);

    if(Wire.available()) {
        uint8_t l = Wire.read();    // Receive a byte as character
        uint8_t h = Wire.read();    // Receive a byte as character
        
        myData = word(h, l);
    }
}

// Aktualizuje stan eksopandera lub nie podejmuje działań jeśli nie trzeba
void PF575::update()
{
    if(pinINT == -1)
    {
        refreshData();
    }
    else
    {
        counterSave++;
        if (digitalRead(pinINT) == LOW || changeSave || counterSave > 100)
        {
            refreshData();
            counterSave = 0;
            changeSave = false;
        }
    }
}

// Aktualizuje WE & WY
void PF575::refreshData()
{
    myData |= ~maskOfSetBits;
    
    writeData();
    readData();
}

uint16_t PF575::getData()
{
    return myData;
}

bool PF575::getValue(uint8_t whichBit)
{
    return bitRead(myData, whichBit);
}

void PF575::setValue(uint8_t whichBit, bool value)
{
    if(getValue(whichBit) != value) changeSave = true;
    bitSet(maskOfSetBits, whichBit); 
    bitWrite(myData, whichBit, value);
}

void PF575::toggleValue(uint8_t whichBit)
{
    changeSave = true;
    bitSet(maskOfSetBits, whichBit); 
    //bitToggle(myData, whichBit);
	myData ^= (1UL << whichBit);
}