#ifndef _MERUSAUDIO_H_ 
#define _MERUSAUDIO_H_
#include <Arduino.h>
  
class MAAudio
{ 
  public:
    MAAudio(int _i2cAddress);
    void setup_ma120x0(void);
    esp_err_t ma_write_byte(uint8_t address, uint8_t value);
    uint8_t ma_read_byte(uint8_t address);

  private:
    int i2c_address;

};


#endif /* _MERUSAUDIO_H_  */ 

  