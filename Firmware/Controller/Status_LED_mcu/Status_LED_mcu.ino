#include <Arduino.h>
#include <Wire.h>

#include "Pin_Defines.h"

#define SLAVE_ADDRESS 0x01
#define REG_MAP_SIZE 14

uint16_t Reg = 0;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  //pinMode(LED5,OUTPUT);
  //pinMode(LED6,OUTPUT);
  pinMode(LED7,OUTPUT);
  pinMode(LED8,OUTPUT);
  pinMode(LED9,OUTPUT);
  pinMode(LED10,OUTPUT);
  pinMode(LED11,OUTPUT);
  pinMode(LED12,OUTPUT);
}

void loop() {
  if(Reg && (1 << 0))
  {
    digitalWrite(LED1,HIGH);
  }
  else
  {
    digitalWrite(LED1,LOW);
  }
  if(Reg && (1 << 1))
  {
    digitalWrite(LED2,HIGH);
  }
  else
  {
    digitalWrite(LED2,LOW);
  }
  if(Reg && (1 << 2))
  {
    digitalWrite(LED3,HIGH);
  }
  else
  {
    digitalWrite(LED3,LOW);
  }
  if(Reg && (1 << 3))
  {
    digitalWrite(LED4,HIGH);
  }
  else
  {
    digitalWrite(LED4,LOW);
  }
  if(Reg && (1 << 6))
  {
    digitalWrite(LED7,HIGH);
  }
  else
  {
    digitalWrite(LED7,LOW);
  }
  if(Reg && (1 << 7))
  {
    digitalWrite(LED8,HIGH);
  }
  else
  {
    digitalWrite(LED8,LOW);
  }
  if(Reg && (1 << 8))
  {
    digitalWrite(LED9,HIGH);
  }
  else
  {
    digitalWrite(LED9,LOW);
  }
  if(Reg && (1 << 9))
  {
    digitalWrite(LED10,HIGH);
  }
  else
  {
    digitalWrite(LED10,LOW);
  }
  if(Reg && (1 << 10))
  {
    digitalWrite(LED11,HIGH);
  }
  else
  {
    digitalWrite(LED11,LOW);
  }
  if(Reg && (1 << 11))
  {
    digitalWrite(LED12,HIGH);
  }
  else
  {
    digitalWrite(LED12,LOW);
  }
}

void requestEvent()
{
  //insert data you want to send here
  //Wire.send(date,number of bytes);
}

void receiveEvent(int bytesReceived)
{
  for (int a = 0;a<bytesReceived;a++)
  {
    //first byte
    if(a == 0)
    {
      Reg = Wire.receive();
    }
    //second byte
    else if(a == 1)
    {
      Reg |= Wire.receive() << 8;
    }
    //anything else
    else
    {
      Wire.receive();
    }
  }
}

