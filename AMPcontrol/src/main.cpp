#include <Arduino.h>
// #include "WiFiControl.h"
// #include "AudioTools.h"
// #include "AudioLibs/AudioA2DP.h"
#include <Wire.h>
#include "SigmaStudioFW.h"
#include "USER_SETTINGS.h"
#include "TESTDSP_IC_1.h"
#include "TESTDSP_IC_1_PARAM.h"
#include "MerusAudio.h"


#define DSP_DVDD 23
#define DSP_RESET 21


void setup(void)
{
  Serial.begin(115200);
 pinMode(DSP_DVDD, OUTPUT);
 digitalWrite(DSP_DVDD, HIGH);
 Serial.println("on");
 pinMode(DSP_RESET, OUTPUT);

 Wire.begin(14, 13, (uint32_t)400000);


delay(100);

digitalWrite(DSP_RESET, LOW);
delay(10);
digitalWrite(DSP_RESET, HIGH);

Serial.println("resert");
delay(560);

default_download_IC_1();

Serial.println("download");

delay(100);



Serial.print("setting up Amp");
setup_ma120x0();



}

void loop()
{
  Serial.println("Flashed?");
  delay(1000);
}