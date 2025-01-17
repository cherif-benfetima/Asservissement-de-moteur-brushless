#include <Arduino.h>

#define Ha 10
#define Hb 9
#define Hc 8

#define TLin_U 5
#define TLin_V 6
#define TLin_W 7

#define THin_U 2
#define THin_V 3
#define THin_W 4

bool a, b,c;
bool kl_U, kh_U, kl_V, kh_V, kl_W, kh_W;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(Ha, INPUT);
  pinMode(Hb, INPUT);
  pinMode(Hc, INPUT);


  pinMode(TLin_U,OUTPUT);
  pinMode(TLin_V,OUTPUT);
  pinMode(TLin_W,OUTPUT);
  
  pinMode(THin_U,OUTPUT);
  pinMode(THin_V,OUTPUT);
  pinMode(THin_W,OUTPUT);
}

void loop() {
  
  a = digitalRead(Ha);
  b = digitalRead(Hb);
  c = digitalRead(Hc);

  kl_U = not(a) and b;
  kh_U = a and not(b);

  kl_V = not(b) and c;
  kh_V = b and not(b);

  kl_W = not(c) and a;
  kh_W = not(a) and c;

  digitalWrite(THin_U,kh_U);
  digitalWrite(TLin_U,kl_U);
  
  digitalWrite(THin_V,kh_V);
  digitalWrite(TLin_V,kl_V);
  
  digitalWrite(THin_W,kh_W);
  digitalWrite(TLin_W,kl_W);
  
}
