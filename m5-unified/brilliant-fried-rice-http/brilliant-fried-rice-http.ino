#include <M5Unified.h>
#include <SD.h>
#include <SPI.h>
#include <driver/i2s.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define BCLK_PIN 12
#define LRCK_PIN 0
#define SADTA_PIN 2

#define SD_SCLK    18
#define SD_MISO    19
#define SD_MOSI    23
#define SD_CS       4

#define EXTERNAL_I2S 0
#define OUTPUT_GAIN 100

#define LEDPIN 32
#define NUMPIXELS 72
#define DELAY 10

AudioFileSourceSD *file;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;

bool isPlay = false;
bool isOn = false;

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int ledIndex = 0;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char *api = "API_ENDPOINT";
const char *closeApi = "CLOSE_API_ENDPOINT";
const char *openApi = "OPEN_API_ENDPOINT";
const char *ip = "IP_ADDRESS";

String lastState = "close"; // close or open

LGFX_Sprite sprite = LGFX_Sprite(&M5.Lcd);

void playMp3() {
  if (!isPlay) {
    isPlay = true;
    file = new AudioFileSourceSD("/bgm.mp3");
    out = new AudioOutputI2S(I2S_NUM_0, EXTERNAL_I2S); 
    out -> SetPinout(BCLK_PIN, LRCK_PIN, SADTA_PIN);
    out -> SetOutputModeMono(true);
    out -> SetGain((float)OUTPUT_GAIN / 100.0);
    mp3 = new AudioGeneratorMP3();
    mp3 -> begin(file, out);
  }
}

void setup() {
  pixels.begin();
  M5.begin();
  M5.Speaker.begin(); 
  M5.Imu.begin();

  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);

  if (!SD.begin(SD_CS, SPI, 25000000)) {
    M5.Display.setCursor(0, 0);
    M5.Display.println("SD mount failed");

    while (true) {
      delay(1000);
    }
  } else {
    sprite.setColorDepth(16);
    sprite.createSprite(240, 80);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print('.');
  }
  M5.Display.println("\nWiFi connected.\n");

  playMp3();
}

void loop() {
  M5.update();
  M5.Imu.getAccelData(&accX, &accY, &accZ);
  pixels.clear();

  int batteryLevel = M5.Power.getBatteryLevel();

  sprite.fillSprite(TFT_BLACK);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setTextSize(2);
  sprite.setCursor(0, 0);
  sprite.print("Battery: ");
  sprite.print(batteryLevel);
  sprite.println("%");
  sprite.print("Status: ");

  if (isOn) {
    sprite.println("ON");
  } else {
    sprite.println("OFF");
  }

  sprite.print("IP: ");
  sprite.println(WiFi.localIP());

  sprite.pushSprite(0, 0);

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();

    if (t.wasReleased()) {
      handleButtonRelease();
    }
  }

  if (isOn) {
    if (mp3 -> isRunning()) {
      isPlay = true;

      if (!mp3 -> loop()) {
        mp3 -> stop();
        isPlay = false;
      }
    } else {
      isPlay = false;
      mp3 -> stop();
    }

    if (-0.8F < accZ) {
      if (lastState == "close") {
        lastState = "open";
        doOpen();
      }

      if (!isPlay) {
        playMp3();
      }

      for (int i = 0; i < 16; i++) {
        pixels.setPixelColor((ledIndex + i * 2) % NUMPIXELS, pixels.Color(100, 100, 100));
      }

      pixels.show();
    } else {
      if (lastState == "open") {
        lastState = "close";
        doClose();
      }

      if (isPlay) {
        isPlay = false;
        mp3 -> stop();
      }

      for (int i = 0; i < NUMPIXELS; i++) { 
        pixels.clear();
        pixels.show();
      }
    }

    ledIndex = (ledIndex + 1) % NUMPIXELS;
  } else {
    isPlay = false;
    mp3 -> stop();

    for (int i = 0; i < NUMPIXELS; i++) { 
      pixels.clear();
      pixels.show();
    }

    delay(100);
  }
}

void handleButtonRelease() {
  isOn = !isOn;
}

void post() {
  HTTPClient http;
  http.begin(api);
  http.addHeader("Content-Type", "application/json");

  int status = http.POST("{}");

  String body = http.getString();
  M5.Display.printf("HTTP %d\n%s\n", status, body.c_str());

  http.end();
}

void doOpen() {
  HTTPClient http;
  http.begin(openApi);
  http.addHeader("Content-Type", "application/json");

  int status = http.POST("{}");

  String body = http.getString();
  M5.Display.printf("HTTP %d\n%s\n", status, body.c_str());

  http.end();
}

void doClose() {
  HTTPClient http;
  http.begin(closeApi);
  http.addHeader("Content-Type", "application/json");

  int status = http.POST("{}");

  String body = http.getString();

  http.end();
}