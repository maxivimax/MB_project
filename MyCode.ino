#define CLK 4
#define DIO 5
#define butt 2
#define ledRed 11
#define ledBlue 12
#define zoomer 10
#define voltmeter A0
//Настройки
#define maxLength 30 // для настройки максимальной высоты при калибровке
#define pogreshnost 2

#define _- 0x40
#define __ 0x08

#include "GyverTM1637.h"
#include <Ultrasonic.h>
#include "GyverButton.h"

GButton butt1(butt);
void(* resetFunc) (void) = 0;
Ultrasonic ultrasonic(8, 9, 9999); // (Trig PIN,Echo PIN)
GyverTM1637 disp(CLK, DIO);

boolean flag = true;
boolean flagButton = true;
boolean flagStandUp = false;
boolean flagBeep = true;

volatile int minDistance = 0;
volatile int maxDistance = 0;
volatile int sitDowns = 0;
volatile int distance = 0;

long beepTime = millis();

void setup() {
  butt1.setClickTimeout(600);
  butt1.setTimeout(300);   
  pinMode(butt, INPUT_PULLUP);
  pinMode(voltmeter, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(zoomer, OUTPUT);

  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
  double volts = 0;
  for (int x = 0; x < 50; x++)
  {
    volts += analogRead(voltmeter);
  }
  double voltsVal = ((volts / 50) * 5) / 1024 * 2;
  disp.displayInt(voltsVal);
  delay(1000);
  runningTextHello();

  attachInterrupt(0, buttonPress, FALLING);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(zoomer, LOW);

}

void loop() {
  butt1.tick(); 

  if (butt1.isSingle()){
    s();
  }
  else{
    disp.displayByte(0x40, 0x08, 0x40, _empty);
    }
  if (butt1.isDouble()){
    d();
  }
  else{
    disp.displayByte(0x40, 0x08, 0x40, _empty);
    }
  if (butt1.isTriple()){
    f();
    delay(4000);
  }
  else{
    disp.displayByte(0x40, 0x08, 0x40, _empty);
    }
}

void LedsOn(int x)
{
  switch (x)
  {
    case 1:
      {
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledBlue, LOW);
        break;
      }
    case 2:
      {
        digitalWrite(ledRed, LOW);
        digitalWrite(ledBlue, HIGH);
        break;
      }
    case 3:
      {
        digitalWrite(ledRed, LOW);
        digitalWrite(ledBlue, LOW);
        break;
      }
  }

}

void runningTextHello() {
  byte welcome_banner[] = {0x40, 0x08, 0x40 };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 250);  // 200 это время в миллисекундах!
}



void s() {
  byte welcome_banner[] = {_S,_L,_A,_U,_A,_empty,_N,_E,_empty,_G,_A,_Y};
  disp.runningString(welcome_banner, sizeof(welcome_banner), 250);  // 200 это время в миллисекундах!
}
void d() {
  byte welcome_banner[] = {_t,_e,_S,_t };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 250);  // 200 это время в миллисекундах!
}
void f() {
  disp.displayByte(0x78, 0x4E, _empty, 0x49);
}



void runningTextGo() {
  byte welcome_banner[] = {_G, _O, _empty, _empty };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 150);  // 200 это время в миллисекундах!
}

void buttonPress () {
  tone(zoomer, 500, 50);
  if (flagButton)
  {
    if (maxDistance == 0)
      maxDistance = distance;
    else
    {
      minDistance = distance;
      flagButton = false;
    }
  }
}
