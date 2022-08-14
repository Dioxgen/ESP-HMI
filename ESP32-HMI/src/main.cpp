/*
  2022/01/02 开始编辑源代码
  ========================
  2022/01/02 添加 Media Player 中 Mjpeg 播放功能
  2022/01/03 添加 Media Player 中 Jpeg  播放功能
  2022/01/03 添加 Media Player 中 MP3   播放功能
  2022/01/09 添加 Media Player 中 PCM   播放功能
  2022/01/09 添加 Media Player 中 TXT   播放功能
  2022/01/12 添加 Draw 功能
  2022/01/12 添加 THMini 功能
  2022/03/28 添加 Album 功能
  2022/03/28 添加 Ebook 功能
  2022/03/29 代码大改
  2022/04/01 蓝牙音频传输（接收）
  2022/04/05 开始编写 Sounder
  2022/04/10 完成编写 Sounder
  2022/04/13 开始编写 Vision
  2022/04/14 完成编写 Vision
  2022/04/17 完善 Ebook
  2022/04/18 开始并完成编写 Settings 
  2022/05/02 用户登陆系统
  2022/07/05 计算器（普通）
  2022/07/08 温度计
  2022/07/12 文件上传
  2022/07/14 WiFi
  2022/08/13 添加Task Manager
  2022/08/14 完成Album功能
*/

//无线AP名称Teacher WiFi
//密码cn@syzx@www

#include <Arduino.h>
#include <WiFi.h>
#include "Main.h"

#define TFT_BL 19
#define TFT_BRIGHTNESS 255
#define StartPrint 0

void setup() {

  Serial.begin(115200);
  //WiFi.mode(WIFI_OFF);

  gfx->begin(60000000);

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0, 4);
  tft.setTextDatum(MC_DATUM);
  tft.setSwapBytes(true);

  touch.setCal(481, 3395, 755, 3487, 480, 320, 0);
  touch.setRotation(1);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  pinMode(35,INPUT_PULLUP);//interrupt
  pinMode(34,INPUT);//Battery
  //attachInterrupt(35, tft_BL, RISING);
  
  /*
    ledcAttachPin(TFT_BL, 1);
    ledcSetup(1, 12000, 8);
    ledcWrite(1, TFT_BRIGHTNESS);
  */
  if ((!SD_MMC.begin("/sdcard", true)) && (!SD_MMC.begin("/sdcard", true)))
  {
    Serial.println(F("ERROR: SD card mount failed!"));
    gfx->println(F("ERROR: SD card mount failed!"));
    delay(1000);
    //    ESP.restart();
  }
  else {
    Serial.println("SD card Ready!");
    Serial.printf("SD.cardSize = %lluGB \n", SD_MMC.cardSize() / (1024 * 1024 * 1024));
    Serial.printf("SD.totalBytes = %lluGB \n", SD_MMC.totalBytes() / (1024 * 1024 * 1024));
    Serial.printf("SD.usedBytes = %lluGB \n", SD_MMC.usedBytes() / (1024 * 1024 * 1024));
    Serial.printf("SD.cardType = %d \r\n", SD_MMC.cardType());
    if (StartPrint){
      tft.println("SD card Ready!");
      tft.printf("SD.cardSize = %lluGB \n", SD_MMC.cardSize() / (1024 * 1024 * 1024));
      tft.printf("SD.totalSize = %lluGB \n", SD_MMC.totalBytes() / (1024 * 1024 * 1024));
      tft.printf("SD.usedSize = %lluGB \n", SD_MMC.usedBytes() / (1024 * 1024 * 1024));
      tft.printf("SD.cardType = %d \r\n", SD_MMC.cardType());
      delay(2000);
    }
  }
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  /*
  IPAddress staticIP(192, 168, IPAD1, IPAD2);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(8, 8, 8, 8);
  WiFi.disconnect();
  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("TP-LINK_6A56", "bobo512w999580");
  wifiMulti.addAP("TP-LINK_6A56", "bobo512w999580");
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  tft.print("Connected to ");
  tft.println(WiFi.SSID());
  tft.print("IP address:\t");
  tft.println(WiFi.localIP());
  */
  if (StartPrint){
    tft.println(readFileLine("/System/Users/Users.txt", 1));
    tft.println(readFileLine("/System/Users/Users.txt", 2));
    tft.println(readFileLine("/System/Users/Users.txt", 3));
    tft.println(readFileLine("/System/Settings/Initial interface.txt", 1));
//  tft.println(readFileLine("/System/Settings/LastUser.txt", 1));
//  tft.println(readFileLine("/User/SoTWild/Data/Others/password.txt", 1));
  }

  //Demos:
  //draw();
  //WiFiInit("AP");
  //File_transfer();
  //DS1307();
  //MPU_Setup();

  Userlogin();
  line = 1;
  tft.fillScreen(TFT_BLACK);
  Userdir = String("/User/") + User + String("/Config");
  //  listDir(SD_MMC, Userdir.c_str() , 3);
  //  delay(5000);
  Refresh();
  tft.setTextColor(TFT_WHITE);
  Serial.println(User);
}

void loop() {
  /*if(Serial.available()){
    char c = Serial.read();
    Serial.println(c);
    switch(c){
      case 'a': drawSdJpeg("/Data/3.jpg", 0, 0); break;
      case 'b': Mjpeg_start("/Video/BadApple/BadApple.pcm","/Video/BadApple/BadApple.mjpeg"); break;
      case 'c': MP3_start("/Data/Remilia.mp3"); break;
      case 'd': MP3_start("/Video/BadApple/BadApple.mp3"); break;
      case 'e': PCM_start("/Video/TOUHOU/TOUHOU.pcm"); break;
      case 'f': draw(); break;
      case 'g': THMini(); break;
      default:Serial.println("error"); break;
    }
    }*/
  MainPage();
}