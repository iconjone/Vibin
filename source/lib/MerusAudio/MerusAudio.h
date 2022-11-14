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
    void register_dump();
    void set_i2c_address(int _i2cAddress);  
    void set_volume_master(int volume);
    void set_volume_channel(char* channel, int volume);

    int i2c_address;

  private:
    // int i2c_address;

};

  //global MAAudio objects
extern MAAudio amp1;
extern MAAudio amp2;
extern MAAudio amp3;
//gloabl array of MAAudio objects
extern MAAudio amps[3];



#endif /* _MERUSAUDIO_H_  */ 

  