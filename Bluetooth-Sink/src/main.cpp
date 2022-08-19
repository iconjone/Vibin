#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"


#include <Wire.h>
#include "MAX30105.h"

A2DPStream in = A2DPStream::instance() ; // A2DP input - A2DPStream is a singleton!
I2SStream out; 
uint16_t sample_rate=44100;
uint8_t channels = 2;      
//CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv 
StreamCopy copier(out, in); // copy in to out


MAX30105 particleSensor;


void setup(void) {
  Serial.begin(115200);
  //AudioLogger::instance().begin(Serial, AudioLogger::Info);
  TwoWire Wire = TwoWire(0); //may need to be 1
  Wire.begin(33,32, (uint32_t)I2C_SPEED_FAST);
  
   if (particleSensor.begin(Wire, (uint32_t)I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  else
  {
    Serial.println("MAX30105 was found. ");
  }

  // start the bluetooth audio receiver
  Serial.println("starting A2DP...");
  auto cfg = in.defaultConfig(RX_MODE);
  cfg.name = "VibinChair";
  in.begin(cfg);  
    // start I2S
  Serial.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.sample_rate = sample_rate; 
  config.channels = channels;
  config.bits_per_sample = 16;
  config.pin_bck = 25;
  config.pin_ws = 27;
  config.pin_data = 22;
  out.begin(config);

}


void loop() {
  copier.copy();
  //print the out stream to the serial port
  //Serial.print(out.peek);


}