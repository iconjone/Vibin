#include <Arduino.h>
#include "WiFiControl.h"
#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"

#include <Wire.h>
// #include "MAX30105.h"
#include "heartRate.h"

TaskHandle_t CoreTaskHandle;

A2DPStream in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
I2SStream out;
uint16_t sample_rate = 44100;
uint8_t channels = 2;
// CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv
StreamCopy copier(out, in); // copy in to out
  // MAX30105 particleSensor;
    AsyncWebServer httpServer(80);
  AsyncWebSocket ws("/ws");

//pin 18 

void CoreTask(void *pvParameters)
{

// const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
//   byte rates[RATE_SIZE];    // Array of heart rates
//   byte rateSpot = 0;
//   long lastBeat = 0; // Time at which the last beat occurred

//   float beatsPerMinute;
//   int beatAvg;
//    Serial.println("testing");
//     TwoWire Wire = TwoWire(0); // may need to be 1
//     Serial.println("testing");
//   Wire.begin(14, 13, (uint32_t)400000);
//   if (particleSensor.begin(Wire, (uint32_t)I2C_SPEED_FAST) == false) // Use default I2C port, 400kHz speed
//   {
//     Serial.println("MAX30105 was not found. Please check wiring/power. ");
//     while (1)
//       ;
//   }
//   else
//   {
//     Serial.println("MAX30105 was found. ");
//   }
//   particleSensor.setup();
//   particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
//   particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED
  // Wire.beginTransmission(0x20);
  // Serial.print("here");
  // Serial.println(Wire.requestFrom(0x20, 1));
  // Wire.endTransmission();


  connectWiFI();

  setUpServer(&httpServer);
  ws.onEvent(onEvent);
  httpServer.addHandler(&ws);
  MDNS.begin("vibinchair");
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  pinMode(18, OUTPUT);
   pinMode(19, OUTPUT);
   pinMode(12, OUTPUT);

  for (;;)
  {
     vTaskDelay(10);
     //loop();
    
    /*
    // Serial.println(WiFi.isConnected());
    // do something
    //delay(1000);
     long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
      // We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 120 && beatsPerMinute > 75)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
        rateSpot %= RATE_SIZE;                    // Wrap variable

        // Take average of readings
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }

    // Serial.print("IR=");
    // Serial.print(irValue);
    if (beatsPerMinute > 30)
    {
      Serial.print("BPM=");
      Serial.println(beatsPerMinute);
    }

    */
  }
}

void setup(void)
{
  Serial.begin(115200);
  // put your setup code here, to run once:

  xTaskCreatePinnedToCore(
      CoreTask,         /* Task function. */
      "CoreTaskHandle", /* name of task. */
      10000,            /* Stack size of task */
      NULL,             /* parameter of the task */
      1,                /* priority of the task */
      &CoreTaskHandle,  /* Task handle to keep track of created task */
      0);               /* pin task to core 0 */


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

void loop()
{
      yield();
   copier.copy();
  // put your main code here, to run repeatedly:
}