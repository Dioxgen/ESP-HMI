#include <SD_MMC.h>
#include <TFT_eSPI.h>
#include <TFT_Touch.h>

#include "Media_Player.h"

void UserCheck(int a){
  tft.fillScreen(TFT_BLACK);
  delay(100);
  User = readFileLine("/System/Users/Users.txt", a);
  drawSdJpeg("/System/Widgets/signinbkd.jpg", 0, 0);
  jpgdir = String ("/User/") + User + String("/Data/Others/") + User + String(".jpg");
  if (SD_MMC.exists(jpgdir.c_str())){
    drawSdJpeg(jpgdir.c_str(), 40, 40);
  }
  else{
    drawSdJpeg("/System/Widgets/user.jpg", 40, 40);
  }
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(40,200);
  tft.print(User);
  
  drawSdJpeg("/System/Widgets/num_keyboard.jpg", 0, 270);
  tft.fillRoundRect(220,70, 220, 50, 8, TFT_WHITE);
  tft.setCursor(225, 40, 4);
  tft.setTextSize(1);
  tft.print("Password:");
  while (1) {
    tft.setCursor(225, 85);
    tft.print(password);
    if (touch.Pressed()) {
      delay(300);
      if ( 0 < touch.X() && touch.X() < 40 ) {
        password = password + String("0");
      }
      else if ( 40 < touch.X() && touch.X() < 80 ) {
        password = password + String("1");
      }
      else if ( 80 < touch.X() && touch.X() < 120 ) {
        password = password + String("2");
      }
      else if ( 120 < touch.X() && touch.X() < 160 ) {
        password = password + String("3");
      }
      else if ( 160 < touch.X() && touch.X() < 200 ) {
        password = password + String("4");
      }
      else if ( 200 < touch.X() && touch.X() < 240 ) {
        password = password + String("5");
      }
      else if ( 240 < touch.X() && touch.X() < 280 ) {
        password = password + String("6");
      }
      else if ( 280 < touch.X() && touch.X() < 320 ) {
        password = password + String("7");
      }
      else if ( 320 < touch.X() && touch.X() < 360 ) {
        password = password + String("8");
      }
      else if ( 360 < touch.X() && touch.X() < 400 ) {
        password = password + String("9");
      }
      else if ( 400 < touch.X()) {
        filedir = String ("/User/") + User + String("/Config/") + String("password.txt");
        if (password == readFileLine(filedir.c_str(), 1)){
          tft.setCursor(225, 85);
          tft.fillRoundRect(220,70, 220, 50, 8, TFT_WHITE);
          tft.print("Welcome!");
          Serial.println(password);
          delay(1000);
          break;
        }
        else{
          tft.setCursor(225, 85);
          tft.fillRoundRect(220,70, 220, 50, 8, TFT_WHITE);
          tft.print("Wrong Password!");
          Serial.println(password);
          delay(1000);
          password = "";
          UserCheck(b);
        }
      }
    }
  }
}
void listUser(int UserPage) {
  Serial.println(UserPage);

  int strlen = 0;
  int a = 1;
  
  String filename;
  String User_head_portrait;
  
  tft.fillScreen(tft.color565(214, 214, 214));
  tft.fillRoundRect(10,270, 460, 40, 8, TFT_BLUE);
  tft.setCursor(25, 280);
  tft.setTextColor(TFT_ORANGE);
  tft.print("Previous");
  tft.print("                                                     ");
  tft.print("Next");
  tft.setTextColor(TFT_BLACK);

  UserBline = (UserPage * 2) - 1;
  UserEline = UserPage + 1;

  strlen = strlen + 35;
  Serial.printf("strlen: %d\n",strlen);

  Serial.println(UserBline);
  Serial.println(UserEline);
  
  while (UserBline <= UserEline) {
    User = readFileLine("/System/Users/Users.txt", UserBline);
    strlen = sizeof(User);
    strlen = strlen + 23;
    filedir = String ("/User/") + User + String("/Data/Others/") + User + String(".jpg");
    User_head_portrait = filedir;
    Serial.println(User_head_portrait);
    if(a == 1) {
      if (SD_MMC.exists(User_head_portrait.c_str())){
        drawSdJpeg(User_head_portrait.c_str(),40,40);
      }
      else{
        drawSdJpeg("/System/Widgets/user.jpg", 40, 40);
      }
      tft.setCursor(40,200);
      tft.print(User);
    }
    else if(a == 2) {
      if (SD_MMC.exists(User_head_portrait.c_str())){
        drawSdJpeg(User_head_portrait.c_str(),290,40);
      }
      else{
        drawSdJpeg("/System/Widgets/user.jpg", 290, 40);
      }
      tft.setCursor(290,200);
      tft.print(User);
    }
    //Serial.println(a);
    UserBline++;
    a++;
  }
}
void Userlogin(){
  listUser(UserPage);

  while(1){
    if(UserPage < 1){
      UserPage = 1;
      listUser(UserPage);
    }
    if (touch.Pressed()) {
      if ((20 < touch.X()) && (touch.X() < 200) && (280 < touch.Y()) && (touch.Y() < 320)) {
        UserPage--;
        listUser(UserPage);
      }
      else if ((280 < touch.X()) && (touch.X() < 480) && (280 < touch.Y()) && (touch.Y() < 320)) {
        UserPage++;
        listUser(UserPage);
      }
      else if ((40 < touch.X()) && (touch.X() < 190) && (40 < touch.Y()) && (touch.Y() < 190)){
        b = (UserPage * 2) - 1;
        UserCheck(b);
        break;
      }
      else if ((290 < touch.X()) && (touch.X() < 440) && (40 < touch.Y()) && (touch.Y() < 190)){
        b = UserPage * 2;
        UserCheck(b);
        break;
      }
    }
    if (EnableUC == 1){
      EnableUC = 0;
      listUser(UserPage);
    }
  }
}

void Refresh() {
  Userdir = String("/User/") + User + String("/Data/Music/MusicData/AlbumArtSmall.jpg");
  SD_MMC.remove(Userdir.c_str());
  Userdir = String("/User/") + User + String("/Data/Music/MusicData/Folder.jpg");
  SD_MMC.remove(Userdir.c_str());

  Userdir = String("/User/") + User + String("/Config/EbookConfig.txt");
  filedir = String("/User/") + User + String("/Data/Ebook/EbookData");
  SD_MMC.remove(Userdir.c_str());
  appendDir(SD_MMC, Userdir.c_str() , filedir, 2);
  
  Userdir = String("/User/") + User + String("/Config/MusicConfig.txt");
  filedir = String("/User/") + User + String("/Data/Music/MusicData");
  SD_MMC.remove(Userdir.c_str());
  appendDir(SD_MMC, Userdir.c_str() , filedir, 2);
  Userdir = String("/User/") + User + String("/Config/MusicCoverConfig.txt");
  filedir = String("/User/") + User + String("/Data/Music/MusicCover");
  SD_MMC.remove(Userdir.c_str());
  appendDir(SD_MMC, Userdir.c_str() , filedir, 2);
  
  Userdir = String("/User/") + User + String("/Config/VideoConfig.txt");
  filedir = String("/User/") + User + String("/Data/Video/VideoData");
  SD_MMC.remove(Userdir.c_str());
  appendDir(SD_MMC, Userdir.c_str() , filedir, 3);

  Userdir = String("/User/") + User + String("/Config/ImageConfig.txt");
  filedir = String("/User/") + User + String("/Data/Image");
  SD_MMC.remove(Userdir.c_str());
  appendDir(SD_MMC, Userdir.c_str() , filedir, 3);
}

void NewUser(fs::FS &fs, const char *UserName) {
  Userdir = String("/User/") + UserName + String("/Config/UserConfig.txt");
  File file = fs.open(Userdir.c_str(), FILE_WRITE);
  file.close();
  Userdir = String("/User/") + UserName + String("/Config/EbookConfig.txt");
  file = fs.open(Userdir.c_str(), FILE_WRITE);
  file.close();
  Userdir = String("/User/") + UserName + String("/Config/GameConfig.txt");
  file = fs.open(Userdir.c_str(), FILE_WRITE);
  file.close();
  Userdir = String("/User/") + UserName + String("/Config/MusicConfig.txt");
  file = fs.open(Userdir.c_str(), FILE_WRITE);
  file.close();
  Userdir = String("/User/") + UserName + String("/Config/VideoConfig.txt");
  file = fs.open(Userdir.c_str(), FILE_WRITE);
  file.close();
}

//Album
void ImageCheck(int Imagenum){
  Image_num = Imagenum;
  tft.fillScreen(TFT_BLACK);
  int strlen = 0;
  String filename;

  strlen = sizeof(User);
  strlen = strlen + 13;
  filedir = String ("/User/") + User + String("/Config/ImageConfig.txt");
  filename = readFileLine(filedir.c_str(), Imagenum).substring(strlen);
  filename = filename.substring(0,filename.length()-4);
  ImageName = filename;
  Serial.print("ImageName:");
  Serial.println(ImageName);
  
  jpgdir = String ("/User/") + User + String("/Data/Image/") + ImageName + String(".jpg");
  drawSdJpeg(jpgdir.c_str(), 0, 0);

  while(1) {
    if(touch.Pressed()) {
      EnableIC = 1;
      break;
    }
  }
}
void listImage(int ImagePage){
  int strlen = 0;
  
  String filename;
  
  tft.fillScreen(TFT_WHITE);
  tft.fillRoundRect(10,270, 460, 40, 8, TFT_BLUE);
  tft.setCursor(25, 280);
  tft.setTextColor(TFT_ORANGE);
  tft.print("Previous");
  tft.print("                    ");
  tft.print("View");
  tft.print("                           ");
  tft.print("Next");
  tft.setTextColor(TFT_DARKGREEN);

  ImageBline = (ImagePage - 1) * 10 + 1;
  ImageEline = ImageBline + 9;
  strlen = strlen + 25;
  Serial.printf("strlen: %d\n",strlen);
  tft.setCursor(0,0);

  tft.setTextColor(TFT_DARKGREEN);

  while (ImageBline <= ImageEline) {
    filename = readFileLine(filedir.c_str(), ImageBline).substring(strlen);
    tft.println(filename.substring(0,filename.length()-4));
    //Serial.println(filename.substring(0,filename.length()-4));
    ImageBline++;
    //Serial.println(ImageBline);
  }
  tft.setTextColor(TFT_BLACK);
}
void Album() {
  int a = 55;
  int strlen = 0;
  String filename;

  //tft.fillScreen(tft.color565(0, 233, 255));
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);

  filedir = String ("/User/") + User + String("/Config/ImageConfig.txt");
  strlen = sizeof(User);
  
  listImage(ImagePage);

  while (1){

    if(ImagePage < 1){
      ImagePage = 1;
      listImage(ImagePage);
    }
    if (touch.Pressed()) {
      if ((460 < touch.X()) && (touch.X() < 480) && (0 < touch.Y()) && (touch.Y() < 20)) {
        break;
      }
      else if ((20 < touch.X()) && (touch.X() < 200) && (280 < touch.Y()) && (touch.Y() < 320)) {
        ImagePage--;
        listImage(ImagePage);
      }
      else if ((220 < touch.X()) && (touch.X() < 260) && (280 < touch.Y()) && (touch.Y() < 320)) {
        ImageCheck(Imagenum);
      }
      else if ((280 < touch.X()) && (touch.X() < 480) && (280 < touch.Y()) && (touch.Y() < 320)) {
        ImagePage++;
        listImage(ImagePage);
      }
      else if (0 < touch.Y() && touch.Y() < 30){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 1, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 1;
        a = 1;
      }
      else if (30 < touch.Y() && touch.Y() < 60){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 28, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 2;
        a = 28;
      }
      else if (60 < touch.Y() && touch.Y() < 90){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 53, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 3;
        a = 53;
      }
      else if (90 < touch.Y() && touch.Y() < 116){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 80, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 4;
        a = 80;
      }
      else if (116 < touch.Y() && touch.Y() < 142){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 105, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 5;
        a = 105;
      }
      else if (142 < touch.Y() && touch.Y() < 168){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 130, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 6;
        a = 130;
      }
      else if (168 < touch.Y() && touch.Y() < 194){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 156, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 7;
        a = 156;
      }
      else if (194 < touch.Y() && touch.Y() < 220){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 182, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 8;
        a = 182;
      }
      else if (220 < touch.Y() && touch.Y() < 246){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 208, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 9;
        a = 208;
      }
      else if (246 < touch.Y() && touch.Y() < 270){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 234, 479, 27, TFT_BLACK);
        Imagenum = (ImagePage -1) * 10 + 10;
        a = 234;
      }
      if(EnableIC == 1) {
        EnableIC = 0;
        listImage(ImagePage);
      }
    }
  }
}

void Game() {
  tft.setTextSize(3);
  tft.setCursor(0, 0, 0);
  tft.print("THMini");
  tft.setCursor(0, 30);
  tft.print("Draw");

  while (1) {
    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if (X_Coord > 400) {
        tft.fillScreen(TFT_BLACK);
        break;
      }
      if     ((0 < X_Coord) && (X_Coord < 50) && (0 < Y_Coord) && (Y_Coord < 30)) {
        THMini();
      }
      else if ((0 < X_Coord) && (X_Coord < 50) && (30 < Y_Coord) && (Y_Coord < 60)) {
        draw();
      }
    }
  }
}

//Sounder:
void MP3_start_Sounder(const char *afilename,String Musicname) {
  pos = 350;

  tft.fillRoundRect(300, 100, 150, 40, 8, TFT_DARKGREEN);
  tft.setCursor(315, 110);
  tft.setTextColor(TFT_ORANGE);
  tft.print("   Previous");
  tft.fillRoundRect(300, 170, 150, 40, 8, TFT_DARKGREEN);
  tft.setCursor(315, 180);
  tft.setTextColor(TFT_ORANGE);
  tft.print("   Suspend");
  tft.fillRoundRect(300, 240, 150, 40, 8, TFT_DARKGREEN);
  tft.setCursor(315, 250);
  tft.setTextColor(TFT_ORANGE);
  tft.print("       Next");

  file = new AudioFileSourceFS(SD_MMC, afilename);
  out = new AudioOutputI2S(0, 1, 128);
  out -> SetGain(1.0); //max 4.0
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  
  while (1) {
    if (mp3->isRunning()) {
      CreatCrollWords(450,30,0,300,40,1,4,TFT_WHITE,Musicname);
      if (touch.Pressed()) {
        Serial.println("Pressed");
        if (300 < touch.X() && touch.X() < 450 && 100 < touch.Y() && touch.Y() < 140){
          mp3->stop();
          //Serial.print("Before:");
          //Serial.println(Music_num);
          Music_num--;
          //Serial.print("After:");
          //Serial.println(Music_num);
          EnableMC = 1;
          break;
        }
        else if (((300 < touch.X()) && (touch.X() < 450)) && ((170 < touch.Y()) && (touch.Y() < 210))){
          mp3->stop();
          //Serial.printf("MP3 done\n");
          MusicPage = -1;
          break;
        }
        else if (300 < touch.X() && touch.X() < 450 && 240 < touch.Y() && touch.Y() < 280){
          mp3->stop();
          if (MusicState == "Random"){
            Music_num = random( 1, MusicEline - 1);
            EnableMC = 1;
          }
          else{
            Music_num++;
            EnableMC = 1;
          }
          break;
        }
        if (touch.Y() > 300) {
          mp3->stop();
          Serial.printf("MP3 stop\n");
          MusicPage = -1;
          break;
        }
      }
      if (!mp3->loop()) {
        mp3->stop();
      }
    }
    else {
      mp3->stop();
      Serial.printf("MP3 done\n");
      if(MusicState == "List Loop") {
        Music_num++;
        EnableMC = 1;
      }
      else if(MusicState == "Loop"){
        EnableMC = 1;
      }
      else {
        Music_num = random( 1, MusicEline - 1);
        EnableMC = 1;
      }
      break;
    }
  }
}
void MusicCheck(int Musicnum){
  Music_num = Musicnum;
  drawSdJpeg("/System/APP/Sounder/A.jpg", 0, 0);

  tft.fillRoundRect(0,300, 480, 30, 8, TFT_WHITE);
  int strlen = 0;
  String filename;
  String MusicCover;

  tft.setTextColor(TFT_WHITE);
  strlen = sizeof(User);
  strlen = strlen + 23;
  filedir = String ("/User/") + User + String("/Config/MusicConfig.txt");
  filename = readFileLine(filedir.c_str(), Musicnum).substring(strlen);
  filename = filename.substring(0,filename.length()-4);
  MusicName = filename;
  //tft.setCursor(300,40);
  //Serial.println(sizeof(filename));

  filedir = String ("/User/") + User + String("/Config/MusicConfig.txt");
  MusicCover = String("/User/") + User +String("/Data/Music/MusicCover/") + MusicName + String(".jpg");
  Serial.println(MusicCover);

  if (SD_MMC.exists(MusicCover.c_str())){
    drawSdJpeg(MusicCover.c_str(), 40, 40);
  }
  else{
    drawSdJpeg("/System/APP/Sounder/M_Album.jpg", 40, 40);
  }
  MP3_start_Sounder(readFileLine(filedir.c_str(), Musicnum).c_str(), MusicName);

  if (EnableMC == 1){
    EnableMC = 0;
    MusicCheck(Music_num);
  }
}
void listMusic(int MusicPage){
  Serial.println(MusicPage);
  int strlen = 0;
  
  String filename;
  
  tft.fillRoundRect(0,50, 480, 280, 8, TFT_WHITE);
  tft.fillRoundRect(10,270, 460, 40, 8, TFT_BLUE);
  tft.setCursor(25, 280);
  tft.setTextColor(TFT_ORANGE);
  tft.print("Previous");
  tft.print("                    ");
  tft.print("Play");
  tft.print("                           ");
  tft.print("Next");
  tft.setTextColor(TFT_DARKGREEN);

  MusicBline = (MusicPage - 1) * 8 + 1;
  MusicEline = MusicBline + 7;
  strlen = strlen + 35;
  Serial.printf("strlen: %d\n",strlen);
  //tft.setCursor(470,230);
  //tft.print(MusicPage);
  tft.setCursor(0,60);
  Serial.println(MusicBline);
  Serial.println(MusicEline);

  tft.setTextColor(TFT_DARKGREEN);

  while (MusicBline <= MusicEline) {
    filename = readFileLine(filedir.c_str(), MusicBline).substring(strlen);
    tft.println(filename.substring(0,filename.length()-4));
    //Serial.println(filename.substring(0,filename.length()-4));
    MusicBline++;
    //Serial.println(MusicBline);
  }
  tft.setTextColor(TFT_BLACK);
}
void MusicSet(){
  tft.fillRoundRect(350,0, 140, 320, 8, tft.color565(230, 230, 230));
  drawSdJpeg("/System/APP/Sounder/List Loop.jpg", 360, 10);
  drawSdJpeg("/System/APP/Sounder/Ble.jpg", 360, 130);

  MS_num = 0;

  while(1){
    if (touch.Pressed()) {
      if ((360 < touch.X()) && (touch.X() < 470) && (10 < touch.Y()) && (touch.Y() < 120)) {
        MS_num++;

        if (MS_num == 1){
          MusicState = "List Loop";
          drawSdJpeg("/System/APP/Sounder/List Loop.jpg", 360, 10);
          delay(100);
        }
        else if (MS_num == 2){
          MusicState = "Loop";
          drawSdJpeg("/System/APP/Sounder/Loop.jpg", 360, 10);
          delay(100);
        }
        else if (MS_num == 3){
          MusicState = "Random";
          drawSdJpeg("/System/APP/Sounder/Random.jpg", 360, 10);
          delay(100);
        }
      }
      else if ((360 < touch.X()) && (touch.X() < 470) && (130 < touch.Y()) && (touch.Y() < 240)) {
        drawSdJpeg("/System/APP/Sounder/BL.jpg", 0, 0);
        BleAudio();
        while(1) {
          if(touch.Pressed()){
            if ((200 < touch.X()) && (touch.X() < 280) && (250 < touch.Y()) && (touch.Y() < 300)) {
              break;
            }
          }
        }
        drawSdJpeg("/System/APP/Sounder/A.jpg", 0, 0);
        MusicPage = -1;
        break;
      }
      else {
        MusicPage = -1;
        break;
      }
    }
    else if (MS_num > 3){
      MS_num = 0;
    }
  }
}
void Music(){
  int a = 55;
  int strlen = 0;
  //int Musicnum = 1;
  //int MusicPage = 1;
  String filename;

  drawSdJpeg("/System/APP/Sounder/A.jpg", 0, 0);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);

  filedir = String ("/User/") + User + String("/Config/MusicConfig.txt");
  strlen = sizeof(User);
  Serial.printf("strlen: %d\n",strlen);
//  strlen = strlen + sizeof(String("/User/SoTWild/Data/Music/MusicData/"));
  
  listMusic(MusicPage);

  while (1){

    if(MusicPage < 1){
      MusicPage = 1;
      listMusic(MusicPage);
    }
    if (touch.Pressed()) {
      if ((440 < touch.X()) && (touch.X() < 480) && (60 < touch.Y()) && (touch.Y() < 100)) {
        MusicSet();
      }
      else if ((460 < touch.X()) && (touch.X() < 480) && (0 < touch.Y()) && (touch.Y() < 20)) {
        break;
      }
      else if ((20 < touch.X()) && (touch.X() < 200) && (280 < touch.Y()) && (touch.Y() < 320)) {
        MusicPage--;
        listMusic(MusicPage);
      }
      else if ((220 < touch.X()) && (touch.X() < 260) && (280 < touch.Y()) && (touch.Y() < 320)) {
        MusicCheck(Musicnum);
      }
      else if ((280 < touch.X()) && (touch.X() < 480) && (280 < touch.Y()) && (touch.Y() < 320)) {
        MusicPage++;
        listMusic(MusicPage);
      }
      else if (60 < touch.Y() && touch.Y() < 90){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 55, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 1;
        a = 55;
      }
      else if (90 < touch.Y() && touch.Y() < 116){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 80, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 2;
        a = 80;
      }
      else if (116 < touch.Y() && touch.Y() < 142){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 105, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 3;
        a = 105;
      }
      else if (142 < touch.Y() && touch.Y() < 168){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 130, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 4;
        a = 130;
      }
      else if (168 < touch.Y() && touch.Y() < 194){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 156, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 5;
        a = 156;
      }
      else if (194 < touch.Y() && touch.Y() < 220){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 182, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 6;
        a = 182;
      }
      else if (220 < touch.Y() && touch.Y() < 246){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 208, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 7;
        a = 208;
      }
      else if (246 < touch.Y() && touch.Y() < 270){
        tft.drawRect(-1, a, 479, 27, TFT_WHITE);
        tft.drawRect(-1, 234, 479, 27, TFT_BLACK);
        Musicnum = (MusicPage -1) * 8 + 8;
        a = 234;
      }
    }
  }
}

//Vision:
void VideoCheck(int videonum){

  int strlen;
  String filename;
  String Vfile;
  String Afile;

  tft.fillScreen(TFT_BLACK);
  IfVision = 1;
  strlen = sizeof(User);
  strlen = strlen + 23;
  filedir = String ("/User/") + User + String("/Config/VideoConfig.txt");
  filename = readFileLine(filedir.c_str(), videonum * 2 - 1).substring(strlen);
  filename = filename.substring(0,filename.length() - 6 );//.mjpeg
  VideoName = filename;

  Vfile = String("/User/") + User +String("/Data/Video/VideoData/") + VideoName + String(".mjpeg");
  Afile = String("/User/") + User +String("/Data/Video/VideoData/") + VideoName + String(".pcm");
  Mjpeg_start(Vfile.c_str(),Afile.c_str());
}
void listVideo(int VideoPage){
  Serial.println(VideoPage);

  int strlen = 0;
  int a = 1;
  
  String filename;
  String VideoCover;
  
  tft.fillScreen(tft.color565(214, 214, 214));
  tft.fillRoundRect(10,270, 460, 40, 8, TFT_BLUE);
  tft.setCursor(25, 280);
  tft.setTextColor(TFT_ORANGE);
  tft.print("Previous");
  tft.print("                  ");
  tft.print("Settings");
  tft.print("                     ");
  tft.print("Next");
  tft.setTextColor(TFT_DARKGREEN);
  tft.setCursor(0, 0);

  VideoBline = (VideoPage - 1) * 8 + 1;
  VideoEline = VideoBline + 7;

  strlen = strlen + 35;
  Serial.printf("strlen: %d\n",strlen);

  Serial.println(VideoBline);
  Serial.println(VideoEline);

  while (VideoBline < VideoEline) {
    strlen = sizeof(User);
    strlen = strlen + 23;
    filedir = String ("/User/") + User + String("/Config/VideoConfig.txt");
    filename = readFileLine(filedir.c_str(), VideoBline).substring(strlen);
    filename = filename.substring(0,filename.length() - 6 );//.mjpeg
    VideoName = filename;
    //Serial.println(VideoName);
    VideoCover = String("/User/") + User +String("/Data/Video/VideoCover/") + VideoName + String(".jpg");//封面路径
    //tft.println(VideoCover);
    if(a == 1) {
      drawSdJpeg(VideoCover.c_str(),40,20);
      //tft.setCursor(40,130);
      //tft.print(VideoName);
    }
    else if(a == 2) {
      drawSdJpeg(VideoCover.c_str(),260,20);
      //tft.setCursor(260,130);
      //tft.print(VideoName);
    }
    else if(a == 3) {
      drawSdJpeg(VideoCover.c_str(),40,160);
      //tft.setCursor(40,270);
      //tft.print(VideoName);
    }
    else {
      drawSdJpeg(VideoCover.c_str(),260,160);
      //tft.setCursor(260,270);
      //tft.print(VideoName);
    }
    //Serial.println(a);
    VideoBline = VideoBline + 2;
    a++;
  }
}
void Video(){
  filedir = String ("/User/") + User + String("/Config/VideoCoverConfig.txt");
  listVideo(VideoPage);

  while(1){
    if(VideoPage < 1){
      VideoPage = 1;
      listVideo(VideoPage);
    }
    if (touch.Pressed()) {
      if ((220 < touch.X()) && (touch.X() < 260) && (280 < touch.Y()) && (touch.Y() < 320)) {
        //MusicSet();
      }
      else if ((460 < touch.X()) && (touch.X() < 480) && (0 < touch.Y()) && (touch.Y() < 20)) {
        break;
      }
      else if ((20 < touch.X()) && (touch.X() < 200) && (280 < touch.Y()) && (touch.Y() < 320)) {
        VideoPage--;
        listVideo(VideoPage);
      }
      else if ((280 < touch.X()) && (touch.X() < 480) && (280 < touch.Y()) && (touch.Y() < 320)) {
        VideoPage++;
        listVideo(VideoPage);
      }
      else if ((40 < touch.X()) && (touch.X() < 220) && (40 < touch.Y()) && (touch.Y() < 140)) {
        Videonum = VideoPage + 3 * (VideoPage - 1);
        VideoCheck(Videonum);
      }
      else if ((260 < touch.X()) && (touch.X() < 440) && (40 < touch.Y()) && (touch.Y() < 140)) {
        Videonum = VideoPage + 3 * (VideoPage - 1) + 1;
        VideoCheck(Videonum);
      }
      else if ((40 < touch.X()) && (touch.X() < 220) && (160 < touch.Y()) && (touch.Y() < 260)) {
        Videonum = VideoPage + 3 * (VideoPage - 1) + 2;
        VideoCheck(Videonum);
      }
      else if ((260 < touch.X()) && (touch.X() < 440) && (160 < touch.Y()) && (touch.Y() < 260)) {
        Videonum = VideoPage + 3 * (VideoPage - 1) + 3;
        VideoCheck(Videonum);
      }
    }
    if (EnableVC == 1){
      EnableVC = 0;
      listVideo(VideoPage);
    }
  }
}

//Ebook
void PlayEbook(const char * path){
  Serial.print("playbook");
  Serial.println(path);
  tft.setTextSize(1);
  line = 1;
  while (1) {
    if (touch.Pressed()) {
      tft.fillScreen(TFT_BLACK);
      if (touch.X() > 100 ) {
        tftline = 1;
        tft.setCursor(0, 0);
        while (tftline < 10) {
          tft.println(readFileLine(path, line));
          line++;
          tftline++;
        }
      }
      else {
        EnableBC = 1;
        break;
      }
    }
  }
}
void EbookCheck(int booknum){
  tft.fillScreen(TFT_BLACK);
  delay(100);
  filedir = String ("/User/") + User + String("/Config/EbookConfig.txt");
  PlayEbook(readFileLine(filedir.c_str(), booknum).c_str());
}
void listEbook(int BookPage){
  Serial.println(BookPage);

  int strlen = 0;
  int a = 1;
  
  String filename;
  String BookCover;
  
  tft.fillScreen(tft.color565(214, 214, 214));
  tft.fillRoundRect(10,270, 460, 40, 8, TFT_BLUE);
  tft.setCursor(25, 280);
  tft.setTextColor(TFT_ORANGE);
  tft.print("Previous");
  tft.print("                  ");
  tft.print("Settings");
  tft.print("                     ");
  tft.print("Next");
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);

  BookBline = (BookPage * 2) - 1;
  BookEline = BookPage + 1;

  strlen = strlen + 35;
  Serial.printf("strlen: %d\n",strlen);

  Serial.println(BookBline);
  Serial.println(BookEline);

  while (BookBline < BookEline) {
    strlen = sizeof(User);
    strlen = strlen + 23;
    filedir = String ("/User/") + User + String("/Config/EbookConfig.txt");
    filename = readFileLine(filedir.c_str(), BookBline).substring(strlen);
    filename = filename.substring(0,filename.length() - 4 );//.txt
    BookName = filename;
    Serial.printf("BookName: %s\n", BookName.c_str());
    BookCover = String("/User/") + User +String("/Data/Ebook/EbookCover/") + BookName + String(".jpg");//封面路径
    //tft.println(BookCover);
    if(a == 1) {
      drawSdJpeg(BookCover.c_str(),40,40);
    }
    else if(a == 2) {
      drawSdJpeg(BookCover.c_str(),260,40);
    }
    //Serial.println(a);
    BookBline++;
    a++;
  }
}
void Ebook(){
  filedir = String ("/User/") + User + String("/Config/EbookConfig.txt");
  listEbook(BookPage);

  while(1){
    if(BookPage < 1){
      BookPage = 1;
      listEbook(BookPage);
    }
    if (touch.Pressed()) {
      if ((220 < touch.X()) && (touch.X() < 260) && (280 < touch.Y()) && (touch.Y() < 320)) {
        //BookSet();
      }
      else if ((460 < touch.X()) && (touch.X() < 480) && (0 < touch.Y()) && (touch.Y() < 20)) {
        break;
      }
      else if ((20 < touch.X()) && (touch.X() < 200) && (280 < touch.Y()) && (touch.Y() < 320)) {
        BookPage--;
        listEbook(BookPage);
      }
      else if ((280 < touch.X()) && (touch.X() < 480) && (280 < touch.Y()) && (touch.Y() < 320)) {
        BookPage++;
        listEbook(BookPage);
      }
      else if ((40 < touch.X()) && (touch.X() < 180) && (40 < touch.Y()) && (touch.Y() < 240)){
        EbookCheck((BookPage * 2) - 1);
      }
      else if ((260 < touch.X()) && (touch.X() < 400) && (40 < touch.Y()) && (touch.Y() < 240)){
        EbookCheck(BookPage * 2);
      }
    }
    if (EnableBC == 1){
      EnableBC = 0;
      listEbook(BookPage);
    }
  }
}

//Settings
void Settings(){
  int a = 0;
  if (a == 0){
    drawSdJpeg("/System/APP/Settings/Settings_home.jpg", 0, 0);
  }
  while(1){//set home
    if (a == 1) {
      a = 0;
      drawSdJpeg("/System/APP/Settings/Settings_home.jpg", 0, 0);
    }
    if (touch.Pressed()){
      if ((460 < touch.X()) && (touch.X() < 480) && (0 < touch.Y()) && (touch.Y() < 20)) {
        break;
      }
      else if (30 < touch.X() && touch.X() < 90 && 100 < touch.Y() && touch.Y() < 120){
        drawSdJpeg("/System/APP/Settings/Settings_system_screen.jpg", 0, 0);
        while(1){//set system
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              Serial.print("Break");
              break;
            }
            else if((0 < touch.X()) && (touch.X() < 60) && (80 < touch.Y()) && (touch.Y() < 110)){
              drawSdJpeg("/System/APP/Settings/Settings_system_audio.jpg", 0, 0);
              while(1){
                if (touch.Pressed()){
                  if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
                    a = 1;
                    break;
                  }
                  /*else if(){
                  }*/
                }
              }
            }
            else if((0 < touch.X()) && (touch.X() < 60) && (110 < touch.Y()) && (touch.Y() < 135)){
              drawSdJpeg("/System/APP/Settings/Settings_system_notice.jpg", 0, 0);
              Userdir = String("/User/") + User + String("/Config/notice.txt");
              tft.setCursor(115,15);
              tft.print(readFileLine(Userdir.c_str(),1));
              while(1){
                if (touch.Pressed()){
                  if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
                    a = 1;
                    break;
                  }
                  else if((115 < touch.X()) && (touch.X() < 160) && (15 < touch.Y()) && (touch.Y() < 450)){
                    notice = !notice;
                    Serial.println(notice);
                    SD_MMC.remove(Userdir.c_str());
                    File file = SD_MMC.open(Userdir.c_str(), FILE_WRITE);
                    file.println(notice + String("Auto add text,do not delete."));
                    file.close();
                    tft.setCursor(115,15);
                    n_otice = readFileLine(Userdir.c_str(),1);
                    n_otice = n_otice.substring(0,1);
                    tft.print(n_otice);
                  }
                }
              }
            }
            else if((0 < touch.X()) && (touch.X() < 60) && (135 < touch.Y()) && (touch.Y() < 160)){
              drawSdJpeg("/System/APP/Settings/Settings_system_battery.jpg", 0, 0);
              while(1){
                if (touch.Pressed()){
                  if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
                    a = 1;
                    break;
                  }
                  /*else if(){

                  }*/
                }
              }
            }
            else if((0 < touch.X()) && (touch.X() < 60) && (160 < touch.Y()) && (touch.Y() < 190)){
              drawSdJpeg("/System/APP/Settings/Settings_system_storage.jpg", 0, 0);
              while(1){
                if (touch.Pressed()){
                  if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
                    a = 1;
                    break;
                  }
                  /*else if(){

                  }*/
                }
              }
            }
            else if((0 < touch.X()) && (touch.X() < 60) && (190 < touch.Y()) && (touch.Y() < 220)){
              drawSdJpeg("/System/APP/Settings/Settings_system_about.jpg", 0, 0);
              while(1){
                if (touch.Pressed()){
                  if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
                    a = 1;
                    break;
                  }
                  /*else if(){

                  }*/
                }
              }
            }
          }
        }
      }
      else if (140 < touch.X() && touch.X() < 230 && 100 < touch.Y() && touch.Y() < 120){
        drawSdJpeg("/System/APP/Settings/Settings_individualization.jpg", 0, 0);
        while(1){//set individualization
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
      else if (280 < touch.X() && touch.X() < 340 && 100 < touch.Y() && touch.Y() < 120){
        drawSdJpeg("/System/APP/Settings/Settings_account.jpg", 0, 0);
        while(1){//set account
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
      else if (390 < touch.X() && touch.X() < 450 && 100 < touch.Y() && touch.Y() < 120){
        drawSdJpeg("/System/APP/Settings/Settings_time.jpg", 0, 0);
        while(1){//set time
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
      else if (30 < touch.X() && touch.X() < 90 && 150 < touch.Y() && touch.Y() < 180){
        drawSdJpeg("/System/APP/Settings/Settings_file.jpg", 0, 0);
        while(1){//set file
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
      else if (140 < touch.X() && touch.X() < 230 && 150 < touch.Y() && touch.Y() < 180){
        drawSdJpeg("/System/APP/Settings/Settings_update.jpg", 0, 0);
        while(1){//set update
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
      else if (140 < touch.X() && touch.X() < 230 && 150 < touch.Y() && touch.Y() < 180){
        drawSdJpeg("/System/APP/Settings/Settings_wifi.jpg", 0, 0);
        while(1){//set wifi
          if (touch.Pressed()){
            if ((0 < touch.X()) && (touch.X() < 80) && (290 < touch.Y()) && (touch.Y() < 320)) {
              a = 1;
              break;
            }
            /*else if(){

            }*/
          }
        }
      }
    }
  }
}

//Calculator
void Calculator(){
  tft.setRotation(0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  touch.setRotation(0);
  drawSdJpeg("/System/Widgets/Calculator_kb.jpg", 0, 0);
  tft.setCursor(185,125);
  tft.print("Back / Clear");
  tft.setCursor(5,5);

  str_CalNum1 = "";
  str_CalNum2 = "";
  decimal_point = 0;
  inputNum = 1;
  Operation_mode = 1;
  CalNum1 = 0;
  CalNum2 = 0;
  CalResult = 0;
  Cal_negative = 0;
  Operation_mode_change = 0;

  while (1){
    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if (0 < X_Coord && X_Coord < 80 && 160 < Y_Coord && Y_Coord < 240) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "1";
        }
        else {
          str_CalNum2 = str_CalNum2 + "1";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (80 < X_Coord && X_Coord < 160 && 160 < Y_Coord && Y_Coord < 240) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "2";
        }
        else {
          str_CalNum2 = str_CalNum2 + "2";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (160 < X_Coord && X_Coord < 240 && 160 < Y_Coord && Y_Coord < 240) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "3";
        }
        else {
          str_CalNum2 = str_CalNum2 + "3";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (240 < X_Coord && X_Coord < 320 && 160 < Y_Coord && Y_Coord < 240) {// /
        if (Operation_mode_change == 0) {
          inputNum = 2;
          decimal_point = 0;
          Operation_mode = 4;
          Operation_mode_change = 1;
          tft.fillRect(240,0,35,50,tft.color565(54, 68, 79));
          tft.setCursor(240,0);
          tft.setTextSize(2);
          tft.setTextColor(tft.color565(255, 255, 0));
          tft.print("/");
          tft.setCursor(5,30);
          tft.setTextSize(1);
          tft.setTextColor(TFT_WHITE);
        }
      }
      else if (0 < X_Coord && X_Coord < 80 && 240 < Y_Coord && Y_Coord < 320) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "4";
        }
        else {
          str_CalNum2 = str_CalNum2 + "4";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (80 < X_Coord && X_Coord < 160 && 240 < Y_Coord && Y_Coord < 320) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "5";
        }
        else {
          str_CalNum2 = str_CalNum2 + "5";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (160 < X_Coord && X_Coord < 240 && 240 < Y_Coord && Y_Coord < 320) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "6";
        }
        else {
          str_CalNum2 = str_CalNum2 + "6";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (240 < X_Coord && X_Coord < 320 && 240 < Y_Coord && Y_Coord < 320) {// *
        if (Operation_mode_change == 0) {
          inputNum = 2;
          decimal_point = 0;
          Operation_mode = 3;
          Operation_mode_change = 1;
          tft.fillRect(240,0,35,50,tft.color565(54, 68, 79));
          tft.setCursor(240,0);
          tft.setTextSize(2);
          tft.setTextColor(tft.color565(255, 255, 0));
          tft.print("x");
          tft.setCursor(5,30);
          tft.setTextSize(1);
          tft.setTextColor(TFT_WHITE);
        }
      }
      else if (0 < X_Coord && X_Coord < 80 && 320 < Y_Coord && Y_Coord < 400) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "7";
        }
        else {
          str_CalNum2 = str_CalNum2 + "7";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (80 < X_Coord && X_Coord < 160 && 320 < Y_Coord && Y_Coord < 400) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "8";
        }
        else {
          str_CalNum2 = str_CalNum2 + "8";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (160 < X_Coord && X_Coord < 240 && 320 < Y_Coord && Y_Coord < 400) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "9";
        }
        else {
          str_CalNum2 = str_CalNum2 + "9";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (240 < X_Coord && X_Coord < 320 && 320 < Y_Coord && Y_Coord < 400) {// -
        if (strlen(str_CalNum1.c_str()) == 0 && Operation_mode_change == 0){
          str_CalNum1 = str_CalNum1 + "-";
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else if (strlen(str_CalNum1.c_str()) != 0 && Operation_mode_change == 0) {
          inputNum = 2;
          decimal_point = 0;
          Operation_mode = 2;
          Operation_mode_change = 1;
          tft.fillRect(240,0,35,50,tft.color565(54, 68, 79));
          tft.setCursor(240,0);
          tft.setTextSize(2);
          tft.setTextColor(tft.color565(255, 255, 0));
          tft.print("-");
          tft.setCursor(5,30);
          tft.setTextSize(1);
          tft.setTextColor(TFT_WHITE);
        }
        else if (strlen(str_CalNum2.c_str()) == 0 && Operation_mode_change == 1){
          str_CalNum2 = str_CalNum2 + "-";
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (0 < X_Coord && X_Coord < 80 && 400 < Y_Coord && Y_Coord < 480) {
        if (inputNum == 1){
          str_CalNum1 = str_CalNum1 + "0";
        }
        else {
          str_CalNum2 = str_CalNum2 + "0";
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (80 < X_Coord && X_Coord < 160 && 400 < Y_Coord && Y_Coord < 480) {
        if (decimal_point == 0){
          if (inputNum == 1){
            str_CalNum1 = str_CalNum1 + ".";
          }
          else {
            str_CalNum2 = str_CalNum2 + ".";
          }
          decimal_point = 1;
        }
        if (inputNum == 1){
          tft.setCursor(5,5);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.print(str_CalNum2);
        }
      }
      else if (160 < X_Coord && X_Coord < 240 && 400 < Y_Coord && Y_Coord < 480) {// =
        CalNum1 = str_CalNum1.toFloat();
        CalNum2 = str_CalNum2.toFloat();
        tft.setTextColor(tft.color565(100, 255, 100));
        tft.setTextSize(2);
        tft.setCursor(5,60);
        tft.print("=");
        if (Operation_mode == 1){
          CalResult = CalNum1 + CalNum2;
          tft.printf("%.5f",CalResult);
        }
        else if (Operation_mode == 2){
          CalResult = CalNum1 - CalNum2;
          tft.printf("%.5f",CalResult);
        }
        else if (Operation_mode == 3){
          CalResult = CalNum1 * CalNum2;
          tft.printf("%.5f",CalResult);
        }
        else {
          CalResult = CalNum1 / CalNum2;
          tft.printf("%.5f",CalResult);
        }
      }
      else if (240 < X_Coord && X_Coord < 320 && 400 < Y_Coord && Y_Coord < 480) {// +
        if (Operation_mode_change == 0) {
          inputNum = 2;
          decimal_point = 0;
          Operation_mode = 1;
          Operation_mode_change = 1;
          tft.fillRect(240,0,35,50,tft.color565(54, 68, 79));
          tft.setCursor(240,0);
          tft.setTextSize(2);
          tft.setTextColor(tft.color565(255, 255, 0));
          tft.print("+");
          tft.setCursor(5,30);
          tft.setTextSize(1);
          tft.setTextColor(TFT_WHITE);
        }
      }
      else if (270 < X_Coord && X_Coord < 320 && 0 < Y_Coord && Y_Coord < 80) {// f(x)
        tft.fillScreen(TFT_BLACK);
        tft.setRotation(1);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(1, 270);
        touch.setRotation(0);

        drawSdJpeg("/System/Widgets/Coordinate_axis.jpg", 0, 0);
        tft.fillCircle(20,280,20,TFT_ORANGE);
        tft.print("+");

        while(1) {
          if (touch.Pressed()) {
            X_Coord = touch.X();
            Y_Coord = touch.Y();
            if (0 < X_Coord && X_Coord < 40 && 300 < Y_Coord && Y_Coord < 280) {
              drawSdJpeg("/System/Widgets/Select_function.jpg", 0, 0);
              while(1) {
                if (touch.Pressed()) {
                  X_Coord = touch.X();
                  Y_Coord = touch.Y();
                  if (0 < X_Coord && X_Coord < 40 && 300 < Y_Coord && Y_Coord < 280) {

                  }
                }
              }
            }
          }
        }
      }
      else if (255 < X_Coord && X_Coord < 320 && 100 < Y_Coord && Y_Coord < 140) {// C
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(1);
        touch.setRotation(0);
        drawSdJpeg("/System/Widgets/Calculator_kb.jpg", 0, 0);
        tft.setCursor(185,125);
        tft.print("Back / Clear");
        tft.setCursor(5,5);

        str_CalNum1 = "";
        str_CalNum2 = "";
        decimal_point = 0;
        inputNum = 1;
        Operation_mode = 1;
        CalNum1 = 0;
        CalNum2 = 0;
        CalResult = 0;
        Cal_negative = 0;
        Operation_mode_change = 0;
      }
      else if (190 < X_Coord && X_Coord < 255 && 100 < Y_Coord && Y_Coord < 140) {//Back
        if(inputNum == 1){
          tft.setCursor(5,5);
          tft.setTextColor(tft.color565(54, 68, 79));
          tft.print(str_CalNum1);
          str_CalNum1 = str_CalNum1.substring(0,str_CalNum1.length() - 1);
          tft.setCursor(5,5);
          tft.setTextColor(TFT_WHITE);
          tft.print(str_CalNum1);
        }
        else {
          tft.setCursor(5,30);
          tft.setTextColor(tft.color565(54, 68, 79));
          tft.print(str_CalNum2);
          str_CalNum2 = str_CalNum2.substring(0,str_CalNum2.length() - 1);
          tft.setCursor(5,30);
          tft.setTextColor(TFT_WHITE);
          tft.print(str_CalNum2);
        }
      }
      else if (0 < X_Coord && X_Coord < 30 && 0 < Y_Coord && Y_Coord < 30) {
        tft.setRotation(1);
        touch.setRotation(0);
        break;
      }
      delay(250);
    }
  }
}

//Thermometer
void DrawDataCurve(){
  T_coordinate = Temperature / 1;
  T_coordinate = T_coordinate * 4;
  T_coordinate = 280 - T_coordinate;

  H_Coordinate = Humidity / 1;
  H_Coordinate = H_Coordinate * 2.4;
  H_Coordinate = 320 - H_Coordinate;
  H_coordinate = H_Coordinate / 1;

  tft.drawPixel(CurveX,T_coordinate,TFT_GREEN);
  tft.drawPixel(CurveX,H_coordinate,TFT_RED);
  if (CurveX > 480) {
    tft.fillRect(0,80,480,320,TFT_BLACK);
    tft.drawFastHLine(0,280,480,TFT_WHITE);
    tft.drawFastVLine(10,80,240,TFT_WHITE);
    CurveX = 11;
  }
  else {
    CurveX = CurveX + 0.25;
  }
}
void Thermometer(){
  tft.setTextSize(1);
  tft.drawFastHLine(0,280,480,TFT_WHITE);
  tft.drawFastVLine(10,80,240,TFT_WHITE);
  while(1) {
    Sensor.UpdateData();
    Temperature = Sensor.GetTemperature();
    Humidity = Sensor.GetRelHumidity();

    tft.setCursor(10,10);
    tft.setTextColor(TFT_GREEN);
    tft.print("Temperature: ");
    if (-40 <= Temperature && Temperature <= 0) {
      tft.setTextColor(tft.color565(255,239,213));
    }
    else if (0 <= Temperature && Temperature <= 18) {
      tft.setTextColor(tft.color565(105,105,105));
    }
    else if (18 <= Temperature && Temperature <= 26) {
      tft.setTextColor(TFT_GREEN);
    }
    else if (26 <= Temperature && Temperature <= 35) {
      tft.setTextColor(TFT_ORANGE);
    }
    else if (35 <= Temperature && Temperature <= 125) {
      tft.setTextColor(TFT_RED);
    }
    tft.setCursor(170,10);
    tft.print(Temperature);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(250,10);
    tft.println(" 'C");

    tft.setCursor(10,40);
    tft.setTextColor(TFT_RED);
    tft.print("Humidity: ");
    if (0 <= Humidity && Humidity <= 40) {
      tft.setTextColor(tft.color565(218,165,32));
    }
    else if (40 <= Humidity && Humidity <= 70) {
      tft.setTextColor(TFT_GREEN);
    }
    else if (70 <= Humidity && Humidity <= 100) {
      tft.setTextColor(tft.color565(135,206,250));
    }
    tft.setCursor(170,40);
    tft.print(Humidity);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(250,40);
    tft.println(" %");

    DrawDataCurve();

    delay(100);
    
    tft.setCursor(170,10);
    tft.setTextColor(TFT_BLACK);
    tft.print(Temperature);
    tft.setCursor(170,40);
    tft.setTextColor(TFT_BLACK);
    tft.print(Humidity);

    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if (450 < X_Coord && X_Coord < 480 && 0 < Y_Coord && Y_Coord < 30) {
        break;
      }
    }
  }
}

//File_transfer
void File_transfer(){
  File_transfer_init();
  while(1){
    server.handleClient();

    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if (450 < X_Coord && X_Coord < 480 && 0 < Y_Coord && Y_Coord < 30) {
        break;
      }
    }
  }
}

//Network
void Network(){
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  if (WiFiMode == "AP") {
    tft.print("SSID: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(ssid);
    tft.setTextColor(TFT_WHITE);
    tft.print("AP IP address: ");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.softAPIP());
    tft.setTextColor(TFT_WHITE);

    tft.println("");
    tft.print("Change Mode");

    while (1){
      if (touch.Pressed()) {
        X_Coord = touch.X();
        Y_Coord = touch.Y();
        if (0 < X_Coord && X_Coord < 130 && 80 < Y_Coord && Y_Coord < 110) {
          WiFiMode = "NULL";
          Network();
        }
        else if (450 < X_Coord && X_Coord < 480 && 0 < Y_Coord && Y_Coord < 30) {
          break;
        }
      }
    }
  }
  else if (WiFiMode == "STA") {
    tft.print("Connected to ");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.SSID());
    tft.setTextColor(TFT_WHITE);
    tft.print("IP address:\t");
    tft.setTextColor(TFT_GREEN);
    tft.println(WiFi.localIP());
    tft.setTextColor(TFT_WHITE);

    tft.println("");
    tft.print("Change Mode");

    while (1){
      if (touch.Pressed()) {
        X_Coord = touch.X();
        Y_Coord = touch.Y();
        if (0 < X_Coord && X_Coord < 130 && 80 < Y_Coord && Y_Coord < 110) {
          WiFiMode = "NULL";
          Network();
        }
        else if (450 < X_Coord && X_Coord < 480 && 0 < Y_Coord && Y_Coord < 30) {
          break;
        }
      }
    }
  }
  else if (WiFiMode == "NULL") {
    tft.print("Please select a mode:");
    tft.setCursor(60,130);
    tft.setTextSize(3);
    tft.setTextColor(TFT_ORANGE);
    tft.print("AP       STA");
    tft.setTextSize(1);
    while (1){
      if (touch.Pressed()) {
        X_Coord = touch.X();
        Y_Coord = touch.Y();
        if (60 < X_Coord && X_Coord < 200 && 130 < Y_Coord && Y_Coord < 190) {
          tft.fillScreen(TFT_BLACK);
          WiFiInit("AP");
        }
        else if (240 < X_Coord && X_Coord < 420 && 130 < Y_Coord && Y_Coord < 190) {
          tft.fillScreen(TFT_BLACK);
          WiFiInit("STA");
        }
        else if (450 < X_Coord && X_Coord < 480 && 0 < Y_Coord && Y_Coord < 30) {
          break;
        }
      }
    }
  }
}

//Task Manager
void Task_Manager(){
  drawSdJpeg("/System/APP/Task_Manager/Task_Manager_CPU.jpg", 0, 0);
  tft.setTextColor(TFT_BLACK);
  ViewTask = 1;
  int Drawed1 = 0;
  int Drawed2 = 0;
  int Drawed3 = 0;
  int Drawed4 = 0;
  int Drawed5 = 0;
  int Drawed6 = 0;

  while(1) {
    if(ViewTask == 1) {
      if(Drawed1 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_CPU.jpg", 0, 0);
        Drawed1 = 1;
        Drawed2 = 0;
        Drawed3 = 0;
        Drawed4 = 0;
        Drawed5 = 0;
        Drawed6 = 0;
      }
      tft.setCursor(65,50);
      tft.print("CPU Frequency: ");
      tft.print(ESP.getCpuFreqMHz());
      tft.print(" MHz");
      tft.setCursor(65,80);
      tft.printf("Run time: %ld Minutes", millis() / 60000);
    }
    else if(ViewTask == 2) {
      if(Drawed2 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_RAM.jpg", 0, 0);
        Drawed1 = 0;
        Drawed2 = 1;
        Drawed3 = 0;
        Drawed4 = 0;
        Drawed5 = 0;
        Drawed6 = 0;
      }
      tft.setCursor(65,50);
      tft.printf("Internal RAM total size: %u KB", ESP.getHeapSize() / 1024);

      tft.setCursor(65,80);
      tft.printf("Available heap size: %u KB", ESP.getFreeHeap() / 1024);

      tft.setCursor(65,110);
      tft.printf("SPI RAM: %u KB", ESP.getPsramSize() / 1024);
      
      tft.setCursor(65,140);
      tft.printf("Available heap: %u KB", ESP.getFreePsram() / 1024);
      /*
      IRAM_Usage = printf("%ld",(long)ESP.getHeapSize()) - printf("%ld",(long)ESP.getFreeHeap());
      Serial.println(IRAM_Usage);
      IRAM_Usage = IRAM_Usage / printf("%ld",(long)ESP.getHeapSize());
      Serial.println(IRAM_Usage);
      SRAM_Usage = printf("%ld",(long)ESP.getPsramSize()) - printf("%ld",(long)ESP.getFreePsram());
      SRAM_Usage = SRAM_Usage / printf("%ld",(long)ESP.getPsramSize());
      Serial.println(IRAM_Usage);
      Serial.println(SRAM_Usage);
      */
      tft.fillRect(65,180,200,130,tft.color565(255, 238, 133));
      tft.fillRect(65,180,200 * IRAM_Usage,130,tft.color565(255, 220, 0));

      tft.fillRect(270,180,200,130,tft.color565(255, 238, 133));
      tft.fillRect(270,180,200 * SRAM_Usage,130,tft.color565(255, 220, 0));
    }
    else if(ViewTask == 3) {
      if(Drawed3 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_ROM.jpg", 0, 0);
        Drawed1 = 0;
        Drawed2 = 0;
        Drawed3 = 1;
        Drawed4 = 0;
        Drawed5 = 0;
        Drawed6 = 0;
      }
      tft.setCursor(65,50);
      tft.printf("Flash Size: %u KB", ESP.getFlashChipSize() / 1024);

      tft.setCursor(65,80);
      tft.printf("Speed: %u MHz", ESP.getFlashChipSpeed() / 1000000);

      tft.setCursor(65,110);
      tft.printf("SD.totalSize = %llu GB", SD_MMC.totalBytes() / (1024 * 1024 * 1024));

      tft.setCursor(65,140);
      tft.printf("SD.usedSize = %llu GB", SD_MMC.usedBytes() / (1024 * 1024 * 1024));

      tft.fillRect(65,180,200,130,tft.color565(162, 255, 150));
      tft.fillRect(65,180,200 * IRAM_Usage,130,tft.color565(30, 255, 0));

      tft.fillRect(270,180,200,130,tft.color565(162, 255, 150));
      tft.fillRect(270,180,200 * SRAM_Usage,130,tft.color565(30, 255, 0));
    }
    else if(ViewTask == 4) {
      if(Drawed4 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_Battery.jpg", 0, 0);
        Drawed1 = 0;
        Drawed2 = 0;
        Drawed3 = 0;
        Drawed4 = 1;
        Drawed5 = 0;
        Drawed6 = 0;
      }
      tft.setCursor(65,50);
      tft.print("Battery Voltage: ");
      tft.setTextColor(TFT_WHITE);
      tft.print(BatteryVol);
      tft.setTextColor(TFT_BLACK);
      tft.print(" V");
      ADC_VALUE = analogRead(34);
      //Serial.println(BatteryVol);
      BatteryVol = (ADC_VALUE * 3.3 * 2 ) / (4095);
      //Serial.println(BatteryVol);
      tft.setCursor(65,50);
      tft.print("Battery Voltage: ");
      tft.print(BatteryVol);
      tft.print(" V");
    }
    else if(ViewTask == 5) {
      if(Drawed5 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_WiFi.jpg", 0, 0);
        Drawed1 = 0;
        Drawed2 = 0;
        Drawed3 = 0;
        Drawed4 = 0;
        Drawed5 = 1;
        Drawed6 = 0;
      }
      tft.setCursor(65,50);
      if (WiFiMode == "STA") {
        tft.print("WiFi Mode: STA");
        tft.setCursor(65,80);
        tft.print("Got IP: ");
        tft.print(WiFi.localIP());
        tft.setCursor(65,110);
        tft.print("Station interface MAC address: ");
        tft.setCursor(65,140);
        tft.print(WiFi.macAddress());
        tft.setCursor(65,170);
        tft.print("Connected to: ");
        tft.print(WiFi.SSID());
      }
      else if (WiFiMode == "AP") {
        tft.print("WiFi Mode: AP");
        tft.setCursor(65,80);
        tft.print("Got IP: ");
        tft.print(WiFi.softAPIP());
        tft.setCursor(65,110);
        tft.print("SSID: ");
        tft.print(ssid);
      }
      else {
        tft.print("WiFi Unconnected");
      }
    }
    else if(ViewTask == 6) {
      if(Drawed6 == 0) {
        drawSdJpeg("/System/APP/Task_Manager/Task_Manager_Temperature.jpg", 0, 0);
        Drawed1 = 0;
        Drawed2 = 0;
        Drawed3 = 0;
        Drawed4 = 0;
        Drawed5 = 0;
        Drawed6 = 1;
      }
      tft.setCursor(65,50);
      tft.print("Temperature: ");
      tft.setTextColor(TFT_WHITE);
      tft.print(Temperature);
      tft.print(" 'C");
      Sensor.UpdateData();
      Temperature = Sensor.GetTemperature();
      tft.setCursor(65,50);
      tft.setTextColor(TFT_BLACK);
      tft.print("Temperature: ");
      tft.print(Temperature);
      tft.print(" 'C");
    }
    if(touch.Pressed()) {
      if(0 < touch.X() && touch.X() < 50) {
        if(0 < touch.Y() && touch.Y() < 50) {
          ViewTask = 1;
        }
        if(50 < touch.Y() && touch.Y() < 100) {
          ViewTask = 2;
        }
        else if(100 < touch.Y() && touch.Y() < 150) {
          ViewTask = 3;
        }
        else if(150 < touch.Y() && touch.Y() < 200) {
          ViewTask = 4;
        }
        else if(200 < touch.Y() && touch.Y() < 250) {
          ViewTask = 5;
        }
        else if(250 < touch.Y() && touch.Y() < 300) {
          ViewTask = 6;
        }
      }
      else if(440 < touch.X() && touch.X() < 480 && 0 < touch.Y() && touch.Y() < 40) {
        break;
      }
    }
  }
}

//Desktop
void DrawAPP() {
  tft.fillRect(0, 30, 400, 270, tft.color565(0, 0, 30));
  if (Mainpage == 1) {
    drawSdJpeg("/System/APP/Sounder/Sounder.jpg", 25, 55);
    drawSdJpeg("/System/APP/Vision/Vision.jpg", 115, 55);
    drawSdJpeg("/System/APP/Painter/Painter.jpg", 205, 55);
    drawSdJpeg("/System/APP/Ebook/Ebook.jpg", 295, 55);
    drawSdJpeg("/System/APP/Player/Player.jpg", 25, 145);
    drawSdJpeg("/System/APP/Album/Album.jpg", 115, 145);
    drawSdJpeg("/System/APP/Settings/Settings.jpg", 205, 145);
    drawSdJpeg("/System/APP/Calculator/Calculator.jpg", 295, 145);
    }
  else if (Mainpage == 2) {
    drawSdJpeg("/System/APP/Thermometer/Thermometer.jpg", 25, 55);
    drawSdJpeg("/System/APP/File_transfer/File_transfer.jpg", 115, 55);
    drawSdJpeg("/System/APP/Network/Network.jpg", 205, 55);
    drawSdJpeg("/System/APP/Task_Manager/Task_Manager.jpg", 295, 55);
  }
  tft.fillRoundRect(25,255,80,20,5,TFT_DARKGREY);
  tft.fillRoundRect(295,255,80,20,5,TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40,255);
  tft.print("<<<");
  tft.setCursor(315,255);
  tft.print(">>>");
}
void MainPage() {
  tft.setCursor(0, 300, 4);
  touch.setRotation(1);

  jpgdir = String ("/User/") + User + String("/Data/Others/") + String("Desktop.jpg");
  if (SD_MMC.exists(jpgdir.c_str())){
    drawSdJpeg(jpgdir.c_str(), 0, 0);
  }
  else{
//    drawSdJpeg("/System/Widgets/user.jpg", 40, 40);
    tft.fillScreen(TFT_DARKGREY);
  }
  tft.fillRect(0, 300, 480, 20, tft.color565(0, 0, 0));
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.print("Start");
  tft.setTextColor(TFT_BLACK);

  Serial.print("Pagenum:");
  Serial.println(Mainpage);
  
  while (1){
    if (touch.Pressed()) {
      X_Coord = touch.X();
      Y_Coord = touch.Y();
      if     (X_Coord < 30 && 300 < Y_Coord) {
        DrawAPP();
        while(1){
          if (touch.Pressed()) {
            X_Coord = touch.X();
            Y_Coord = touch.Y();
            if (0 < X_Coord && X_Coord < 450 && 30 < Y_Coord && Y_Coord < 320) {
              if (25 < X_Coord && X_Coord < 105 && 255 < Y_Coord && Y_Coord < 275) {
                Mainpage--;
                if (Mainpage < 1) {
                  Mainpage = 1;
                }
                DrawAPP();
              }
              else if (295 < X_Coord && X_Coord < 375 && 255 < Y_Coord && Y_Coord < 275) {
                Mainpage++;
                if (Mainpage > 2) {
                  Mainpage = 2;
                }
                DrawAPP();
              }
              else if (Mainpage == 1) {
                if (25 < X_Coord && X_Coord < 105 && 55 < Y_Coord && Y_Coord < 135) {
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Music();
                }
                else if (115 < X_Coord && X_Coord < 195 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Video();
                }
                else if (205 < X_Coord && X_Coord < 285 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  draw();
                }
                else if (295 < X_Coord && X_Coord < 375 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Ebook();
                }
                else if (25 < X_Coord && X_Coord < 105 && 145 < Y_Coord && Y_Coord < 225){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  THMini();
                }
                else if (115 < X_Coord && X_Coord < 195 && 145 < Y_Coord && Y_Coord < 225){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Album();
                }
                else if (205 < X_Coord && X_Coord < 285 && 145 < Y_Coord && Y_Coord < 225){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Settings();
                }
                else if (295 < X_Coord && X_Coord < 375 && 145 < Y_Coord && Y_Coord < 225){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Calculator();
                }
              }
              else if (Mainpage == 2) {
                if (25 < X_Coord && X_Coord < 105 && 55 < Y_Coord && Y_Coord < 135) {
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Thermometer();
                }
                else if (115 < X_Coord && X_Coord < 195 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  File_transfer();
                }
                else if (205 < X_Coord && X_Coord < 285 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Network();
                }
                else if (295 < X_Coord && X_Coord < 375 && 55 < Y_Coord && Y_Coord < 135){
                  tft.fillScreen(TFT_BLACK);
                  delay(100);
                  Task_Manager();
                }
              }
            }
            else {
              MainPage();
            }
          }
        }
      }
    }
  }
}