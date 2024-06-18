/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：ENVIII_SHT30_QMP6988.  环境传感器
* date：2021/8/17
*******************************************************************************
  Please connect to Port,Read temperature, humidity and atmospheric pressure and display them on the display screen
  请连接端口,读取温度、湿度和大气压强并在显示屏上显示
*/

//*******************************************************
//
// M5Unit-ENV ライブラリは 0.0.7 を使用してください
// 0.0.8 や 0.0.9 では動作しません
//
//*******************************************************

//  対応機種　Applicable model
//  * M5Stack Core     with LoRa Module and ENVIII UNIT
//  * M5Stack Core2    with LoRa Module and ENVIII UNIT
//  * M5Stack ATOM S3  with LoRa KIT and ENVIII UNIT
//  * M5Stack ATOM     with LoRa KIT and ENVIII UNIT
//  * M5StickC Plus2   with LoRa HAT and ENVIII UNIT / LoRa UNIT and ENVIII HAT
//  * M5StickC Plus    with LoRa HAT and ENVIII UNIT / LoRa UNIT and ENVIII HAT
//  * M5StickC         with LoRa HAT and ENVIII UNIT / LoRa UNIT and ENVIII HAT
//


#include <M5Unified.h>

//#define VIA_GROVE // LoRa UNITを使用する場合は先頭の//を削除

// 以下は機種に合わせてコメントアウトを解除してください

//// * M5StickC Plus / Plus2   with LoRa HAT and ENVIII UNIT / LoRa UNIT and ENVIII HAT
 #define ROT 3
 #define TSIZE 2
 #define RPOS 62
 #define SX 135
 #define SY 35
 #define SR 8
 #ifdef VIA_GROVE
 #define SCL_pin 26 // ENV III HAT
 #define SDA_pin 0  // ENV III HAT
 #define RX_pin 33 // LoRa UNIT via Grove
 #define TX_pin 32 // LoRa UNIT via Grove
 #define RESET_pin -1 // No RESET Pin
 #define BOOT_pin -1  // No Boot Pin
 #else
 #define SCL_pin 33 // ENV III via Grove
 #define SDA_pin 32 // ENV III via Grove
 #define RX_pin 26 // LoRa HAT
 #define TX_pin 0  // LoRa HAT
 #define RESET_pin -1 // No RESET Pin
 #define BOOT_pin -1  // No Boot Pin
 #endif

// * M5StickC  with LoRa HAT and ENVIII UNIT / LoRa UNIT and ENVIII HAT
//#define ROT 3
//#define TSIZE 2
//#define RPOS 62
//#define SX 135
//#define SY 35
//#define SR 8
//#ifdef VIA_GROVE
//#define SCL_pin 26 // ENV III HAT
//#define SDA_pin 0  // ENV III HAT
//#define RX_pin 33 // LoRa UNIT via Grove
//#define TX_pin 32 // LoRa UNIT via Grove
//#define RESET_pin -1 // No RESET Pin
//#define BOOT_pin -1  // No Boot Pin
//#else
//#define SCL_pin 33 // ENV III via Grove
//#define SDA_pin 32 // ENV III via Grove
//#define RX_pin 26 // LoRa HAT
//#define TX_pin 0  // LoRa HAT
//#define RESET_pin -1 // No RESET Pin
//#define BOOT_pin -1  // No Boot Pin
//#endif



//// * M5Stack ATOM S3  with LoRa KIT and ENVIII UNIT
//#define ROT 1
//#define TSIZE 2
//#define RPOS 100
//#define SX 100
//#define SY 80
//#define SR 8
//#define SCL_pin 1
//#define SDA_pin 2
//#define RX_pin 5 // LoRa KIT
//#define TX_pin 6 // LoRa KIT
//#define RESET_pin 7 // LoRa KIT
//#define BOOT_pin 39 // LoRa KIT

//// * M5Stack ATOM  with LoRa KIT and ENVIII UNIT
//#define ROT 1
//#define TSIZE 2
//#define RPOS 100
//#define SX 100
//#define SY 80
//#define SR 8
//#define SCL_pin 32
//#define SDA_pin 26
//#define RX_pin 22 // LoRa KIT
//#define TX_pin 19 // LoRa KIT
//#define RESET_pin 23 // LoRa KIT
//#define BOOT_pin 21 // LoRa KIT

//// * M5Stack Core2    with LoRa Module and ENVIII UNIT
//#define ROT 1
//#define TSIZE 4
//#define RPOS 180
//#define SX 270
//#define SY 35
//#define SR 15
//#define SCL_pin 33
//#define SDA_pin 32
//#define RX_pin 13 // stack LoRa MODULE
//#define TX_pin 14 // stack LoRa MODULE
//#define RESET_pin 19 // stack LoRa MODULE
//#define BOOT_pin 22  // stack LoRa MODULE


//// * M5Stack Core    with LoRa Module/LoRa UNIT
//#define ROT 1
//#define TSIZE 4
//#define RPOS 180
//#define SX 270
//#define SY 35
//#define SR 15
//#define SCL_pin 22
//#define SDA_pin 21
//#define RX_pin 16 // stack LoRa MODULE
//#define TX_pin 17 // stack LoRa MODULE
//#define RESET_pin 13 // stack LoRa MODULE
//#define BOOT_pin 22  // stack LoRa MODULE



#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "M5_ENV.h"
#include "LoRa.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;


void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Wire.end(); // For overcome bug of Wire and UNIT_ENV
  Wire.begin(SDA_pin, SCL_pin); //Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
  qmp6988.init();
  delay(10);
  LoRaInit(RX_pin, TX_pin, RESET_pin, BOOT_pin);
  M5.Lcd.setRotation(ROT);
  M5.Lcd.setTextSize(TSIZE);
}

void loop() {
  pressure = qmp6988.calcPressure() / 100;
  if(sht30.get()==0){ //Obtain the data of shT30.  获取sht30的数据
    tmp = sht30.cTemp;  //Store the temperature obtained from shT30.  将sht30获取到的温度存储
    hum = sht30.humidity; //Store the humidity obtained from the SHT30.  将sht30获取到的湿度存储
  }else{
    tmp=0,hum=0;
  }
  
  M5.Lcd.fillRect(0,20,100,60,BLACK); //Fill the screen with black (to clear the screen).  将屏幕填充黑色(用来清屏)
  M5.Lcd.setCursor(0,18);
  M5.Lcd.printf("T:%2.1f 'C\r\nH:%2.1f %%  \r\nP:%4.0f hPa\r\n", tmp, hum, pressure);
  M5.Lcd.fillCircle(SX, SY, SR, RED);
  Serial.printf("T: %2.1f ℃  \r\nH:   %2.1f ％  \r\nP: %4.0f hPa\r\n---\n", tmp, hum, pressure);
  LoRaCommand("MC"+String(tmp)+" "+String(hum)+((pressure < 1000)? "  ":" ")+String(pressure));
  delay(400);
  M5.Lcd.fillCircle(SX, SY, SR, BLACK);
  delay(5000);
}
