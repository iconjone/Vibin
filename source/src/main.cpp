#include <Arduino.h>
#include <Wire.h>
#include "WiFiControl.h"
#include "SigmaStudioFW.h"
#include "USER_SETTINGS.h"
#include "TESTDSP_IC_1.h"
#include "TESTDSP_IC_1_PARAM.h"
#include "MerusAudio.h"

#define DEBUG true
#define Serial \
  if (DEBUG)   \
  Serial

TaskHandle_t CoreTaskHandle;

AsyncWebServer httpServer(80);
AsyncWebSocket ws("/ws");

#define DSP_DVDD 23
#define DSP_RESET 21

#define MA_NENABLE_IO 18 
#define MA_NMUTE_IO 19  

void CoreTask(void *pvParameters)
{
  Serial.println("Providing Power to DSP...");
  digitalWrite(DSP_DVDD, HIGH);

  Serial.println("Starting I2C bus...");
  Wire.begin(14, 13, (uint32_t)400000);

  delay(100);

  Serial.println("Resetting DSP...");
  digitalWrite(DSP_RESET, LOW);
  delay(10);
  digitalWrite(DSP_RESET, HIGH);

  delay(560);

  Serial.println("Downloading DSP program...");
  default_download_IC_1();

  Serial.println("Setting up Amp...");
  MAAudio amp1(0x20);
  MAAudio amp2(0x21);
  MAAudio amp3(0x22);

  Serial.println("Turning on Amps... Disabled & Mute");
  digitalWrite(MA_NMUTE_IO, LOW);
  digitalWrite(MA_NENABLE_IO, HIGH);
  delay(10);
  digitalWrite(MA_NENABLE_IO, LOW);
  delay(10);
  Serial.println("Amps Enabled...");

  Serial.println("Setting up Registers...");
  amp1.setup_ma120x0();
  Serial.println("Amp 1 Done");
  amp2.setup_ma120x0();
  Serial.println("Amp 2 Done");
  amp3.setup_ma120x0();
  Serial.println("Amp 3 Done");

  Serial.println("Amps Enabled & Unmuted...");
  digitalWrite(MA_NMUTE_IO, HIGH);

  Serial.println("Modules Setup Complete!");

  // for (;;)
  // {

  // }
}

void setup(void)
{
  Serial.begin(115200);

  Serial.println("Source On: Setting Up Modules...");

  pinMode(DSP_DVDD, OUTPUT);
  pinMode(DSP_RESET, OUTPUT);
  pinMode(MA_NENABLE_IO, OUTPUT);
  pinMode(MA_NMUTE_IO, OUTPUT);

  xTaskCreatePinnedToCore(
      CoreTask,         /* Task function. */
      "CoreTaskHandle", /* name of task. */
      10000,            /* Stack size of task */
      NULL,             /* parameter of the task */
      7,                /* priority of the task */
      &CoreTaskHandle,  /* Task handle to keep track of created task */
      0);               /* pin task to core 0 */


  connectWiFI();

  setUpServer(&httpServer);
  ws.onEvent(onEvent);
  httpServer.addHandler(&ws);
  MDNS.begin("vibinchair");
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
}

void loop()
{
  // yield();
  // copier.copy();
  // put your main code here, to run repeatedly:
}