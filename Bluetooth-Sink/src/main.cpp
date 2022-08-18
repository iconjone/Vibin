#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"



A2DPStream in = A2DPStream::instance() ; // A2DP input - A2DPStream is a singleton!
CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv 
StreamCopy copier(out, in); // copy in to out

// Arduino Setup
void setup(void) {
  Serial.begin(115200);

  // start the bluetooth audio receiver
  Serial.println("starting A2DP...");
  auto cfg = in.defaultConfig(RX_MODE);
  cfg.name = "VibinChair";
  in.begin(cfg);  
}

// Arduino loop  
void loop() {
  copier.copy();
}