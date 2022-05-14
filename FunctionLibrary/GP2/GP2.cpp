 /*
  GP2 Module library V1.0
  2017 Copyright (c) Adeept Technology Inc.  All right reserved.
*/

#include <GP2.h>

GP2::GP2(int PIN_DATA_OUT, int PIN_LED_VCC)
{
   		_PIN_DATA_OUT = PIN_DATA_OUT;
		_PIN_LED_VCC = PIN_LED_VCC;
}
/**
 * Read the GP2 sensor output voltage
*/
double GP2::getOutputV(void){
  digitalWrite(_PIN_LED_VCC, LOW);
  delayMicroseconds(DELAY_BEFORE_SAMPLING);
  double analogOutput = analogRead(_PIN_DATA_OUT);
  delayMicroseconds(DELAY_AFTER_SAMPLING);
  digitalWrite(_PIN_LED_VCC, HIGH);
  delayMicroseconds(DELAY_LED_OFF);
  //Arduino analog read value range is 0 ~ 1023, the following conversion is 0 ~ 5v
  double outputV = analogOutput / 1024 * 5;
  return outputV;
}
/**
   Calculate the dust density based on the output voltage
*/
double GP2::getDustDensity(double outputV) {
  //Output voltage and dust density conversion formula: ug / m3 = (V - 0.9) / 5 * 1000
  double ugm3 = (outputV - 0.9) / 5 * 1000;
  //Remove the undetected range
  if (ugm3 < 0) {
    ugm3 = 0;
  }
  return ugm3;
}
/**
   Calculate AQI based on dust density
   Environmental air quality index (AQI) technical requirements (Trial)
 */
double GP2::getAQI(double ugm3) {
  double aqiL = 0;
  double aqiH = 0;
  double bpL = 0;
  double bpH = 0;
  double aqi = 0;
  //According to the correspondence between pm2.5 and aqi were calculated aqi
  if (ugm3 >= 0 && ugm3 <= 35) {
    aqiL = 0;
    aqiH = 50;
    bpL = 0;
    bpH = 35;
  } else if (ugm3 > 35 && ugm3 <= 75) {
    aqiL = 50;
    aqiH = 100;
    bpL = 35;
    bpH = 75;
  } else if (ugm3 > 75 && ugm3 <= 115) {
    aqiL = 100;
    aqiH = 150;
    bpL = 75;
    bpH = 115;
  } else if (ugm3 > 115 && ugm3 <= 150) {
    aqiL = 150;
    aqiH = 200;
    bpL = 115;
    bpH = 150;
  } else if (ugm3 > 150 && ugm3 <= 250) {
    aqiL = 200;
    aqiH = 300;
    bpL = 150;
    bpH = 250;
  } else if (ugm3 > 250 && ugm3 <= 350) {
    aqiL = 300;
    aqiH = 400;
    bpL = 250;
    bpH = 350;
  } else if (ugm3 > 350) {
    aqiL = 400;
    aqiH = 500;
    bpL = 350;
    bpH = 500;
  }
  //formula: aqi = (aqiH - aqiL) / (bpH - bpL) * (desity - bpL) + aqiL;
  aqi = (aqiH - aqiL) / (bpH - bpL) * (ugm3 - bpL) + aqiL;
  return aqi;
}
/**
   According to aqi access level description
*/
String GP2::getGradeInfo(double aqi) {
  String gradeInfo;
  if (aqi >= 0 && aqi <= 50) {
    gradeInfo = String("Perfect");
  } else if (aqi > 50 && aqi <= 100) {
    gradeInfo = String("Good");
  } else if (aqi > 100 && aqi <= 150) {
    gradeInfo = String("Mild polluted");
  } else if (aqi > 150 && aqi <= 200) {
    gradeInfo = String("Medium polluted");
  } else if (aqi > 200 && aqi <= 300) {
    gradeInfo = String("Heavily polluted");
  } else if (aqi > 300 && aqi <= 500) {
    gradeInfo = String("Severely polluted");
  } else {
    gradeInfo = String("Broken roof!!!");
  }
  return gradeInfo;
}
