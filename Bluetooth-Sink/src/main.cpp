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

TaskHandle_t TestTask1;

void TestTask( void * pvParameters ) {
  TwoWire Wire = TwoWire(0); //may need to be 1
  Wire.begin(14,13, (uint32_t)I2C_SPEED_FAST);
  
   if (particleSensor.begin(Wire, (uint32_t)I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  else
  {
    Serial.println("MAX30105 was found. ");
  }
particleSensor.setup(); 
  for(;;) {
    Serial.println(particleSensor.getIR());
  }
}

void setup(void) {
  Serial.begin(115200);
  //AudioLogger::instance().begin(Serial, AudioLogger::Info);

  xTaskCreatePinnedToCore(
                    TestTask,   /* Task function. */
                    "TestTask1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TestTask1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 


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


