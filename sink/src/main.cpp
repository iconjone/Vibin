#include <Arduino.h>
// #include "WiFiControl.h"
#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"
#include <Wire.h>
// #include "SigmaDSP_parameters.h"

TaskHandle_t CoreTaskHandle;

A2DPStream in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
I2SStream out;
uint16_t sample_rate = 48000;
uint8_t channels = 2;
// CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv
StreamCopy copier(out, in); // copy in to out

// AsyncWebServer httpServer(80);
// AsyncWebSocket ws("/ws");

  // TwoWire Wire = TwoWire(0); // may need to be 1
  // SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f, 21); //DSP_I2C_ADDRESS

// pin 18


#define DSP_DVDD 23
#define DSP_RESET 21

void CoreTask(void *pvParameters)
{

  // connectWiFI();

  // setUpServer(&httpServer);
  // ws.onEvent(onEvent);
  // httpServer.addHandler(&ws);
  // MDNS.begin("vibinchair");
  // httpServer.begin();
  // MDNS.addService("http", "tcp", 80);


Wire.begin(14, 13, (uint32_t)400000);
  // dsp.begin();

    Serial.println(F("Pinging i2c lines...\n0 -> deveice is present\n2 -> device is not present"));
  Serial.print(F("DSP response: "));
  // Serial.println(dsp.ping());

    Serial.print(F("\nLoading DSP program... "));
  // loadProgram(dsp);
  Serial.println("Done!\n");

  for (;;)
  {
  Serial.println(F("Confused"));
  delay(1000);
  }

}

void setup(void)
{
  Serial.begin(115200);

   pinMode(DSP_DVDD, INPUT);
 pinMode(DSP_RESET, INPUT);
//  pinMode(23, OUTPUT);
//  pinMode(21, OUTPUT);
// digitalWrite(23, HIGH);
// digitalWrite(21, HIGH);

  // xTaskCreatePinnedToCore(
  //     CoreTask,         /* Task function. */
  //     "CoreTaskHandle", /* name of task. */
  //     10000,            /* Stack size of task */
  //     NULL,             /* parameter of the task */
  //     7,                /* priority of the task */
  //     &CoreTaskHandle,  /* Task handle to keep track of created task */
  //     0);               /* pin task to core 0 */

    // start the bluetooth audio receiver
    


      Serial.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.sample_rate = sample_rate;
  config.channels = channels;
  config.bits_per_sample = 16;
  config.pin_bck = 25;
  config.pin_ws = 27;
  config.pin_data = 32;
  out.begin(config);

    Serial.println("starting A2DP...");
  auto cfg = in.defaultConfig(RX_MODE);
  cfg.name = "VibinChair";
  in.begin(cfg);


}

void loop()
{
  // yield();
  copier.copy();
  // put your main code here, to run repeatedly:
}