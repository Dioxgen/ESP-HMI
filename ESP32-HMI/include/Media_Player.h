//这是一个媒体播放应用
//支持的格式有：.mjpeg / .jpg / .mp3 / .pcm / .txt

String User = "Null";
String Userdir;
String filedir;
String filedir2;
String bookdir;
String password = "9961";
String MusicName;
String MusicState = "List Loop";
String VideoName;
String BookName;
String str_CalNum1;
String str_CalNum2;

int Mainpage = 1;
int MS_num;
int Musicnum = 1;
int Music_num = 1;
int MusicPage = 1;
int MusicBline;
int MusicEline;
int EnableMC = 0; //MusicCheck
int pos;
int a = 1;
int VideoPage = 1;
int VideoBline;
int VideoEline;
int Videonum = 1;
int EnableVC = 0;
int IfVision = 1;
int BookPage = 1;
int EnableBC = 0;
int BookBline;
int BookEline;
int UserPage = 1;
int EnableUC = 0;
int UserBline;
int UserEline;
int b;
int decimal_point = 0;
int inputNum = 1;
int Operation_mode = 1;//1-add 2-subtract 3-multiply 4-divide
int Cal_negative = 0;//判断是否已经输入负数
int Operation_mode_change = 0;

float CalNum1 = 0;
float CalNum2 = 0;
float CalResult = 0;

bool notice;
String n_otice;

//Mjpeg player:
#include "MjpegClass.h"
#include <Arduino_GFX_Library.h>
#include <SD_MMC.h>
#include <driver/i2s.h>

Arduino_DataBus *bus = new Arduino_ESP32SPI(27, 5, 18, 23, -1);
Arduino_ILI9488_18bit *gfx = new Arduino_ILI9488_18bit(bus, 33, 3);

#define TFT_BRIGHTNESS 255
#define MJPEG_BUFFER_SIZE (480 * 320 * 2 / 14)
#define FPS 30
#define READ_BUFFER_SIZE 2048
#define TFT_BL 19

static MjpegClass mjpeg;
int next_frame = 0;
int skipped_frames = 0;
unsigned long total_read_audio = 0;
unsigned long total_play_audio = 0;
unsigned long total_read_video = 0;
unsigned long total_play_video = 0;
unsigned long total_remain = 0;
unsigned long start_ms, curr_ms, next_frame_ms;
static unsigned long total_show_video = 0;
bool Media_play = true;

static int drawMCU(JPEGDRAW *pDraw) {
  unsigned long s = millis();
  gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  total_show_video += millis() - s;
  return 1;
}
void Mjpeg_start(const char *MJPEG_FILENAME, const char *AUDIO_FILENAME) {
  //WiFi.mode(WIFI_OFF);
  gfx->fillScreen(TFT_BLACK);
  
  // Init Audio
  i2s_config_t i2s_config_dac = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
    // .dma_buf_count = 5,
    .dma_buf_count = 8,
    .dma_buf_len = 490,
    .use_apll = false,
  };
  Serial.printf("%p\n", &i2s_config_dac);
  if (i2s_driver_install(I2S_NUM_0, &i2s_config_dac, 0, NULL) != ESP_OK)
  {
    Serial.println(F("ERROR: Unable to install I2S drives!"));
    gfx->println(F("ERROR: Unable to install I2S drives!"));
  }
  else {
    i2s_set_pin((i2s_port_t)0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_zero_dma_buffer((i2s_port_t)0);

    File aFile = SD_MMC.open(AUDIO_FILENAME);
    if (!aFile || aFile.isDirectory()) {
      Serial.println(F("ERROR: Failed to open file for reading!"));
      gfx->println(F("ERROR: Failed to open file for reading!"));
    }
    else {
      File vFile = SD_MMC.open(MJPEG_FILENAME);
      if (!vFile || vFile.isDirectory()) {
        Serial.println(F("ERROR: Failed to open file for reading"));
        Serial.println(AUDIO_FILENAME);
        Serial.println(MJPEG_FILENAME);
        gfx->println(F("ERROR: Failed to open file for reading"));
      }
      else {
        uint8_t *aBuf = (uint8_t *)malloc(2940);
        // uint8_t *aBuf = (uint8_t *)malloc(5880);
        if (!aBuf) {
          Serial.println(F("aBuf malloc failed!"));
        }
        else {
          uint8_t *mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
          if (!mjpeg_buf) {
            Serial.println(F("mjpeg_buf malloc failed!"));
          }
          else {
            Serial.println(F("PCM audio MJPEG video start"));
            start_ms = millis();
            curr_ms = millis();
            mjpeg.setup(&vFile, mjpeg_buf, drawMCU, true, true);
            next_frame_ms = start_ms + (++next_frame * 1000 / FPS);

            // prefetch first audio buffer
            aFile.read(aBuf, 980);
            i2s_write_bytes((i2s_port_t)0, (char *)aBuf, 980, 0);

            while (vFile.available() && aFile.available()) {
              // Read audio
              aFile.read(aBuf, 2940);
              // aFile.read(aBuf, 5880);
              total_read_audio += millis() - curr_ms;
              curr_ms = millis();

              // Play audio
              i2s_write_bytes((i2s_port_t)0, (char *)aBuf, 980, 0);
              i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 980), 980, 0);
              i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 1960), 980, 0);

              total_play_audio += millis() - curr_ms;
              curr_ms = millis();

              // Read video
              mjpeg.readMjpegBuf();
              total_read_video += millis() - curr_ms;
              curr_ms = millis();
              // check show frame or skip frame
              if (millis() < next_frame_ms) {
                // Play video
                mjpeg.drawJpg();
                total_play_video += millis() - curr_ms;

                int remain_ms = next_frame_ms - millis();
                total_remain += remain_ms;
                if (remain_ms > 0) {
                  //delay(remain_ms);
                }
              }
              else {
                ++skipped_frames;
                Serial.println(F("Skip frame"));
              }

              curr_ms = millis();
              next_frame_ms = start_ms + (++next_frame * 1000 / FPS);
            }
            int time_used = millis() - start_ms;
            Serial.println(F("PCM audio MJPEG video end"));
            vFile.close();
            aFile.close();
            // avoid unexpected output at audio pins
            i2s_driver_uninstall((i2s_port_t)0); //stop & destroy i2s driver
            gfx->fillScreen(TFT_BLACK);
            int played_frames = next_frame - 1 - skipped_frames;
            float fps = 1000.0 * played_frames / time_used;
#define CHART_MARGIN 24
#define LEGEND_A_COLOR 0xE0C3
#define LEGEND_B_COLOR 0x33F7
#define LEGEND_C_COLOR 0x4D69
#define LEGEND_D_COLOR 0x9A74
#define LEGEND_E_COLOR 0xFBE0
#define LEGEND_F_COLOR 0xFFE6
#define LEGEND_G_COLOR 0xA2A5
            gfx->setCursor(0, 0);
            gfx->setTextColor(WHITE);
            gfx->printf("Played frames: %d\n", played_frames);
            gfx->printf("Skipped frames: %d (%0.1f %%)\n", skipped_frames, 100.0 * skipped_frames / played_frames);
            gfx->printf("Actual FPS: %0.1f\n\n", fps);
            int16_t r1 = ((gfx->height() - CHART_MARGIN - CHART_MARGIN) / 2);
            int16_t r2 = r1 / 2;
            int16_t cx = gfx->width() - gfx->height() + CHART_MARGIN + CHART_MARGIN - 1 + r1;
            int16_t cy = r1 + CHART_MARGIN;
            float arc_start = 0;
            float arc_end = max(2.0, 360.0 * total_read_audio / time_used);
            for (int i = arc_start + 1; i < arc_end; i += 2) {
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_D_COLOR);
            }
            gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_D_COLOR);
            gfx->setTextColor(LEGEND_D_COLOR);
            gfx->printf("Read PCM:\n%0.1f %%\n", 100.0 * total_read_audio / time_used);

            arc_start = arc_end;
            arc_end += max(2.0, 360.0 * total_play_audio / time_used);
            for (int i = arc_start + 1; i < arc_end; i += 2) {
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_C_COLOR);
            }
            gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_C_COLOR);
            gfx->setTextColor(LEGEND_C_COLOR);
            gfx->printf("Play audio:\n%0.1f %%\n", 100.0 * total_play_audio / time_used);

            arc_start = arc_end;
            arc_end += max(2.0, 360.0 * total_read_video / time_used);
            for (int i = arc_start + 1; i < arc_end; i += 2) {
              gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, i - 90.0, LEGEND_B_COLOR);
            }
            gfx->fillArc(cx, cy, r1, r2, arc_start - 90.0, arc_end - 90.0, LEGEND_B_COLOR);
            gfx->setTextColor(LEGEND_B_COLOR);
            gfx->printf("Read MJPEG:\n%0.1f %%\n", 100.0 * total_read_video / time_used);
            arc_start = arc_end;
            arc_end += max(2.0, 360.0 * total_play_video / time_used);
            for (int i = arc_start + 1; i < arc_end; i += 2) {
              gfx->fillArc(cx, cy, r1, 0, arc_start - 90.0, i - 90.0, LEGEND_A_COLOR);
            }
            gfx->fillArc(cx, cy, r1, 0, arc_start - 90.0, arc_end - 90.0, LEGEND_A_COLOR);
            gfx->setTextColor(LEGEND_A_COLOR);
            gfx->printf("Play video:\n%0.1f %%\n", 100.0 * total_play_video / time_used);
            next_frame = 0;
            skipped_frames = 0;
            total_read_audio = 0;
            total_play_audio = 0;
            total_read_video = 0;
            total_play_video = 0;
            total_remain = 0;
            total_show_video = 0;
            EnableVC = 1;
            delay(1500);
            gfx->fillScreen(TFT_BLACK);
          }
        }
      }
    }
  }
}

//jpeg Player:
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

TFT_eSPI tft = TFT_eSPI();

int jpgnum = 0;
String jpgdir;

void drawSdJpeg(const char *filename, int xpos, int ypos);
void jpegRender(int xpos, int ypos);
void jpegInfo();
void showTime(uint32_t msTime);
void SD_read_Time(uint32_t msTime);

void drawSdJpeg(const char *filename, int xpos, int ypos) {
  uint32_t readTime = millis();
  File jpegFile = SD_MMC.open(filename, FILE_READ);

  if ( !jpegFile ) {
    Serial.print("ERROR: File \"");
    Serial.print(filename);
    Serial.println("\" not found!");
    return;
  }
  boolean decoded = JpegDec.decodeSdFile(jpegFile);
  SD_read_Time(millis() - readTime);

  if (decoded) {
    jpegInfo();
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}
void jpegRender(int xpos, int ypos) {
  uint32_t drawTime = millis();
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);
  uint32_t min_w = (mcu_w < (max_x % mcu_w) ? mcu_w : (max_x % mcu_w));
  uint32_t min_h = (mcu_h < (max_y % mcu_h) ? mcu_h : (max_y % mcu_h));
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;
  max_x += xpos;
  max_y += ypos;
  while (JpegDec.read()) {
    pImg = JpegDec.pImage;
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;
    if (win_w != mcu_w) {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++) {
        p += mcu_w;
        for (int w = 0; w < win_w; w++) {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }
    //    uint32_t mcu_pixels = win_w * win_h;
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort();
  }
  tft.setSwapBytes(swapBytes);
  showTime(millis() - drawTime);
}
void jpegInfo() {
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print("Width      :");
  Serial.println(JpegDec.width);
  Serial.print("Height     :");
  Serial.println(JpegDec.height);
  Serial.print("Components :");
  Serial.println(JpegDec.comps);
  Serial.print("MCU / row  :");
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print("MCU / col  :");
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print("Scan type  :");
  Serial.println(JpegDec.scanType);
  Serial.print("MCU width  :");
  Serial.println(JpegDec.MCUWidth);
  Serial.print("MCU height :");
  Serial.println(JpegDec.MCUHeight);
  Serial.print("===============");
  Serial.println("");
}
void showTime(uint32_t msTime) {
  Serial.print(F(" JPEG drawn in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}
void SD_read_Time(uint32_t msTime) {
  Serial.print(F(" SD JPEG read in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}

//Game-TH-Mini
#include <TFT_Touch.h>

#define DOUT 32  // Data out pin    (T_DO)  of touch screen
#define DIN  27  // Data in pin     (T_DIN) of touch screen
#define DCS  16  // Chip select pin (T_CS)  of touch screen
#define DCLK 0   // Clock pin       (T_CLK) of touch screen
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

int y = 0;
int spd = 25;
int obj = 1;
int score = 0;
int state = 0;

void THMini() {
  tft.setRotation(0);
  touch.setRotation(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  while (1) {
    if (state == 0) {
      tft.setCursor(5, 5);
      tft.setTextColor(TFT_BLUE);
      Userdir = String ("/User/") + User + String("/Data/Game/TouHou/Mini/image/start.jpg");
      drawSdJpeg(Userdir.c_str(), 0, 0);
      state = 1;
    }
    if (touch.Pressed()) {
      Userdir = String ("/User/") + User + String("/Data/Game/TouHou/Mini/image/background.jpg");
      drawSdJpeg(Userdir.c_str(), 0, 0);
      tft.print("score:");
      tft.setCursor(5, 25);
      tft.print(score);
      while (1) {
        if (touch.Pressed()) {
          //tft.fillRect(127,y - spd,64,64,TFT_WHITE);
          Userdir = String ("/User/") + User + String("/Data/Game/TouHou/Mini/image/background.jpg");
          drawSdJpeg(Userdir.c_str(), 0, 0);
          tft.fillRect(40, 0, 280, 30, TFT_WHITE);
          if (y < 100) {
            score = score + 50;
          }
          else if (y < 250) {
            score = score + 100;
          }
          else if (y < 400) {
            score = score + 200;
          }
          tft.setCursor(5, 5);
          tft.print("score:");
          tft.setCursor(5, 25);
          tft.print(score);
          spd = random(30, 40);
          obj = random(1, 3);
          y = 0;
        }
        else {
          tft.fillRect(127, y - spd, 64, 64, TFT_WHITE);
          if (obj == 1) {
            Userdir = String ("/User/") + User + String("/Data/Game/TouHou/Mini/image/object1.jpg");
            drawSdJpeg(Userdir.c_str(), 127, y);
          }
          else if (obj == 2) {
            Userdir = String ("/User/") + User + String("/Data/Game/TouHou/Mini/image/object2.jpg");
            drawSdJpeg(Userdir.c_str(), 127, y);
          }
          y = y + spd;

          if (y > 400) {
            y = 0;
            score = 0;
            state = 0;
            spd = random(30, 40);
            obj = random(1, 3);
            break;
          }
          else {
            delay(30);
          }
        }
      }
    }
    else {
      delay(30);
    }
  }
}

//Draw
int ColorPaletteHigh = 30; // Height of palette boxes
int color = TFT_WHITE;     //Starting paint brush color
int X_Coord;
int Y_Coord;
unsigned int colors[10] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_BLACK, TFT_CYAN, TFT_YELLOW, TFT_WHITE, TFT_MAGENTA, TFT_BLACK, TFT_BLACK};

void draw() {
  tft.setRotation(1);
  touch.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  for (int i = 0; i < 10; i++)
  {
    tft.fillRect(i * 32, 0, 32, ColorPaletteHigh, colors[i]);
  }
  tft.setCursor(264, 7, 2);
  tft.setTextColor(TFT_WHITE);
  tft.print("Clear");
  tft.drawRect(435,0,45,ColorPaletteHigh,TFT_WHITE);
  tft.setCursor(445, 7);
  tft.print("Quit");
  tft.drawRect(0, 0, 319, 30, TFT_WHITE);
  tft.fillRect(300, 9, 12, 12, color);

  while (1) {
    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if (Y_Coord < ColorPaletteHigh + 2) {
        if ((X_Coord / 32 > 7) && (X_Coord < 320)) {
          // Clear the screen to current colour
          tft.fillRect(0, 30, /*399*/ 480 , /*239*/ 320, color);
        }
        else if ((X_Coord > 435) && (Y_Coord < 25)){
          break;
        }
        else {
          color = colors[X_Coord / 32];
          tft.fillRect(300, 9, 12, 12, color);
        }
      }
      else {
        tft.fillCircle(X_Coord, Y_Coord, 2, color);
      }
    }
  }
}

//mp3 Player:
#include <AudioFileSourceFS.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

AudioGeneratorMP3 *mp3;
AudioFileSourceFS *file;
AudioOutputI2S *out;

int fileline = 1;

void MP3_start(const char *filename) {
  file = new AudioFileSourceFS(SD_MMC, filename);
  out = new AudioOutputI2S(0, 1, 128);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  while (1) {
    if (mp3->isRunning()) {
      if (!mp3->loop()) {
        fileline = 1;
        mp3->stop();
      }
    }
    else {
      Serial.printf("MP3 done\n");
      break;
    }
  }
}

//PCM Player:
void PCM_start(const char *AUDIOFILENAME) {
  i2s_config_t i2s_config_dac = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 490,
    .use_apll = false,
  };
  Serial.printf("%p\n", &i2s_config_dac);
  if (i2s_driver_install(I2S_NUM_0, &i2s_config_dac, 0, NULL) != ESP_OK) {
    Serial.println(F("ERROR: Unable to install I2S drives!"));
  }
  else {
    i2s_set_pin((i2s_port_t)0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_zero_dma_buffer((i2s_port_t)0);
    File aFile = SD_MMC.open(AUDIOFILENAME);
    if (!aFile || aFile.isDirectory()) {
      Serial.println(F("ERROR: Failed to open  file for reading!"));
    }
    else {
      uint8_t *aBuf = (uint8_t *)malloc(2940);
      if (!aBuf) {
        Serial.println(F("aBuf malloc failed!"));
      }
      Serial.println(F("PCM audio start"));
      aFile.read(aBuf, 980);
      i2s_write_bytes((i2s_port_t)0, (char *)aBuf, 980, 0);
      while (aFile.available()) {
        aFile.read(aBuf, 2940);
        i2s_write_bytes((i2s_port_t)0, (char *)aBuf, 980, 0);
        i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 980), 980, 0);
        i2s_write_bytes((i2s_port_t)0, (char *)(aBuf + 1960), 980, 0);
      }
      Serial.println(F("PCM audio end"));
      aFile.close();
      i2s_driver_uninstall((i2s_port_t)0);
    }
  }
}

//txt Player:
char buf[128];
unsigned long line = 1;
int tftline = 1;

String readFileLine(const char* path, int num) {
  Serial.printf("Reading file: %s line: %d\n", path, num);
  File file = SD_MMC.open(path);
  if (!file) {
    return ("Failed to open file for reading");
  }
  char* p = buf;
  while (file.available()) {
    char c = file.read();
    if (c == '\n') {
      num--;
      if (num == 0) {
        *(p++) = '\0';
        String s(buf);
        s.trim();
        return s;
      }
    }
    else if (num == 1) {
      *(p++) = c;
    }
  }
  file.close();
  return String("error");
}

//listDir
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.print(file.size() / 1024);
      Serial.println("KB");
      tft.print("  FILE: ");
      tft.print(file.name());
      tft.print("  SIZE: ");
      tft.print(file.size() / 1024);
      tft.println("KB");
    }
    file = root.openNextFile();
  }
}
String filesname;
void appendDir(fs::FS &fs, const char * dirname, String filename, uint8_t levels) {
  File root = fs.open(filename.c_str());
  Serial.println(filename);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else {
      Serial.println(file.name());
      File Configfile = SD_MMC.open(Userdir.c_str(), FILE_APPEND);
      Configfile.println(file.name());
      Configfile.close();
    }
    file = root.openNextFile();
  }
}

//Ble-internal DAC
#include <BluetoothA2DPSink.h>

BluetoothA2DPSink a2dp_sink;

void BleAudio(){
  const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
      .sample_rate = 44100,
      .bits_per_sample = (i2s_bits_per_sample_t) 16,
      .channel_format =  I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false
  };
  a2dp_sink.set_i2s_config(i2s_config);  
  a2dp_sink.start("ESP-HMI");
}

//Scrolling Words
TFT_eSprite img = TFT_eSprite(&tft);

void build_banner(int IWIDTH, int IHEIGHT,String msg, int xpos,int size,int font, uint16_t TextColor) {
  // We could just use fillSprite(color) but lets be a bit more creative...
  // Now print text on top of the graphics
  img.setTextSize(size);           // Font size scaling is x1
  img.setTextFont(font);           // Font 4 selected
  img.setTextColor(TextColor);  // Black text, no background colour
  img.setTextWrap(false);       // Turn of wrap so we can print past end of sprite
  // Need to print twice so text appears to wrap around at left and right edges
  img.setCursor(xpos, 2);  // Print text at xpos
  img.print(msg);
  img.setCursor(xpos - IWIDTH, 2); // Print text at xpos - sprite width
  img.print(msg);
}
void CreatCrollWords(int IWIDTH, int IHEIGHT, int WAIT, int X, int Y,int size,int font, uint16_t TextColor, String msg){
  //Serial.println("CreatCrollWords");
  // Create the sprite and clear background to black
  while(a == 1){
    img.createSprite(IWIDTH, IHEIGHT);
    //Serial.println("This will be exed once.");
    a = 0;
  }
  //Serial.println(pos);
  pos--;
  img.fillSprite(tft.color565(0, 128, 255));
  build_banner(IWIDTH, IHEIGHT, msg, pos, size, font, TextColor);
  img.pushSprite(X, Y);
  delay(WAIT);
  if (pos < 0) {
    img.deleteSprite();
    pos = IWIDTH;
    a = 1;
  }
}/*
void CreatCrollWords(int IWIDTH,int pos, int IHEIGHT, int WAIT, int X, int Y,int size,int font, uint16_t TextColor, String msg){
  while (1) {
    // Create the sprite and clear background to black
    img.createSprite(IWIDTH, IHEIGHT);
    for (int pos = IWIDTH; pos > 0; pos--) {
      img.fillSprite(tft.color565(0, 128, 255));
      build_banner(IWIDTH,IHEIGHT,msg, pos, size, font, TextColor);
      img.pushSprite(X, Y);
      delay(WAIT);
    }
    // Delete sprite to free up the memory
    img.deleteSprite();
  }
}*/

//Thermometer
#include <SHT3x.h>
SHT3x Sensor;

float Temperature = 0;
float Humidity = 0;
float H_Coordinate = 0;
float CurveX = 11;
int T_coordinate = 0;
int H_coordinate = 0;

void GetfromMometer(){
  Sensor.Begin();
  Sensor.UpdateData();
  Serial.print("Temperature: ");
  Serial.print(Sensor.GetTemperature());
  Serial.write("\xC2\xB0"); //°
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(Sensor.GetRelHumidity());
  Serial.println("%");
}

//Network
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>

#define NTP_OFFSET  28800
#define NTP_INTERVAL 1 * 1000
#define NTP_ADDRESS  "ntp.ntsc.ac.cn"

const char *ssid = "ESP-HMI";
const char *psword = "123456789";

String WiFiMode = "NULL";
String IPAD = "192.168.0.254";  //后两位与 IPAD1 IPAD2保持一致
uint8_t IPAD1 = 0;        //TP-LINK：0 ，水星：1 ，华为：3 ，小米：31
uint8_t IPAD2 = 254;      //自行设置 2~254

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
WiFiMulti wifiMulti;

void getNetTime(){
  timeClient.begin();
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int tm_Month = ptm->tm_mon + 1;
  int tm_Year = ptm->tm_year + 1900;
  timeClient.getFormattedTime();
}
void WiFiInit(String Mode){
  tft.setCursor(0,0);
  tft.setTextColor(TFT_WHITE);
  if (Mode == "AP") {
    WiFi.softAP(ssid, psword);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    tft.print("SSID: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(ssid);
    tft.setTextColor(TFT_WHITE);
    tft.print("AP IP address: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(myIP);
    tft.setTextColor(TFT_WHITE);
    WiFiMode = "AP";
  }
  else if (Mode == "STA") {
    IPAddress staticIP(192, 168, IPAD1, IPAD2);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress dns(8, 8, 8, 8);
    //WiFi.disconnect();
    WiFi.config(staticIP, gateway, subnet, dns);
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("TP-LINK_6A56", "bobo512w999580");
    wifiMulti.addAP("TP-LINK_6A56", "bobo512w999580");
    Serial.println("Connecting ...");
    tft.println("Connecting ...");
    while (wifiMulti.run() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
      tft.print(".");
    }
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

    tft.print("Connected to ");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.SSID());
    tft.setTextColor(TFT_WHITE);
    tft.print("IP address:\t");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.localIP());
    tft.setTextColor(TFT_WHITE);
    WiFiMode = "STA";
  }
}

//File transfer
#include <WebServer.h>

WebServer server(80);

File fsUploadFile;
File contentFile;

void handleFileUpload() {

  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {                   // 如果上传状态为UPLOAD_FILE_START

    String filename = upload.filename;                        // 建立字符串变量用于存放上传文件名
    if (!filename.startsWith("/")) filename = "/User/" + User + "/Data/upload/" + filename; // 为上传文件名前加上"/"
    Serial.println("File Name: " + filename);                 // 通过串口监视器输出上传文件的名称
    if (SD_MMC.exists(filename)) {
      contentFile = SD_MMC.open("/System/Server/DuplicateFileName.html");
      if (!contentFile) {
        Serial.println("读取文件失败");
        return;
      }
      server.streamFile(contentFile, "text/html");
      contentFile.close();
      server.send(500, "text/plain", "500: couldn't create file");
    }
    else {
      fsUploadFile = SD_MMC.open(filename, FILE_WRITE);            // 在SD卡中建立文件用于写入用户上传的文件数据
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {        // 如果上传状态为UPLOAD_FILE_WRITE

    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // 向SD卡文件写入浏览器发来的文件数据

  }
  else if (upload.status == UPLOAD_FILE_END) {          // 如果上传状态为UPLOAD_FILE_END
    if (fsUploadFile) {                                   // 如果文件成功建立
      fsUploadFile.close();                               // 将文件关闭
      //      Serial.println(" Size: "+ upload.totalSize);        // 通过串口监视器输出文件大小
      server.sendHeader("Location", "/System/Server/success.html"); // 将浏览器跳转到/success.html（成功上传页面）
      server.send(303);                               // 发送相应代码303（重定向到新页面）
    }
    else {                                              // 如果文件未能成功建立
      Serial.println("File upload failed");               // 通过串口监视器输出报错信息
      server.send(500, "text/plain", "500: couldn't create file"); // 向浏览器发送相应代码500（服务器错误）
    }
  }
}
//回复状态码 200 给客户端
void respondOK() {
  server.send(200);
}
//从文件路径中获取文件名
String indexOfFilename(String filename)  {
  char filename2[200];
  uint8_t len = 0;
  uint8_t cout = 0;
  uint8_t i, j = 0;
  len = filename.length();
  for (i = 0; i <= len; i++) {
    if (filename[i] == '/') {
      cout = i;
    }
  }
  for (i = cout + 1; i <= len; i++) {
    if (j < 195) {
      filename2[j] = filename[i];
      j++;
    }
  }
  j = 0;
  filename = filename2;
  return filename;
}
//计算文件大小
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}
void File_transfer_listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);
  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        File_transfer_listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
//列出上传的文件
String listUploadDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  String filename = "";
  String filename2 = "";
  String message = "";
  File root = fs.open(dirname);
  if (!root) {
    message += "Failed to open directory <br />";
    return message;
  }
  if (!root.isDirectory()) {
    message += "Not a directory <br />";
    return message;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      message += "  DIR : ";
      message += String(file.name()) + String("<br />");
      if (levels) {
        message += listUploadDir(fs, file.name(), levels - 1);
      }
    } else {
      filename = String(file.name());
      filename2 = indexOfFilename(filename);
      message += "  File: " + filename2 + "    <a href=\"" + filename + "\" download=\"" + filename2 + "\">[Download]</a>";
      message += "    <a href=\"" + filename + "\" target=\"_self\">" + "[View]" + "</a>";//直接查看功能
      message += String("<br />");
      message += String("  Size: ");
      message += formatBytes(file.size()) + String("<br /><br />");
    }
    file = root.openNextFile();
  }
  return message;
}
void listUploadFile() {
  String UPFDir = "/User/" + User + "/Data/upload";
  if (!server.authenticate(User.c_str(), password.c_str())) //校验用户是否登录
  {
    return server.requestAuthentication(); //请求进行用户登录认证
  }
  String header = "<html><body>";
  String message = header + "<h2>upload:</h2>";
  message += listUploadDir(SD_MMC, UPFDir.c_str(), 1);
  server.send(200, "text/html", message);
}
// 获取文件类型
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".tiff")) return "image/tiff";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".swf")) return "application/x-shockwave-flash";
  else if (filename.endsWith(".SWF")) return "application/x-shockwave-flash";
  else if (filename.endsWith(".acc")) return "audio/aac";
  else if (filename.endsWith(".mp3")) return "audio/mpeg";
  else if (filename.endsWith(".avi")) return "video/x-msvideo";
  else if (filename.endsWith(".mp4")) return "video/mp4";
  else if (filename.endsWith(".mpeg")) return "video/mpeg";
  else if (filename.endsWith(".m3u8")) return "application/x-mpegURL";
  else if (filename.endsWith(".m3u8")) return "video/MP2T";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
bool handleFileRead(String path) {            //处理浏览器HTTP访问
  if (path.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    path = "/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  }
  String contentType = getContentType(path);  // 获取文件类型
  if (SD_MMC.exists(path)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SD_MMC.open(path, FILE_READ);          // 则尝试打开该文件
    server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}
void handleNotFound() //未注册链接回调函数
{
  bool fileReadOK = handleFileRead(server.uri()); // 通过handleFileRead函数处处理用户访问
  if (!fileReadOK) {
    File PNFFile = SD_MMC.open("/System/Server/404.html");
    server.streamFile(PNFFile, "text/html");
    PNFFile.close();
  }
}
void ServerMainPage(){
  contentFile = SD_MMC.open("/System/Server/ESP-HMI.html");
  if (!contentFile) {
    Serial.println("读取文件失败");
    handleNotFound();
    return;
  }
  server.streamFile(contentFile, "text/html");
  contentFile.close();
}
void favicon(){
  contentFile = SD_MMC.open("/System/Server/favicon.ico");
  if (!contentFile) {
    Serial.println("读取文件失败");
    handleNotFound();
    return;
  }
  server.streamFile(contentFile, "image/ico");
  contentFile.close();
}
void File_transfer_init(){
  tft.setCursor(0,0);
  if (WiFiMode == "STA") {
    server.on("/",ServerMainPage);
    server.on("/favicon.ico", favicon);
    server.on("/System/Server/upload.html",   // 如果客户端通过upload页面
              HTTP_POST,
              respondOK,        // 则回复状态码 200 给客户端
              handleFileUpload);// 并且运行处理文件上传函数
    server.on("/filelist", HTTP_GET, listUploadFile);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Web服务器启动");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP());
    tft.println("WebServer Started");
    tft.print("Got IP: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.localIP());
    tft.setTextColor(TFT_WHITE);
    tft.print("Mode: ");
    tft.setTextColor(TFT_BLUE);
    tft.println(WiFiMode);
    tft.setTextColor(TFT_WHITE);
  }
  else if (WiFiMode == "AP") {
    server.on("/",ServerMainPage);
    server.on("/favicon.ico", favicon);
    server.on("/System/Server/upload.html",   // 如果客户端通过upload页面
              HTTP_POST,
              respondOK,        // 则回复状态码 200 给客户端
              handleFileUpload);// 并且运行处理文件上传函数
    server.on("/filelist", HTTP_GET, listUploadFile);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Web服务器启动");
    Serial.print("Got IP: ");
    Serial.println(WiFi.softAPIP());
    tft.println("WebServer Started");
    tft.print("Got IP: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.softAPIP());
    tft.setTextColor(TFT_WHITE);
    tft.print("Mode: ");
    tft.setTextColor(TFT_BLUE);
    tft.println(WiFiMode);
    tft.setTextColor(TFT_WHITE);
  }
  else {
    tft.print("Please connect to WiFi first.");
  }
}