#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ESPmDNS.h>

void connectWiFI(){
     WiFi.mode(WIFI_STA);
WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);

WiFi.hostname("vibinchair");

  //WiFi.begin("WhiteSky-Junction", "h6trew7e");
  WiFi.begin("Vibin", "vibinon1");

 while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  
  Serial.println("WiFi started");
  // print ip address
  Serial.println(WiFi.localIP());
}