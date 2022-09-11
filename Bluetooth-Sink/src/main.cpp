#include <Arduino.h>

#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ESPmDNS.h>

A2DPStream in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
I2SStream out;
uint16_t sample_rate = 44100;
uint8_t channels = 2;
// CsvStream<int16_t> out(Serial, 2); // ASCII stream as csv
StreamCopy copier(out, in); // copy in to out

MAX30105 particleSensor;

TaskHandle_t I2CTaskHandle;

AsyncWebServer httpServer(80);
AsyncWebSocket ws("/ws");

// setup led pin 2
const int ledPin = 2;
int mutePin = 19;
int EnPin = 18;
int resetPin = 12;
int m6Pin = 26;
int m7Pin = 33;

bool ledState = false;

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    // client connected
    // os_printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    // client disconnected
    // os_printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR)
  {
    // error was received from the other end
    // os_printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  }
  else if (type == WS_EVT_PONG)
  {
    // pong message was received (in response to a ping request maybe)
    // os_printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  }
  else if (type == WS_EVT_DATA)
  {
    // data packet
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      // os_printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if (info->opcode == WS_TEXT)
      {
        data[len] = 0;
        // os_printf("%s\n", (char*)data);
      }
      else
      {
        for (size_t i = 0; i < info->len; i++)
        {
          // os_printf("%02x ", data[i]);
        }
        // os_printf("\n");
      }
      if (info->opcode == WS_TEXT)
      {
        client->text("I got your text message");
        ledState = !ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW);
        digitalWrite(EnPin, ledState ? HIGH : LOW);
        digitalWrite(mutePin, ledState ? HIGH : LOW);
        digitalWrite(resetPin, ledState ? HIGH : LOW);
        digitalWrite(m6Pin, ledState ? HIGH : LOW);
        digitalWrite(m7Pin, ledState ? HIGH : LOW);
      }
      else
        client->binary("I got your binary message");
    }
    else
    {
      // message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.println("Pretty big");
        // os_printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        // os_printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      // os_printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if (info->message_opcode == WS_TEXT)
      {
        data[len] = 0;
        // os_printf("%s\n", (char*)data);
      }
      else
      {
        for (size_t i = 0; i < len; i++)
        {
          // os_printf("%02x ", data[i]);
        }
        // os_printf("\n");
      }

      if ((info->index + len) == info->len)
      {
        // os_printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          // os_printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void I2CTask(void *pvParameters)
{
  pinMode(ledPin, OUTPUT);
  pinMode(mutePin, OUTPUT);
  pinMode(EnPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  //pinMode(m6Pin, OUTPUT);
  //pinMode(m7Pin, OUTPUT);
  Serial.println("Setting");
  digitalWrite(ledPin, HIGH);
  digitalWrite(mutePin, HIGH);
  digitalWrite(EnPin, HIGH);
  digitalWrite(resetPin, HIGH);

  WiFi.mode(WIFI_STA);
  String hostname = "vibinchair";
WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
WiFi.setHostname(hostname.c_str()); //define hostname
  delay(50);
  WiFi.begin("Vibin", "vibinon1");
  //attach callback on _arduino_event_cb STA got new IP
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
      MDNS.begin("vibinchair");

  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
 while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
    
  
  Serial.println("WiFi started");
  // print ip address
  Serial.println(WiFi.localIP());
  httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  request->send_P(200, "text/html", "<h1>Vibin</h1>"); 
                  Serial.println("Hello");
                  ledState = !ledState;
                  digitalWrite(ledPin, ledState ? HIGH : LOW); });
  ws.onEvent(onEvent);
  httpServer.addHandler(&ws);
          MDNS.addService("_http", "tcp", 80);
  MDNS.addService("_ws", "tcp", 80);
      MDNS.begin("vibinchair");


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
