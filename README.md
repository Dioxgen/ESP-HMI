# ESP-HMI

###### 【Designed by SoTWild】

[TOC]

ESP-HMI 是 [Link 设备链](/blog/sotwild/20211025.html)中的一个，是整个项目最**难**开发的部分，主要提供**远程直接开关设备、数据汇总（设备监控）**的功能。

我还开发了一些针对**不同人群**的功能：**文本阅读、编辑、图片查看、播放MJPEG视频、运行小程序**等。如果你是**开发者**，你也可以使用引出的I/O对它进行**二次开发**。（普通人可以把它看做一个功能**极其简单**的小电脑）

为了高效率运行程序，我移植了 **FreeRTOS** 操作系统，使得主控芯片可以 “同时” 运行多个程序。

------

### 主要硬件组成：

#### · ESP32-WROVER

[ESP32-WROVER](extension://oikmahiipjniocckomdccmplodldodja/pdf-viewer/web/viewer.html?file=http%3A%2F%2Fstatics3.seeedstudio.com%2Fassets%2Ffile%2Fbazaar%2Fproduct%2Fesp32-wrover_datasheet_cn.pdf) 系列模组基于 **ESP32-D0WD** **双核**芯片设计，其强大的双核性能适用于对内存需求大的应用场景，例如多样的 **AIoT** 应用和**网关**应用。

<img src="https://tse2-mm.cn.bing.net/th/id/OIP-C.HAEQhUWocwVcb56vDvSXiwHaFj?pid=ImgDet&rs=1" style="zoom:33%;" />

<center>ESP32-WROVER 模组</center>

#### · 3.5 寸 TFT

> 驱动芯片：ILI9488
>
> 通讯接口：SPI
>
> 触摸芯片：XPT2046

<img src="https://tse1-mm.cn.bing.net/th/id/R-C.c19b0647f33463543697b2392da7909e?rik=XlUPUTWrrgJY4w&riu=http%3a%2f%2fspotpear.cn%2fuploads%2fpicture%2fproduct%2farduino%2farduino-expansion%2f3.5inch-tft-touch-shield%2f3.5inch-tft-touch-shield-02.jpg&ehk=owJ7ZBHyZSnNiZ5OAvgHWOqoyD5GxQSpUkddMqO3YaE%3d&risl=&pid=ImgRaw&r=0" style="zoom:35%;" />

<center>屏幕模块</center>

#### · 存储卡

最大 **32G**，推荐 4 或 8G。

<img src="https://tse1-mm.cn.bing.net/th/id/R-C.d46a9a0f22c0a49e08a9399979d838b5?rik=89LjVtYCdusYQA&riu=http%3a%2f%2fimg.11665.com%2fimg02_p%2fi2%2f10739028636189440%2fT1DGqAFotXXXXXXXXX_!!0-item_pic.jpg&ehk=Y%2fUCSCyDxTxxWsFynax8Yrqs4gzPDIwAu3rvuZC%2fPV8%3d&risl=&pid=ImgRaw&r=0" style="zoom:10%;" />

<center>32G TF卡</center>

#### · 传感器

**SHT30 模拟温湿度传感器**（I2C接口）

SHT30能够提供极高的可靠性和出色的长期稳定性，具有功耗低、反应快、抗干扰能力强等优点。

轻松实现城市环境监控、智能楼宇、工业自动化、智能家居等物联网应用场景的温湿度传感。

> - 主芯片（传感器）：Sensirion SHT30
> - 供电电压(VCC)：3.3V ~ 5.5V
> - 通信接口：Gravity Analog （PH2.0-3P，模拟电压输出0.3-2.7V）
> - 工作电流：< 0.5 mA
> - 产品尺寸：30×22 mm
> - 重量：3 g

**温度测量性能**

> - 量程：-40 ~ 125 ℃
> - 分辨率：0.01 ℃，14bit
> - 精度：±0.2℃@10~55℃（典型值），±1.5℃@-40 ~ 125 ℃（典型值）
> - 响应速度：> 2s

**湿度测量性能**

> - 量程：0~100 %RH
> - 分辨率：0.006 %，14bit
> - 精度：±3 %RH@10~90 %RH（典型值），±8 %RH@0~100 %RH（典型值）
> - 响应速度：> 8s

<img src="https://www.startece.com/startececms/template/website/product/upload/2019/11/04/1324cea8f6a64d76aa382c47696dfd65.jpg" style="zoom:10%;" />

<center>SHT30</center>

#### · PCF8563

PCF8563 是 PHILIPS 公司推出的一款**工业级内含I2C 总线接口功能**的具有**极低功耗**的多功能**时钟/日历芯片**。是一款性价比极高的时钟芯片，它已被广泛用于电表、水表、气表、电话、传真机、便携式仪器以及电池供电的仪器仪表等产品领域。

<img src="https://image3.pushauction.com/2/0/0/f37477f9-6aaa-4217-9548-c36e3e397539/e593b779-4bc5-40a3-a4b6-8498011dd512.jpg" style="zoom:10%;" />

<center>PCF8563 模块</center>

#### PCB:

验证板：只有最基础硬件（包括SHT30），注意 GPIO2 在烧录程序时需断开，如果你要频繁烧录程序，最好 PCB上飞线一个开关，或者拔下开发板（如果你愿意的话）。

[Gerber_PCB_ESP32开发直插板.zip](https://github.com/SoTWild/ESP-HMI/blob/main/PCB/验证板/Gerber_PCB_ESP32开发直插板.zip)

<img src="https://i2.imgu.cc/images/2022/04/27/CK8ux.jpg" style="zoom:10%;" />

<center>验证板</center>

最终板：硬件齐全，自动烧录，配备电池。

[还没设计完……](https://github.com/SoTWild/ESP-HMI)

##### 元件解读：

[还没设计完……](https://github.com/SoTWild/ESP-HMI)

------

### 软件：

本人一切嵌入式皆为自学，~~原谅我写成屎山或执行效率不高~~

[Media_Player.h](https://github.com/SoTWild/ESP-HMI/blob/main/ESP32-HMI/include/Media_Player.h) 这里是**主要**函数：

```c
void Mjpeg_start(const char *MJPEG_FILENAME, const char *AUDIO_FILENAME);
```

这是进行 **Mjpeg** 视频播放，源代码由[Play Video With ESP32](https://www.instructables.com/Play-Video-With-ESP32/)修改而来，详情见[MjpegClass.h](https://github.com/SoTWild/ESP-HMI/blob/main/ESP32-HMI/include/MjpegClass.h)。

```c
void drawSdJpeg(const char *filename, int xpos, int ypos);
```

这是绘制 **.jpg** 格式图片，源代码由[Bodmer/JPEGDecoder](https://github.com/Bodmer/JPEGDecoder)修改而来。

```c
void MP3_start(const char *filename);
```

这是进行 .mp3 格式音频的播放，源代码由[ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)修改而来。

```c
void PCM_start(const char *AUDIOFILENAME);
```

这是进行 .pcm 格式音频的播放，源代码由[Play Video With ESP32](https://www.instructables.com/Play-Video-With-ESP32/)修改而来。

```c
String readFileLine(const char* path, int num);
```

读取 .txt 文本的某一行，源代码来自[peng-zhihui/HoloCubic](https://github.com/peng-zhihui/HoloCubic)。

```c
void BleAudio();
```

蓝牙音频接收，来自[ESP32-A2DP](https://github.com/pschatzmann/ESP32-A2DP)。



[Main.h](https://github.com/SoTWild/ESP-HMI/blob/main/ESP32-HMI/include/Main.h) 这是应用：

> Album 相册
>
> Game 小游戏
>
> Sounder 音乐播放器
>
> Vision 视频播放器
>
> Ebook 电子书阅读器
>
> Settings 设置



[main.cpp](https://github.com/SoTWild/ESP-HMI/blob/main/ESP32-HMI/src/main.cpp) 开机时执行的程序，包括：

> 1）加载串口
>
> 2）屏幕初始化
>
> 3）触摸初始化
>
> 4）挂载SD卡
>
> 5）连接 Wi-Fi
>
> 6）读取设置文件
>
> 7）运行 MainPage(); 主页程序

------

