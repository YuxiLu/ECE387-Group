/*
File name:  WeatherBox.c
ECE387 Group Project
Team Members:
YuxiLu
MingshauiSong
RuiYang
ZhilongYu
*/
#include <LCD12864RSPI.h>
#include <DHT11.h>
#include <BMP180.h>
#include <GP2.h>

DHT11 dht11;
BMP180 bmp180(0x77);
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )
int PIN_DATA_OUT = 2;
int PIN_LED_VCC = 4;
GP2 gp2(PIN_DATA_OUT, PIN_LED_VCC);

/*Set the alarm value*/
int humidityThreshold = 60;
int temperatureThreshold = 30;
int gasThreshold = 500;
/*Time setting*/
unsigned long oldTime=0;
unsigned long newTime;
unsigned long time;
int Time = 17;
int Minute = 30;
int Second = 00;

/*Pin connects*/
#define DHT11PIN  2
int redPin = 11;
int greenPin = 10;
int bluePin = 6;
int gassensorSPin = 7;
int gassensorAPin = 1;
int passiveBuzzerPin=5;
int blueButton1Pin=13;
int blueButton2Pin=12;
int redButtonPin=1;
int whiteButtonPin=0;
int photoresistorPin = 0;

/*LCD show info*/
unsigned char show0[] = "    :   :  ";  
unsigned char show1[] = " Weather box ";  
unsigned char show2_1[] = "Yuxi Lu";  
unsigned char show2_2[] = "MingshuaiSong";  
unsigned char show2_3[] = "Rui Yang";
unsigned char show2_4[] = "Zhilong Yu";
unsigned char show4[] = "Shutting down.";    
unsigned char show5[] = "Shutting down..";    
unsigned char show6[] = "Shutting down...";    
unsigned char show7[] = " Humi:      %RH";      
unsigned char show8[] = " Temp:      C  ";    
unsigned char show9[] = " Page:  1/4 ";  
unsigned char show10[] = "Pres:        Pa";
unsigned char show11[] = "  Atm:          ";
unsigned char show12[] = " Page:  2/4 ";
unsigned char show13[] = "Smoke/gas: ";
unsigned char show14[] = "Light:";
unsigned char show15[] = " Page:  3/4 ";
unsigned char show16[] = "GP2valt:      V";
unsigned char show17[] = "  ugm3:";
unsigned char show18[] = " Page:  4/4 ";  
 
/*Miami logo*/
unsigned char logo0 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xC0, 0x03, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE0, 0x07, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xF0, 0x0F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x3F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFE, 0x7F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0xFB, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xCF, 0xF3, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE7, 0xE7, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xF1, 0xCF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x3F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFE, 0x7F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x00, 0x0C, 0x0C, 0x01, 0x80, 0x30, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x0C, 0x01, 0x80, 0x38, 0x01, 0x80, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xC0, 0x2C, 0x0C, 0x02, 0xC0, 0x3C, 0x02, 0x80, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x02, 0x60, 0x4C, 0x0C, 0x04, 0x60, 0x26, 0x04, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x02, 0x30, 0x8C, 0x0C, 0x0F, 0xF0, 0x23, 0x08, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x02, 0x19, 0x0C, 0x0C, 0x08, 0x10, 0x21, 0x90, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x02, 0x06, 0x04, 0x0C, 0x10, 0x18, 0x20, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x02, 0x00, 0x0E, 0x0C, 0x30, 0x0C, 0x60, 0x40, 0xE0, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0x01, 0x83, 0x00, 0x60, 0xC3, 0x01, 0x87, 0xF8, 0x6F, 0x06, 0x61, 0x8F, 0xFE, 0x70, 0xE0,
0x02, 0x00, 0x82, 0xC0, 0x60, 0xC1, 0x81, 0x06, 0x00, 0x61, 0x88, 0x01, 0x80, 0x40, 0x18, 0x80,
0x02, 0x00, 0x82, 0x70, 0x60, 0xC0, 0x83, 0x06, 0x00, 0x61, 0x8C, 0x01, 0x80, 0x40, 0x0D, 0x00,
0x02, 0x00, 0x82, 0x18, 0x60, 0xC0, 0xC2, 0x07, 0xF0, 0x7E, 0x03, 0xC1, 0x80, 0x40, 0x06, 0x00,
0x02, 0x00, 0x82, 0x06, 0x60, 0xC0, 0x64, 0x06, 0x00, 0x6C, 0x00, 0x71, 0x80, 0x40, 0x06, 0x00,
0x03, 0x01, 0x02, 0x01, 0xE0, 0xC0, 0x38, 0x06, 0x00, 0x63, 0x00, 0x31, 0x80, 0x40, 0x06, 0x00,
0x01, 0xC6, 0x02, 0x00, 0x60, 0xC0, 0x18, 0x07, 0xF8, 0x60, 0xCE, 0x61, 0x80, 0xE0, 0x06, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int page = 0;
int staticDisplay = 0;

/*Set pin input and output*/
void setup()
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(gassensorAPin, INPUT);
    pinMode(passiveBuzzerPin,OUTPUT);
    pinMode(blueButton1Pin,INPUT);
    pinMode(blueButton2Pin,INPUT);
    pinMode(redButtonPin,INPUT);
    pinMode(whiteButtonPin,INPUT);
    pinMode(photoresistorPin, INPUT);
    bmp180.begin();
    //turn off LED
    color(0,0,0);
    //initialized the sreen
    L12864.Initialise();
    delay(100);
    //clean the screen
    L12864.CLEAR();
    delay(100);
    dht11.read(DHT11PIN);
    delay(100);
    gp2.begin();
}

/*Loop method*/
void loop()
{
   keyDetection();
   showPage();
   delay(100);
}

/*Show all information on LCD screen*/
void bootInitialization()
{
    L12864.DrawFullScreen(logo0);
    delay(2000);
    L12864.CLEAR();
    delay(100);
    L12864.DisplayString(1,1,show1,AR_SIZE(show1));
    delay(2000);
    L12864.CLEAR();
    delay(100);
    L12864.DisplayString(0,0,show2_1,AR_SIZE(show2_1));
    delay(100);
    L12864.DisplayString(1,0,show2_2,AR_SIZE(show2_2));
    delay(100);
    L12864.DisplayString(2,0,show2_3,AR_SIZE(show2_3));
    delay(100);
    L12864.DisplayString(3,0,show2_4,AR_SIZE(show2_4));
    delay(2000);
    L12864.CLEAR();
    delay(100);
}

/*Key Detection*/
void keyDetection()
{
    if(digitalRead(whiteButtonPin)==LOW){
       delay(10);
       if(digitalRead(whiteButtonPin)==LOW){
           while(digitalRead(whiteButtonPin)==LOW);
           L12864.CLEAR();
           delay(100);
           //help to prevent the repeated shutdown
           if(page!=0){
            //show4 show5 show6 info display
              for(int i=0;i<2;i++){
                  L12864.DisplayString(1,0,show4,AR_SIZE(show4));
                  delay(1000);
                  L12864.DisplayString(1,0,show5,AR_SIZE(show5));
                  delay(1000);
                  L12864.DisplayString(1,0,show6,AR_SIZE(show6));
                  delay(1000);
              }
              //clear the sreen
              L12864.CLEAR();
              delay(100);
           }
           //turn off LED
           color(0,0,0);
           page = 0;
        }
     }
     if(digitalRead(redButtonPin)==LOW){
        delay(10);
        if(digitalRead(redButtonPin)==LOW){
           while(digitalRead(redButtonPin)==LOW);
           L12864.CLEAR();
           delay(100);
           bootInitialization();
           staticDisplay = 0;
           page = 1;
        }
     }
     if(digitalRead(blueButton1Pin)==LOW){
        delay(10);
        if(digitalRead(blueButton1Pin)==LOW){
           while(digitalRead(blueButton1Pin)==LOW);
           if(page!=0){
              L12864.CLEAR();
              delay(100);
              staticDisplay = 0;
              page = page - 1;
              if(page <= 1){
                 page = 1;
               }
            }
        }
      }
     if(digitalRead(blueButton2Pin)==LOW){
        delay(10);
        if(digitalRead(blueButton2Pin)==LOW){
           while(digitalRead(blueButton2Pin)==LOW);
           L12864.CLEAR();
           delay(100);
           staticDisplay = 0;
           if(page!=0){
              page = page + 1;
              if(page >= 4){
                 page = 4;
              }
           }
        }
      }      
}

/*Show page informatiom*/
void showPage()
{
    double temphumidity = 0;
    double tempTemperature = 0;
    double dataphotoresistor = 0;
    char strhumidity[2];
    char strTemperature[2];
    char strPressure[5];
    char strAtm[5];
    char strphotoresistor[4];
    float dataTemperature = 0;
    float dataPressure = 0;
    float dataAtm = 0;
    double datagassensor = 0;
    char strgassensor[5];
    double tempOutputV = 0;
    double tempUgm3 = 0;
    char strUgm3[5];
    char strOutputV[5];
    char strSecond[2];
    char strMinute[2];
    char strTime[2];
    newTime = millis();
    time = (newTime - oldTime)/1000;
    if(time>=1){oldTime = newTime;Second = Second + time; }
    if(Second >= 60){ Second = 0; Minute++; }
    if(Minute>=60){ Minute = 0;Time++; }
    if(Time>=24){ Time = 0;}
    dht11.read(DHT11PIN);
    temphumidity = dht11.humidity;
    tempTemperature = dht11.temperature;
    datagassensor = analogRead(gassensorAPin);
    dataphotoresistor = analogRead(photoresistorPin);
    if(page != 0){
       if(temphumidity>humidityThreshold||tempTemperature>temperatureThreshold||datagassensor>gasThreshold){
        //turn on red LED
          color(255, 0, 0);
          tone(passiveBuzzerPin,500);
          delay(100);
         //turn off the buzzer
          noTone(passiveBuzzerPin);
       }else{
        //turn on green LED
          color(0,255, 0);
       }
       //convert float into string
       dtostrf(Time,2,0,strTime);
       dtostrf(Minute,2,0,strMinute);
       dtostrf(Second,2,0,strSecond);
       L12864.DisplayString(0,1,(unsigned char *)strTime,2);
       L12864.DisplayString(0,3,(unsigned char *)strMinute,2);
       L12864.DisplayString(0,5,(unsigned char *)strSecond,2);
    }
    if(page == 0){
      //turn off red LED
       color(0,0, 0);
    }
    switch(page){
    case 1:  
           if(staticDisplay == 0){
            //Display: Humi
              L12864.DisplayString(1,0,(unsigned char *)show7,AR_SIZE(show7));
              delay(10);
              //Display temperature
              L12864.DisplayString(2,0,(unsigned char *)show8,AR_SIZE(show8));
              delay(10);
              //Display page number
              L12864.DisplayString(3,0,(unsigned char *)show9,AR_SIZE(show9));
              delay(10);
              L12864.DisplayString(0,0,(unsigned char *)show0,AR_SIZE(show0));
              delay(10);
              staticDisplay = 1;
           }
           dtostrf(temphumidity,5,2,strhumidity);
           //Display humidity data
           L12864.DisplayString(1,3,(unsigned char *)strhumidity,6);
           dtostrf(tempTemperature,5,2,strTemperature);
           //Display temperature data
           L12864.DisplayString(2,3,(unsigned char *)strTemperature,6);
           break;
    case 2:
           if(staticDisplay == 0){
             //Display: Pres:        Pa
              L12864.DisplayString(1, 0, (unsigned char *)show10, AR_SIZE(show10));
              delay(10);
              //Display: Atm:      
              L12864.DisplayString(2, 0, (unsigned char *)show11, AR_SIZE(show11));
              delay(10);
              //Display: page number
              L12864.DisplayString(3, 0, (unsigned char *)show12, AR_SIZE(show12));
              delay(10);
              //Display "    :   :"
              L12864.DisplayString(0,0,(unsigned char *)show0,AR_SIZE(show0));
              delay(10);
              staticDisplay = 1;
           }
           dataTemperature = bmp180.GetTemperature();
           dataPressure = bmp180.GetPressure();
           dataAtm = dataPressure / 101325;
           dtostrf(dataPressure, 5, 2, strPressure);
           //Display pressure data
           L12864.DisplayString(1, 3, (unsigned char *)strPressure, 6);
           dtostrf(dataAtm, 5, 4, strAtm);
           //Display standard atmosphere data
           L12864.DisplayString(2, 3, (unsigned char *)strAtm, 6);
           break;
    case 3:
           if(staticDisplay == 0){
            //Display: Smoke/gas:
              L12864.DisplayString(1, 0, (unsigned char *)show13, AR_SIZE(show13));
              delay(10);
              //Display: Light:
              L12864.DisplayString(2, 0, (unsigned char *)show14, AR_SIZE(show14));
              delay(10);
              //Display: page number
              L12864.DisplayString(3, 0, (unsigned char *)show15, AR_SIZE(show15));
              delay(10);
              //Display "    :   :"
              L12864.DisplayString(0,0,(unsigned char *)show0,AR_SIZE(show0));
              delay(10);
              staticDisplay = 1;
           }
           dtostrf(datagassensor,4,0,strgassensor);
           L12864.DisplayString(1,5,(unsigned char *)strgassensor,4);
           dtostrf(dataphotoresistor,4,0,strphotoresistor);
           //Display photoresistor data
           L12864.DisplayString(2,3,(unsigned char *)strphotoresistor,4);
           break;
    case 4:
          if(staticDisplay == 0){
            //Display: GP2valt:     V
              L12864.DisplayString(1, 0, (unsigned char *)show16, AR_SIZE(show16));
              delay(10);
              //Display: ugm3:
              L12864.DisplayString(2, 0, (unsigned char *)show17, AR_SIZE(show17));
              delay(10);
              //Display: page number
              L12864.DisplayString(3, 0, (unsigned char *)show18, AR_SIZE(show18));
              delay(10);
              //Display "    :   :"
              L12864.DisplayString(0, 0, (unsigned char *)show0,AR_SIZE(show0));
              delay(10);
              staticDisplay = 1;
           }
           tempOutputV = gp2.getOutputV();
           dtostrf(tempOutputV,5,2,strOutputV);
           //Display GP2 valtage data
           L12864.DisplayString(1,4,(unsigned char *)strOutputV,6);
           //Calculate the dust concentration
           tempUgm3 = gp2.getDustDensity(tempOutputV);
           dtostrf(tempUgm3,5,2,strUgm3);
           //Display ugm3 data
           L12864.DisplayString(2,4,(unsigned char *)strUgm3,6);
          break;
    default:break;
    }
}

/* generate the color and write PWM signal output*/
void color (unsigned char red, unsigned char green, unsigned char blue)
{    
     analogWrite(redPin, 255-red);
     analogWrite(greenPin, 255-green);
     analogWrite(bluePin, 255-blue);
} 
