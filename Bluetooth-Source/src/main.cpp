#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"



I2SStream i2sStream;                            // Access I2S as stream
A2DPStream a2dpStream = A2DPStream::instance(); // access A2DP as stream
VolumeStream volume(a2dpStream);
StreamCopy copier(volume, i2sStream); // copy i2sStream to a2dpStream
const int ledPin = 2;
int mutePin = 19;
int EnPin = 18;
int resetPin = 12;
int m6Pin = 26;
int m7Pin = 33;
uint16_t sample_rate = 44100;
uint8_t channels = 2;
void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(mutePin, OUTPUT);
  pinMode(EnPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(m6Pin, OUTPUT);
  pinMode(m7Pin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(mutePin, HIGH);
  digitalWrite(EnPin, HIGH);
  digitalWrite(resetPin, HIGH);
  digitalWrite(m6Pin, HIGH);
  digitalWrite(m7Pin, HIGH);

  volume.setVolume(0.3);
      Serial.println("starting A2DP...");
    auto cfgA2DP = a2dpStream.defaultConfig(TX_MODE);
    cfgA2DP.name = "JBL GO 2";
    cfgA2DP.auto_reconnect = true;

    //init a char array to be passed as a pointer to begin()
    //char *name = new char[cfgA2DP.name.length() + 1];
    a2dpStream.begin(cfgA2DP);


        // start i2s input with default configuration
    Serial.println("starting I2S...");
    a2dpStream.setNotifyAudioChange(i2sStream); // i2s is using the info from a2dp
    auto config = i2sStream.defaultConfig(RX_MODE);
    

  config.sample_rate = sample_rate;
  config.channels = channels;
  config.bits_per_sample = 16;
  config.pin_bck = 4;
  config.pin_ws = 5;
  config.pin_data = 19;

  i2sStream.begin(config);
}

void loop() {
  // put your main code here, to run repeatedly:
  copier.copy();
}