#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ESPmDNS.h>

#include "MerusAudio.h"

void connectWiFI();

void setUpServer(AsyncWebServer *httpServer);

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);