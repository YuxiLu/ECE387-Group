/***********************************************************
File name: LCD12864RSPI.cpp
Description: Demo LCD12864 spi
***********************************************************/
#include <LCD12864RSPI.h>


extern "C" 
{
#include "Arduino.h" 
#include <inttypes.h>
#include <stdio.h>  //not needed yet
#include <string.h> //needed for strlen()
#include <avr/pgmspace.h>
}



LCD12864RSPI::LCD12864RSPI() 
{
this->DEFAULTTIME = 80; // 80 ms default time
this->delaytime = DEFAULTTIME;
} 

//*********************Delay function************************//
void LCD12864RSPI::delayns(void)
{   
delayMicroseconds(delaytime);
 }


void LCD12864RSPI::WriteByte(int dat)
{
    digitalWrite(latchPin, HIGH);
    delayns();
    shiftOut(dataPin, clockPin, MSBFIRST, dat);
    digitalWrite(latchPin, LOW);
}


void LCD12864RSPI::WriteCommand(int CMD)
{
   int H_data,L_data;
   H_data = CMD;
   H_data &= 0xf0;           //Mask the lower 4 bits of data
   L_data = CMD;             //xxxx0000format
   L_data &= 0x0f;           //Shield high 4 bits of data
   L_data <<= 4;             //xxxx0000format
   WriteByte(0xf8);          
   WriteByte(H_data);
   WriteByte(L_data);
}


void LCD12864RSPI::WriteData(int CMD)
{
   int H_data,L_data;
   H_data = CMD;
   H_data &= 0xf0;           //Mask the lower 4 bits of data
   L_data = CMD;             //xxxx0000format
   L_data &= 0x0f;           //Shield high 4 bits of data
   L_data <<= 4;             //xxxx0000format
   WriteByte(0xfa);          //RS=1��The data is written
   WriteByte(H_data);
   WriteByte(L_data);
}



void LCD12864RSPI::Initialise()
{
    pinMode(latchPin, OUTPUT);     
    pinMode(clockPin, OUTPUT);    
    pinMode(dataPin, OUTPUT);
    digitalWrite(latchPin, LOW);
    delayns();

    WriteCommand(0x30);        //Function Set the control word
    WriteCommand(0x0c);        //Displays the switch control word
    WriteCommand(0x01);        //Clear the screen control word
    WriteCommand(0x06);        //Enter the setpoint control word
}


void LCD12864RSPI::CLEAR(void)
{  
    WriteCommand(0x30);//
    WriteCommand(0x01);//Clear the display
}


void LCD12864RSPI::DisplayString(int X,int Y,uchar *ptr,int dat)
{
  int i;

  switch(X)
   {
     case 0:  Y|=0x80;break;

     case 1:  Y|=0x90;break;

     case 2:  Y|=0x88;break;

     case 3:  Y|=0x98;break;

     default: break;
   }
  WriteCommand(Y); // Positioning Displays the start address

  for(i=0;i<dat;i++)
    { 
      WriteData(ptr[i]);//Note that the Chinese character code value, two consecutive yards that a Chinese character
    }
}



void LCD12864RSPI::DisplaySig(int M,int N,int sig)
{
  switch(M)
   {
     case 0:  N|=0x80;break;

     case 1:  N|=0x90;break;

     case 2:  N|=0x88;break;

     case 3:  N|=0x98;break;

     default: break;
   }
  WriteCommand(N); // Positioning Displays the start address
  WriteData(sig);  //Outputs a single character
 }




void LCD12864RSPI::DrawFullScreen(uchar *p)
{
      int ygroup,x,y,i;
      int temp;
      int tmp;
             
      for(ygroup=0;ygroup<64;ygroup++) //Writing the half-image portion of the liquid crystal
        {                              //Write coordinates
           if(ygroup<32)
            {
             x=0x80;
             y=ygroup+0x80;
            }
           else 
            {
              x=0x88;
              y=ygroup-32+0x80;    
            }         
           WriteCommand(0x34);        //Write extended instruction command
           WriteCommand(y);           //Writes y-axis coordinates
           WriteCommand(x);           //Writes x-axis coordinates
           WriteCommand(0x30);        //Write basic command command
           tmp=ygroup*16;
           for(i=0;i<16;i++)
		 {
		    temp=p[tmp++];
		    WriteData(temp);
               }
          }
        WriteCommand(0x34);        //Write extended instruction command
        WriteCommand(0x36);        //Display the image
}


LCD12864RSPI L12864 = LCD12864RSPI();