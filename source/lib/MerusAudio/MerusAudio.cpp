//
// MA120x0P ESP32 Driver
//
// Merus Audio - September 2018
// Written by Joergen Kragh Jakobsen, jkj@myrun.dk
//
// Register interface thrugh I2C for MA12070P and MA12040P
//   Support a single amplifier/i2c address
//
//
#include <Arduino.h>
#include <Wire.h>

#include "MerusAudio.h"

#include "ma120x0.h"

#define MA_NENABLE_IO 18 // CONFIG_MA120X0_NENABLE_PIN
#define MA_NMUTE_IO 19   // CONFIG_MA120X0_NMUTE_PIN

MAAudio amp1(0x20);
MAAudio amp2(0x21);
MAAudio amp3(0x22);

// put 3 amps in an array
MAAudio amps[3] = {amp1, amp2, amp3};

MAAudio::MAAudio(int _i2c_address)
{
  i2c_address = _i2c_address;
}

void MAAudio::set_i2c_address(int _i2c_address)
{
  i2c_address = _i2c_address;
}

#define MA120X0_ADDR 0x20 // CONFIG_MA120X0_I2C_ADDR  /*!< slave address for MA120X0 amplifier */

void MAAudio::setup_ma120x0()
{
  Serial.print("i2c address: ");
  Serial.println(i2c_address, HEX);
  uint8_t res = ma_read_byte(MA_hw_version__a);
  Serial.print("MA120x0P version: ");
  Serial.println(res, HEX);

  ma_write_byte(MA_i2s_format__a, 0x8);              // Set i2s left justified, set audio_proc_enable
  ma_write_byte(MA_audio_proc_limiterEnable__a, 81); // 81

  ma_write_byte(MA_vol_db_master__a, 0x18); // E // Set vol_db_master

  printf("Clear errors\n");
  ma_write_byte(45, 0x34);
  ma_write_byte(45, 0x30);
  printf("MA120x0P init done\n");
}

esp_err_t MAAudio::ma_write_byte(uint8_t address, uint8_t value)
{
  Wire.beginTransmission(i2c_address); // Initialize a new I2C transmission at                                               // DSP_I2C_ADDR (adjust in USER_DEFINES.h)
  // Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
  // Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
  Wire.write(address);    // Add low byte of address to the I2C buffer
  Wire.write(value);      //  Add the whole data packet to the I2C buffer
  Wire.endTransmission(); // Send the entire I2C transmission to the DSP
  return 1;
}

uint8_t MAAudio::ma_read_byte(uint8_t address)
{
  uint8_t value = 0;

  Wire.beginTransmission(i2c_address); // Initialize a new I2C transmission at
  Wire.write(address);                 //  Add the whole data packet to the I2C buffer
  Wire.endTransmission();              // Send the entire I2C transmission to the DSP
  Wire.requestFrom(i2c_address, 1);    //  Add the whole data packet to the I2C buffer

  if (Wire.available())
  {
    value = Wire.read();
  }
  else
  {
    // printf("Wire.available() returned false\n");
    value = -1;
  }
  return value;
}

void MAAudio::register_dump()
{
  for (int i = 0; i < 0x7D; i++)
  {
    Serial.print(i, HEX);
    Serial.print(": 0x");
    Serial.print(ma_read_byte(i), HEX);
    Serial.print(" -  0b");
    Serial.println(ma_read_byte(i), BIN);
  }
}

void MAAudio::set_volume_master(int volume)
{
  volume = 1023 - volume;

  int integer = volume / 4;
  int fractional = volume % 4;

  ma_write_byte(MA_vol_db_master__a, integer);
  ma_write_byte(MA_vol_lsb_master__a, fractional);
}

void MAAudio::set_volume_channel(char *channel, int volume)
{
  volume = 1023 - volume;

  int integer = volume / 4;
  int fractional = volume % 4;

  if (strcmp(channel, "0L") == 0)
  {
    set_MA_vol_db_ch0(integer);
    set_MA_vol_lsb_ch0(fractional);
  }
  else if (strcmp(channel, "0R") == 0)
  {
    set_MA_vol_db_ch1(integer);
    set_MA_vol_lsb_ch1(fractional);
  }
  else if (strcmp(channel, "1L") == 0)
  {
    set_MA_vol_db_ch2(integer);
    set_MA_vol_lsb_ch2(fractional);
  }
  else if (strcmp(channel, "1R") == 0)
  {
    set_MA_vol_db_ch3(integer);
    set_MA_vol_lsb_ch3(fractional);
  }
  else if (strcmp(channel, "LM") == 0)
  {
    set_MA_vol_db_ch0(integer);
    set_MA_vol_lsb_ch0(fractional);
    set_MA_vol_db_ch2(integer);
    set_MA_vol_lsb_ch2(fractional);
    Serial.println("LM");
  }
  else if (strcmp(channel, "RM") == 0)
  {
    set_MA_vol_db_ch1(integer);
    set_MA_vol_lsb_ch1(fractional);
    set_MA_vol_db_ch3(integer);
    set_MA_vol_lsb_ch3(fractional);
  }
}