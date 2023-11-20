#include "Arduino.h"
#include "ArduinoFrameCRC.h"
/*
  ArduinoFrameCRC.h - Library for creating protective data frames.
  CRC16.h - Library for creating a CRC calculation.
  Created by Pawel Kaczmarczyk, 14 June 2023.
  Released into the MIT license.
*/

CRC16::CRC16()
{
    _crcValue = 0xFFFF;
}

void CRC16::Update(uint8_t data)
{
    uint16_t index = ((_crcValue >> 8) ^ data) & 0xFF;
    _crcValue = pgm_read_word_near(crcTable + index)  ^ (_crcValue << 8);;
}

uint16_t CRC16::GetCRC()
{
    return _crcValue;
}

void CRC16::Reset()
{
    _crcValue = 0xFFFF;
}


void ArduinoFrameCRC::Reset()
{
_arduinoCRC.Reset();
*fullFrame = {0};
lengthFrame = 0;
}

ArduinoFrameCRC::ArduinoFrameCRC()
{
Reset();
}

void ArduinoFrameCRC::AddData(uint8_t data)
{
fullFrame[lengthFrame] = data;
lengthFrame++;
}

void ArduinoFrameCRC::TakeWithSerial(HardwareSerial &mySerial)
{
Reset();
uint8_t tempSerialTable[256] = { 0 };
int odebraneBajty = mySerial.readBytes((char*)tempSerialTable, 256);

for(int i  = 0; i < odebraneBajty; i++)
    AddData(tempSerialTable[i]);
}

// Funkcja do tworzenia ramki
void ArduinoFrameCRC::CreateFrame()
{
uint8_t transmitFrame[10 + lengthFrame] = {0};
uint8_t checkSum = lengthFrame;

transmitFrame[0] = 0xEF;
transmitFrame[1] = 0xEF;
transmitFrame[2] = 0xEF;
transmitFrame[3] = 0xE0;
transmitFrame[4] = lengthFrame;

_arduinoCRC.Reset();
_arduinoCRC.Update(lengthFrame);

for(int i = 0; i < lengthFrame; i++)
{
    _arduinoCRC.Update(fullFrame[i]);
    transmitFrame[5 + i] = fullFrame[i];
    checkSum += fullFrame[i];
}

_arduinoCRC.Update(checkSum);

uint16_t tempCRC = _arduinoCRC.GetCRC();

transmitFrame[lengthFrame + 5] = checkSum;
transmitFrame[lengthFrame + 6] = highByte(tempCRC);
transmitFrame[lengthFrame + 7] = lowByte(tempCRC);
transmitFrame[lengthFrame + 8] = 0xE0;
transmitFrame[lengthFrame + 9] = 0xE0;


lengthFrame = lengthFrame + 10;
for(int j = 0; j < lengthFrame; j++)
    fullFrame[j] = transmitFrame[j];
}

// Funkcja do odpakowania ramki
void ArduinoFrameCRC::DecodeFrame()
{
uint8_t reciveFrame[lengthFrame] = {0};
bool bitStart = false, bitCrc = false, finallyOk = false;
uint8_t iloscDanych = 0, indexDanych = 0, j = 0, checkSum = 0;

_arduinoCRC.Reset();

for(int i = 0; i < lengthFrame - 2; i++)
{
    if(bitCrc)
    {
    _arduinoCRC.Update(checkSum);
    uint16_t tempCRC = _arduinoCRC.GetCRC();

    if((fullFrame[i] == checkSum) && (fullFrame[i + 1] == highByte(tempCRC)) && (fullFrame[i + 2] == lowByte(tempCRC)))
    {
        finallyOk = true;
        break;
    }
    }

    if(bitStart && (indexDanych + iloscDanych >= i) && !bitCrc)
    {
    _arduinoCRC.Update(fullFrame[i]);
    checkSum += fullFrame[i];

    if(indexDanych != i)
        reciveFrame[j++] = fullFrame[i];

    if(indexDanych + iloscDanych == i)
        bitCrc = true;
    }

    if((fullFrame[i] == 0xEF) && (fullFrame[i + 1] == 0xE0))
    {
    i++;
    bitStart = true;
    indexDanych = i + 1;

    iloscDanych = fullFrame[i + 1];
    }
}

if(finallyOk)
{
    lengthFrame = j;
    for(int k = 0; k < lengthFrame; k++)
    fullFrame[k] = reciveFrame[k];
}
else
    lengthFrame = 0;
}