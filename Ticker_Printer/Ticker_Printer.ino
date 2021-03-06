#include "Queue.h"

#define dly 100

const static byte charmap[95][5] = {{0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x7D, 0x00, 0x00}, {0x00, 0x70, 0x00, 0x70, 0x00},//  !"
                                    {0x14, 0x7F, 0x14, 0x7F, 0x14}, {0x12, 0x2A, 0x7F, 0x2A, 0x24}, {0x62, 0x64, 0x08, 0x13, 0x23},// #$%
                                    {0x36, 0x49, 0x35, 0x02, 0x05}, {0x08, 0x70, 0x60, 0x00, 0x00}, {0x00, 0x1C, 0x22, 0x41, 0x00},// &'(
                                    {0x00, 0x41, 0x22, 0x1C, 0x00}, {0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, {0x08, 0x08, 0x7F, 0x08, 0x08},// )*+
                                    {0x01, 0x0E, 0x0C, 0x00, 0x00}, {0x08, 0x08, 0x08, 0x08, 0x08}, {0x00, 0x03, 0x03, 0x00, 0x00},// ,-.
                                    {0x02, 0x04, 0x08, 0x10, 0x20}, {0x3E, 0x45, 0x49, 0x51, 0x3E}, {0x00, 0x21, 0x7F, 0x01, 0x00},// /01
                                    {0x27, 0x49, 0x49, 0x49, 0x31}, {0x22, 0x41, 0x49, 0x49, 0x36}, {0x0C, 0x14, 0x24, 0x7F, 0x04},// 234
                                    {0x72, 0x51, 0x51, 0x51, 0x4E}, {0x1E, 0x29, 0x49, 0x49, 0x06}, {0x43, 0x44, 0x48, 0x50, 0x60},// 567
                                    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x30, 0x49, 0x49, 0x4A, 0x3C}, {0x00, 0x36, 0x36, 0x00, 0x00},// 89:
                                    {0x01, 0x6E, 0x6C, 0x00, 0x00}, {0x08, 0x14, 0x22, 0x41, 0x00}, {0x14, 0x14, 0x14, 0x14, 0x14},// ;<=
                                    {0x00, 0x41, 0x22, 0x14, 0x08}, {0x20, 0x40, 0x45, 0x48, 0x30}, {0x26, 0x49, 0x4F, 0x41, 0x4E},// >?@
                                    {0x1F, 0x24, 0x44, 0x24, 0x1F}, {0x41, 0x7F, 0x49, 0x49, 0x36}, {0x3E, 0x41, 0x41, 0x41, 0x22},// ABC
                                    {0x41, 0x7F, 0x41, 0x41, 0x3E}, {0x7F, 0x49, 0x49, 0x41, 0x41}, {0x7F, 0x48, 0x48, 0x40, 0x40},// DEF
                                    {0x3E, 0x41, 0x41, 0x49, 0x4F}, {0x7F, 0x08, 0x08, 0x08, 0x7F}, {0x00, 0x41, 0x7F, 0x41, 0x00},// GHI
                                    {0x02, 0x01, 0x01, 0x01, 0x7E}, {0x7F, 0x08, 0x14, 0x22, 0x41}, {0x7F, 0x01, 0x01, 0x01, 0x01},// JKL
                                    {0x7F, 0x20, 0x18, 0x20, 0x7F}, {0x7F, 0x20, 0x10, 0x08, 0x7F}, {0x3E, 0x41, 0x41, 0x41, 0x3E},// MNO
                                    {0x7F, 0x48, 0x48, 0x48, 0x30}, {0x3E, 0x41, 0x45, 0x42, 0x3D}, {0x7F, 0x48, 0x4C, 0x4A, 0x31},// PQR
                                    {0x32, 0x49, 0x49, 0x49, 0x26}, {0x40, 0x40, 0x7F, 0x40, 0x40}, {0x7E, 0x01, 0x01, 0x01, 0x7E},// STU
                                    {0x70, 0x0C, 0x03, 0x0C, 0x70}, {0x7F, 0x02, 0x04, 0x02, 0x7F}, {0x63, 0x14, 0x08, 0x14, 0x63},// VWX
                                    {0x60, 0x10, 0x0F, 0x10, 0x60}, {0x43, 0x45, 0x49, 0x51, 0x61}, {0x00, 0x7F, 0x41, 0x41, 0x00},// YZ[
                                    {0x20, 0x10, 0x08, 0x04, 0x02}, {0x00, 0x41, 0x41, 0x7F, 0x00}, {0x10, 0x20, 0x40, 0x20, 0x10},// \]^
                                    {0x01, 0x01, 0x01, 0x01, 0x01}, {0x00, 0x00, 0x70, 0x68, 0x00}, {0x02, 0x15, 0x15, 0x15, 0x0F},// _`a
                                    {0x7F, 0x0A, 0x11, 0x11, 0x0E}, {0x0E, 0x11, 0x11, 0x11, 0x0A}, {0x0E, 0x11, 0x11, 0x0A, 0x7F},// bcd
                                    {0x0E, 0x15, 0x15, 0x15, 0x0C}, {0x00, 0x08, 0x3F, 0x48, 0x20}, {0x08, 0x14, 0x15, 0x15, 0x1E},// efg
                                    {0x7F, 0x08, 0x10, 0x10, 0x0F}, {0x00, 0x11, 0x5F, 0x01, 0x00}, {0x02, 0x01, 0x01, 0x01, 0x5E},// hij
                                    {0x7F, 0x04, 0x0A, 0x11, 0x00}, {0x00, 0x41, 0x7F, 0x01, 0x00}, {0x1F, 0x10, 0x0F, 0x10, 0x0F},// klm
                                    {0x1F, 0x08, 0x10, 0x10, 0x0F}, {0x0E, 0x11, 0x11, 0x11, 0x0E}, {0x1F, 0x0C, 0x12, 0x12, 0x0C},// nop
                                    {0x08, 0x14, 0x14, 0x1F, 0x01}, {0x1F, 0x08, 0x10, 0x10, 0x08}, {0x09, 0x15, 0x15, 0x15, 0x12},// qrs
                                    {0x10, 0x10, 0x7F, 0x11, 0x12}, {0x1E, 0x01, 0x01, 0x02, 0x1F}, {0x1C, 0x02, 0x01, 0x02, 0x1C},// tuv
                                    {0x1E, 0x01, 0x06, 0x01, 0x1E}, {0x11, 0x0A, 0x04, 0x0A, 0x11}, {0x18, 0x05, 0x05, 0x05, 0x1E},// wxy
                                    {0x11, 0x13, 0x15, 0x19, 0x11}, {0x00, 0x08, 0x36, 0x41, 0x00}, {0x00, 0x00, 0x77, 0x00, 0x00},// z{|
                                    {0x00, 0x41, 0x36, 0x08, 0x00}, {0x20, 0x40, 0x20, 0x10, 0x20}};                               // }~

void setup() {
  Serial.begin(9600);
  for(int i = 2; i >= 9; i++)
  {
    pinMode(i, OUTPUT);
  }
}
byte c = 0;
Queue<char> queue = Queue<char>(512);
void loop() {
  if(queue.count()>0)
  {
    char in=queue.pop();
    Serial.println((char)in);
    c = in - 0x20;
    //Serial.println(c);
    for(int col = 0; col < 5; col++)
    {
      for(int row = 0; row < 7; row++)
      {
        if((charmap[c][col] >> row) & 1)
        {
          Serial.print("*");
          digitalWrite(row+2, HIGH);
        }
        else
        {
          Serial.print(" ");
          digitalWrite(row+2, LOW);
        }
      }
      delay(dly/2);
      Serial.print("\n");
      for(int i = 2; i <= 8; i++)
      {
        digitalWrite(i,LOW);
      }
      digitalWrite(9, HIGH);
      delay(dly/2);
      digitalWrite(9, LOW);
    }
    for(int row = 0; row < 7; row++)
    {
      Serial.print(" ");
      digitalWrite(row+2, LOW);
    }
    delay(dly);
    Serial.print("\n");
    digitalWrite(9, HIGH);
    delay(dly/2);
    digitalWrite(9, LOW);
  }
}

void serialEvent(){
  while(Serial.available())
  {
    char in=Serial.read();
    queue.push(in);
    delayMicroseconds(700);
  }
}


