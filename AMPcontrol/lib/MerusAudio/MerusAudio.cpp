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


#define MA_NENABLE_IO  18//CONFIG_MA120X0_NENABLE_PIN  
#define MA_NMUTE_IO    19//CONFIG_MA120X0_NMUTE_PIN 



static const char* I2C_TAG = "i2c";
#define I2C_CHECK(a, str, ret)  if(!(a)) {                                             \
        ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);      \
        return (ret);                                                                   \
        }


#define I2C_MASTER_SCL_IO CONFIG_MA120X0_SCL_PIN  13//4   /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_MA120X0_SDA_PIN  14//0
    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0
 /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ    400000     /*!< I2C master clock frequency */

#define MA120X0_ADDR  0x20//CONFIG_MA120X0_I2C_ADDR  /*!< slave address for MA120X0 amplifier */

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */


void setup_ma120x0()
{  // Setup control pins nEnable and nMute
//    gpio_config_t io_conf;

//    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
//    io_conf.mode = GPIO_MODE_OUTPUT;
//    io_conf.pin_bit_mask = (1ULL<<MA_NENABLE_IO | 1ULL<<MA_NMUTE_IO);
//    io_conf.pull_down_en = 0;
//    io_conf.pull_up_en = 0;

//    gpio_config(&io_conf);
    Serial.println("setup_ma120x0");

   pinMode(MA_NMUTE_IO, OUTPUT);
   pinMode(MA_NENABLE_IO, OUTPUT); 
   digitalWrite(MA_NMUTE_IO, LOW);
   digitalWrite(MA_NENABLE_IO, HIGH);


//    i2c_master_init();
//Unnessecary due to Wire
    
   digitalWrite(MA_NENABLE_IO, LOW);
   Serial.println("Enabling");
	  
   uint8_t res = ma_read_byte(MA_hw_version__a);
    Serial.print("MA120x0P version: ");
    Serial.println(res, HEX);
    Serial.println("MA120x0P setup done");
  //  //printf("Hardware version: 0x%02x\n",res);
  //01001001

   ma_write_byte(MA_i2s_format__a,0x8);          // Set i2s left justified, set audio_proc_enable
   ma_write_byte(MA_audio_proc_limiterEnable__a,81);
  //  ma_write_byte(MA_audio_proc_limiterEnable__a,0b01010001);

  //  //ma_write
   ma_write_byte(MA_vol_db_master__a,0x5);    // Set vol_db_master 

  //  ma_write_byte(MA_manualPM__a, 0b01111101);

  
  res = get_MA_i2s_format();
    Serial.print("MA120x0P i2s_format: ");
    Serial.println(ma_read_byte(53),BIN);

     Serial.print("MA120x0P limiter and i2s data: ");
    Serial.println(ma_read_byte(54),BIN);

       printf("Clear errors\n");
   ma_write_byte(45,0x34);
   ma_write_byte(45,0x30);
   printf("MA120x0P init done\n");

for (int i = 0; i < 0x7D; i++){
    Serial.print(i, HEX);
    Serial.print(": 0x");
    Serial.print(ma_read_byte(i), HEX);
    Serial.print(" -  0b");
    Serial.println(ma_read_byte(i), BIN);
}

  //  ma_write_byte(0x20, 0b00011111);
  //  ma_write_byte(0x20, 0b10011111);
  //     Serial.println(ma_read_byte(0x7C), BIN);


     


     digitalWrite(MA_NMUTE_IO, HIGH);
  //       ma_write_byte(0x20, 0b00011111);
  //  ma_write_byte(0x20, 0b10011111);
      Serial.println(ma_read_byte(0x7C), BIN);

   //printf("Unmute\n");
}


// void i2c_master_init()
// {  int i2c_master_port = I2C_MASTER_NUM;
//    i2c_config_t conf;
//    conf.mode = I2C_MODE_MASTER;
//    conf.sda_io_num = I2C_MASTER_SDA_IO;
//    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.scl_io_num = I2C_MASTER_SCL_IO;
//    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
//    esp_err_t res = i2c_param_config(i2c_master_port, &conf);
//    //printf("Driver param setup : %d\n",res);
//    res = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
//    //printf("Driver installed   : %d\n",res);
// }

esp_err_t ma_write(uint8_t address, uint8_t *wbuf, uint8_t n)
{
  // bool ack = ACK_VAL;
  // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  // i2c_master_start(cmd);
  // i2c_master_write_byte(cmd, MA120X0_ADDR<<1 | WRITE_BIT, ACK_CHECK_EN);
  // i2c_master_write_byte(cmd, address, ACK_VAL);

  // for (int i=0 ; i<n ; i++)
  // { if (i==n-1) ack = NACK_VAL;
  //   i2c_master_write_byte(cmd, wbuf[i], ack);
  // }
  // i2c_master_stop(cmd);
  // int ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  // i2c_cmd_link_delete(cmd);
  // if (ret == ESP_FAIL) { return ret; }
  // return ESP_OK;


  if (n < 30) {
    Wire.beginTransmission(MA120X0_ADDR);   // Initialize a new I2C transmission at                                               // DSP_I2C_ADDR (adjust in USER_DEFINES.h)
    Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
    Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
    Wire.write(wbuf, n);               //  Add the whole data packet to the I2C buffer
    Wire.endTransmission();  
    }                // Send the entire I2C transmission to the DSP
    // } else {
    //     // Go through the transmission one byte at a time.
    //     int currentByte = 0;

    //     while (currentByte < n) {
    //         // Begin a new I2C transaction.
    //         Wire.beginTransmission(MA120X0_ADDR);
    //         Wire.write(address >> 8);
    //         Wire.write(address & 0xff);

    //         int bytesTransmitted = 0;
    //         // For each byte, find out if it will fit into the current transaction, and
    //         // make sure there is still data left to send.
    //         while ((bytesTransmitted + getMemoryDepth(uint32_t(address)) <= 30) &&
    //                (currentByte < n)) {
    //             // If another register will fit inside the current I2C burst write,
    //             // add each byte to the I2C buffer one at a time.
    //             for (byte i = 0; i < getMemoryDepth(uint32_t(address)); i++) {
    //                 Wire.write(wbuf[currentByte++]);
    //                 bytesTransmitted++;
    //             }
    //             // Increment address once per written register.
    //             address++;
    //         }
    //         Wire.endTransmission();    // Now that the buffer is full, send the I2C burst.
    //         if (currentByte == n) {
    //             break;
    //         }
    //     }
    // }
    return 1;
}

esp_err_t ma_write_byte(uint8_t address, uint8_t value)
{ 
  // esp_err_t ret=0;
  // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  // i2c_master_start(cmd);
  // i2c_master_write_byte(cmd, (MA120X0_ADDR<<1) | WRITE_BIT , ACK_CHECK_EN);
  // i2c_master_write_byte(cmd, address, ACK_VAL);
  // i2c_master_write_byte(cmd, value, ACK_VAL);
  // i2c_master_stop(cmd);
  // ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  // i2c_cmd_link_delete(cmd);
  // if (ret == ESP_FAIL) {
  //    //printf("ESP_I2C_WRITE ERROR : %d\n",ret);
	//  return ret;
  // }
  // return ESP_OK;
    Wire.beginTransmission(MA120X0_ADDR);   // Initialize a new I2C transmission at                                               // DSP_I2C_ADDR (adjust in USER_DEFINES.h)
    // Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
    // Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
    Wire.write(address);              // Add low byte of address to the I2C buffer
    Wire.write(value);               //  Add the whole data packet to the I2C buffer
    Wire.endTransmission();                  // Send the entire I2C transmission to the DSP
    return 1;
}

//prbly doesnt get used?
esp_err_t ma_read(uint8_t address, uint8_t *rbuf, uint8_t n)
{
// { esp_err_t ret;
//   i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//   if (cmd == NULL ) { //printf("ERROR handle null\n"); }
//   i2c_master_start(cmd);
//   i2c_master_write_byte(cmd, (MA120X0_ADDR<<1) | WRITE_BIT, ACK_CHECK_EN);
//   i2c_master_write_byte(cmd, address, ACK_VAL);
//   i2c_master_start(cmd);
//   i2c_master_write_byte(cmd, (MA120X0_ADDR<<1) | READ_BIT, ACK_CHECK_EN);

//   i2c_master_read(cmd, rbuf, n-1 ,ACK_VAL);
//  // for (uint8_t i = 0;i<n;i++)
//  // { i2c_master_read_byte(cmd, rbuf++, ACK_VAL); }
//   i2c_master_read_byte(cmd, rbuf + n-1 , NACK_VAL);
//   i2c_master_stop(cmd);
//   ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100 / portTICK_RATE_MS);
//   i2c_cmd_link_delete(cmd);
//   if (ret == ESP_FAIL) {
//       //printf("i2c Error read - readback\n");
// 	  return ESP_FAIL;
//   }
  return 0;


}


uint8_t ma_read_byte(uint8_t address)
{
  uint8_t value = 0;
  // esp_err_t ret;
  // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  // i2c_master_start(cmd);								// Send i2c start on bus
  // i2c_master_write_byte(cmd, (MA120X0_ADDR<<1) | WRITE_BIT, ACK_CHECK_EN );
  // i2c_master_write_byte(cmd, address, ACK_VAL);         // Send address to start read from
  // i2c_master_start(cmd);							    // Repeated start
  // i2c_master_write_byte(cmd, (MA120X0_ADDR<<1) | READ_BIT, ACK_CHECK_EN);
  // i2c_master_read_byte(cmd, &value, NACK_VAL);
  // i2c_master_stop(cmd);
  // ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  // i2c_cmd_link_delete(cmd);
  // if (ret == ESP_FAIL) {
  //     //printf("i2c Error read - readback\n");
	//   return ESP_FAIL;
  // }
    Wire.beginTransmission(MA120X0_ADDR);   // Initialize a new I2C transmission at                                               // DSP_I2C_ADDR (adjust in USER_DEFINES.h)

  //  Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
    //Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
    Wire.write(address);               //  Add the whole data packet to the I2C buffer
    Wire.endTransmission();                  // Send the entire I2C transmission to the DSP

    Wire.requestFrom(MA120X0_ADDR, 1);               //  Add the whole data packet to the I2C buffer

    if(Wire.available()) {
        value = Wire.read();
    }
    else {
        //printf("Wire.available() returned false\n");
        value = -1;
    }
    
  

  return value;
}