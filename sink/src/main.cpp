#include <Arduino.h>
#include "WiFiControl.h"
// #include "AudioTools.h"
// #include "AudioLibs/AudioA2DP.h"

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#include <Adafruit_INA260.h>

Adafruit_INA260 ina260 = Adafruit_INA260();

TaskHandle_t CoreTaskHandle;

// A2DPStream in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
// I2SStream out;
// uint16_t sample_rate = 44100;
// uint8_t channels = 2;
// // CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv
// StreamCopy copier(out, in); // copy in to out
MAX30105 particleSensor;
AsyncWebServer httpServer(80);
AsyncWebSocket ws("/ws");

// pin 18

void CoreTask(void *pvParameters)
{

  const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
  byte rates[RATE_SIZE];    // Array of heart rates
  byte rateSpot = 0;
  long lastBeat = 0; // Time at which the last beat occurred

  float beatsPerMinute;
  float oldBPM = 0;
  int beatAvg;
  Serial.println("testing");
  TwoWire Wire = TwoWire(0); // may need to be 1
  Serial.println("testing");
  Wire.begin(14, 13, (uint32_t)400000);
  if (particleSensor.begin(Wire, (uint32_t)I2C_SPEED_FAST) == false) // Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }
  else
  {
    Serial.println("MAX30105 was found. ");
  }

  if (!ina260.begin())
  {
    Serial.println("Couldn't find INA260 chip");
    while (1)
      ;
  }
  else
  {
    Serial.println("Found INA260 chip");
  }
  delay(500);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED

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

  float oldCurrent = 0;
  float oldVoltage = 0;
  int counter = 0;

  for (;;)
  {
    counter++;
    // loop();

    // Serial.println(WiFi.isConnected());
    // do something
    // delay(1000);

    long irValue = particleSensor.getIR();
    // Serial.println(irValue);
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

    if (beatsPerMinute > 70 && beatsPerMinute < 110)
    {
      if (oldBPM != beatsPerMinute)
      {
        String bpmMessage = "{\"type\":\"command\", \"origin\":\"device\",\"nId\":0,\"data\": \"bpm-" + String(beatsPerMinute) + "\"}";
        ws.textAll(bpmMessage);
        oldBPM = beatsPerMinute;
      }
    }

    float voltage = ina260.readBusVoltage();
    // float current = 0;
    // float voltage = 0;

    // generate random number
    if (counter > 1000)
    {
      Serial.println("reading");
      float current = ina260.readCurrent();
      if (current != oldCurrent && current != 0.0 )
      {
        String currentMessage = "{\"type\":\"command\", \"origin\":\"device\",\"nId\":0,\"data\": \"ma-&" + String(current) + "\"}";
        ws.textAll(currentMessage);
        oldCurrent = current;
        // Serial.print("Current: ");
        // Serial.println(current);
      }
    }
    if (counter > 1000)
    {
      counter = 0;
      float voltage = ina260.readBusVoltage();
      if (voltage != oldVoltage && voltage != 0.0 )
      {
        String voltageMessage = "{\"type\":\"command\", \"origin\":\"device\",\"nId\":0,\"data\": \"v-&" + String(voltage) + "\"}";
        ws.textAll(voltageMessage);
        oldVoltage = voltage;
        // Serial.print("Voltage: ");
        // Serial.println(voltage);
      }
    }
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

  // Serial.println("starting I2S...");
  //   auto config = out.defaultConfig(TX_MODE);
  //   config.sample_rate = sample_rate;
  //   config.channels = channels;
  //   config.bits_per_sample = 16;
  //   config.pin_bck = 25;
  //   config.pin_ws = 27;
  //   config.pin_data = 32;
  //   out.begin(config);

  //     // start the bluetooth audio receiver
  //   Serial.println("starting A2DP...");
  //   auto cfg = in.defaultConfig(RX_MODE);
  //   cfg.name = "VibinChair";
  //   in.begin(cfg);
}

void loop()
{
  // yield();
  //  copier.copy();
  // put your main code here, to run repeatedly:
}