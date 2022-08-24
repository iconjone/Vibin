#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"



A2DPStream in = A2DPStream::instance() ; // A2DP input - A2DPStream is a singleton!
I2SStream out; 
uint16_t sample_rate=44100;
uint8_t channels = 2;      
//CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv 
StreamCopy copier(out, in); // copy in to out


void setup(void) {
  Serial.begin(115200);
  //AudioLogger::instance().begin(Serial, AudioLogger::Info);


    // start I2S
  Serial.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.sample_rate = sample_rate; 
  config.channels = channels;
  config.bits_per_sample = 16;
  config.pin_bck = 25;
  config.pin_ws = 27;
  config.pin_data = 32;
  out.begin(config);


    // start the bluetooth audio receiver
  Serial.println("starting A2DP...");
  auto cfg = in.defaultConfig(RX_MODE);
  cfg.name = "VibinChair";
  in.begin(cfg);  

}


void loop() {
  copier.copy();
  //print the out stream to the serial port
  //Serial.print(out.peek);


}