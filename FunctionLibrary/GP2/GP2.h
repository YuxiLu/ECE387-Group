 /*
  GP2 Module library V1.0
  2017 Copyright (c) Adeept Technology Inc.  All right reserved.
*/

#ifndef GP2_H
#define GP2_H

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


class GP2
{
	const int DELAY_BEFORE_SAMPLING = 280; //Waiting time before sampling
    const int DELAY_AFTER_SAMPLING = 40; //Wait time after sampling
    const int DELAY_LED_OFF = 9680; //Intervals
	int _PIN_DATA_OUT;
	int _PIN_LED_VCC;
public:
    
    double getOutputV(void);
	double getDustDensity(double outputV); 
	double getAQI(double ugm3);
	String getGradeInfo(double aqi);
		void begin(void){
		pinMode(_PIN_DATA_OUT, INPUT); //Defined as input (ADC reads analog)
        pinMode(_PIN_LED_VCC, OUTPUT); //Defined as output
		}
    GP2(int PIN_DATA_OUT, int PIN_LED_VCC);
};
#endif