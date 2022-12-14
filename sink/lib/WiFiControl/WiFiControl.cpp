#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ESPmDNS.h>
#include <ArduinoJson.h>
//#include <Arduino.h>

void connectWiFI()
{
  // WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);

  WiFi.hostname("vibinchair");

  // WiFi.begin("WhiteSky-Junction", "h6trew7e");
  WiFi.begin("Vibin", "vibinon1");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("WiFi started");
  // print ip address
  Serial.println(WiFi.localIP());
}

void setUpServer(AsyncWebServer *httpServer)
{
  httpServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
                  request->send_P(200, "text/html", "<h1>Vibin</h1> </br> <h2> Please download the app to get started </h2>"); 
                  Serial.println("Webpage Requested"); });
}

bool test = false;
StaticJsonDocument<64> doc;
DynamicJsonDocument repeat(3072);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    // client connected
    // os_printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("{\"type\":\"update\",\"origin\":\"device\",\"nId\":0,\"data\":\"cl-%u\"}", client->id());
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
    Serial.println("Pong");
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
        Serial.println((char *)data);
        DeserializationError error = deserializeJson(doc, (char *)data);
        if (error)
        {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        const char *type = doc["type"];     // "command"
        const char *origin = doc["origin"]; // "device"
        int nId = doc["nId"];               // 13000
        const char *dataM = doc["data"];
        
        doc["type"] = "update";

        String output;
        serializeJson(doc, output);
        client->text(output);
        //send to all clients
        

       // client->text("I got your text message");
        if(strcmp(type, "command") == 0 ){
        if (strcmp((char *)dataM, "en-on") == 0)
        {
          digitalWrite(18, HIGH);
        }
        else if (strcmp((char *)dataM, "en-off") == 0)
        {
          digitalWrite(18, LOW);
        }
        else if (strcmp((char *)dataM, "m-on") == 0)
        {
          digitalWrite(19, HIGH);
        }
        else if (strcmp((char *)dataM, "m-off") == 0)
        {
          digitalWrite(19, LOW);
        }
                else if (strcmp((char *)dataM, "r-on") == 0)
        {
          digitalWrite(12, HIGH);
        }
        else if (strcmp((char *)dataM, "r-off") == 0)
        {
          digitalWrite(12, LOW);
        }
        }



        // client->ping();
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
