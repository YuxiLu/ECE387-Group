/*
  BMP180 Module library V1.0
  2017 Copyright (c) Adeept Technology Inc.  All right reserved.
*/
#ifndef BMP180_H
#define BMP180_H

#include <Arduino.h>
#include <Wire.h>



class BMP180
{
private:
  
  const unsigned char OSS = 0;  // Oversampling Setting
  int _BMP180_ADDRESS;

  // Calibration values
  int ac1;
  int ac2;
  int ac3;
  unsigned int ac4;
  unsigned int ac5;
  unsigned int ac6;
  int b1;
  int b2;
  int mb;
  int mc;
  int md;
  // b5 is calculated in bmp180GetTemperature(...), this variable is also used in bmp180GetPressure(...)
  // so ...Temperature(...) must be called before ...Pressure(...).
  long b5;

public:

  float calcAltitude(float pressure);
  int readRegister(int deviceAddress, byte address);
  void writeRegister(int deviceAddress, byte address, byte val);
  // Read 2 bytes from the BMP180
  // First byte will be from 'address'
  // Second byte will be from 'address'+1
  int ReadInt(int address);
  // Read 1 byte from the BMP180 at 'address'
  unsigned char Read(int address);
  // Stores all of the bmp180's calibration values into global variables
  // Calibration values are required to calculate temp and pressure
  // This function should be called at the beginning of the program
  void Calibration();
  float GetTemperature();// Calculate temperature in deg C
  
  long GetPressure();// Calculate pressure given up
                                           // calibration values must be known
                                           // b5 is also required so bmp180GetTemperature(...) must be called first.
                                           // Value returned will be pressure in units of Pa.
  void begin(){  Wire.begin(); Calibration();};  // Initialize  adressing
  BMP180(int BMP180_ADDRESS);
 
  
};

#endif