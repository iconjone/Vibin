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

// digitalWrite(19, HIGH);

}

void loop()
{
  Serial.println("Flashed?");
      Serial.print(0x7C, HEX);
    Serial.print(": 0x");
    Serial.print(ma_read_byte(0x7C), HEX);
    Serial.print(" -  0b");
    Serial.println(ma_read_byte(0x7C), BIN);
  delay(10000);
        //  printf("Clear errors\n");
   ma_write_byte(45,0x34);
   ma_write_byte(45,0x30);

  //  ma_write_byte(0x20, 0b00011111);
  //  ma_write_byte(0x20, 0b10011111);
  //     Serial.println(ma_read_byte(0x20), BIN);
  //  ma_write_byte(0x20, 0b00011111);
  //  Serial.println(ma_read_byte(0x20), BIN);

   Serial.println(ma_read_byte(0x7C), BIN);
}