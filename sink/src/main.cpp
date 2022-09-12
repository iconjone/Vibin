#include <Arduino.h>
#include "WiFiControl.h"

TaskHandle_t CoreTaskHandle;

//pin 18 

void CoreTask(void *pvParameters)
{
  connectWiFI();
  AsyncWebServer httpServer(80);
  AsyncWebSocket ws("/ws");
  setUpServer(&httpServer);
  ws.onEvent(onEvent);
  httpServer.addHandler(&ws);
  MDNS.begin("vibinchair");
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  pinMode(18, OUTPUT);

  for (;;)
  {
    // do something
    delay(1000);
  }
}

void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
  xTaskCreatePinnedToCore(
      CoreTask,         /* Task function. */
      "CoreTaskHandle", /* name of task. */
      10000,            /* Stack size of task */
      NULL,             /* parameter of the task */
      2,                /* priority of the task */
      &CoreTaskHandle,  /* Task handle to keep track of created task */
      0);               /* pin task to core 0 */

}

void loop()
{
  // put your main code here, to run repeatedly:
}