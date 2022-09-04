#include <Arduino.h>
const int ledPin = 2;
int mutePin = 19;
int EnPin = 18;
int resetPin = 12;
int m6Pin = 26;
int m7Pin = 33;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(mutePin, OUTPUT);
  pinMode(EnPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(m6Pin, OUTPUT);
  pinMode(m7Pin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(mutePin, LOW);
  digitalWrite(EnPin, LOW);
  digitalWrite(resetPin, LOW);
  digitalWrite(m6Pin, LOW);
  digitalWrite(m7Pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}