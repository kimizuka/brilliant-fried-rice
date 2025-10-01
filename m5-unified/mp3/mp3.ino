#include <M5Unified.h>
#include <SD.h>
#include <SPI.h>
#include <driver/i2s.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

#define BCLK_PIN 12
#define LRCK_PIN 0
#define SADTA_PIN 2

#define SD_SCLK    18
#define SD_MISO    19
#define SD_MOSI    23
#define SD_CS       4

#define EXTERNAL_I2S 0
#define OUTPUT_GAIN 10
#define DELAY 10

AudioFileSourceSD *file;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;

bool isPlay = false;

void playMp3() {
  if (!isPlay) {
    file = new AudioFileSourceSD("/bgm.mp3");
    out = new AudioOutputI2S(I2S_NUM_0, EXTERNAL_I2S); 
    out -> SetPinout(BCLK_PIN, LRCK_PIN, SADTA_PIN);
    out -> SetOutputModeMono(true);
    out -> SetGain((float)OUTPUT_GAIN / 100.0);
    mp3 = new AudioGeneratorMP3();
    mp3 -> begin(file, out);

    isPlay = true;
  }
}

void setup() {
  M5.begin();
  M5.Speaker.begin(); 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(4);

  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI, 25000000)) {
    M5.Display.setCursor(0, 0);
    M5.Display.println("SD mount failed");

    while (true) {
      delay(1000);
    }
  }

  playMp3();
}

void loop() {
  M5.update();

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println(isPlay);

  if (mp3 -> isRunning()) {
    isPlay = true;
    if (!mp3 -> loop()) {
      mp3 -> stop();
      isPlay = false;
    }
  }

  if (!isPlay) {
    playMp3();
  }

  delay(DELAY);
}
