#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

A2DPStream in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
I2SStream out;
uint16_t sample_rate = 44100;
uint8_t channels = 2;
// CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv
StreamCopy copier(out, in); // copy in to out

MAX30105 particleSensor;

TaskHandle_t I2CTaskHandle;

AsyncWebServer httpServer(80);

// setup led pin 2
const int ledPin = 2;

bool ledState = false;
void I2CTask(void *pvParameters)
{
  pinMode(ledPin, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Vibin", "vibinon1");
  Serial.println("WiFi started");
  // print ip address
  Serial.println(WiFi.localIP());

  httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  request->send_P(200, "text/plain", "Hello, world");
                  Serial.println("Hello");
                  ledState = !ledState;
                  digitalWrite(ledPin, ledState ? HIGH : LOW);
                });
  httpServer.begin();
  // core 0
  const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
  byte rates[RATE_SIZE];    // Array of heart rates
  byte rateSpot = 0;
  long lastBeat = 0; // Time at which the last beat occurred

  float beatsPerMinute;
  int beatAvg;
  TwoWire Wire = TwoWire(0); // may need to be 1
  Wire.begin(14, 13, (uint32_t)I2C_SPEED_FAST);

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
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED
  for (;;)
  {
    // httpServer.
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
      // We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)
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
    Serial.print(", BPM=");
    Serial.println(beatsPerMinute);
    // Serial.print(", Avg BPM=");
    // Serial.print(beatAvg);

    // if (irValue < 50000)
    //   Serial.print(" No finger?");

    // Serial.println();
  }
}

void setup(void)
{
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  xTaskCreatePinnedToCore(
      I2CTask,         /* Task function. */
      "I2CTaskHandle", /* name of task. */
      10000,           /* Stack size of task */
      NULL,            /* parameter of the task */
      2,               /* priority of the task */
      &I2CTaskHandle,  /* Task handle to keep track of created task */
      0);              /* pin task to core 0 */

  // xTaskCreatePinnedToCore(
  //                   WiFiTask,   /* Task function. */
  //                   "WiFiTaskHandle",     /* name of task. */
  //                   10000,       /* Stack size of task */
  //                   NULL,        /* parameter of the task */
  //                   1,           /* priority of the task */
  //                   &WiFiTaskHandle,      /* Task handle to keep track of created task */
  //                   0);          /* pin task to core 0 */

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

void loop()
{
  // core 1
  copier.copy();
  // print the out stream to the serial port
  // Serial.print(out.peek);
}
