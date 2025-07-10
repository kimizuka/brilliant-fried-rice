#include <M5Core2.h>
#include <driver/i2s.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <Adafruit_NeoPixel.h>

AudioFileSourceSD *file;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;

#define BCLK_PIN 12
#define LRCK_PIN 0
#define SADTA_PIN 2
#define EXTERNAL_I2S 0
#define OUTPUT_GAIN 100

bool isPlay = false;
bool isOn = false;

#define LEDPIN 32
#define NUMPIXELS 72
#define DELAY 10

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int ledIndex = 0;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

ButtonColors offColor = {BLACK, WHITE, WHITE};
ButtonColors onColor  = {BLACK, RED, RED};

int buttonWidth = 160;
int buttonHeight = 90;

Button button(
  (320 - buttonWidth) / 2,
  (240 - buttonHeight) / 2,
  buttonWidth,
  buttonHeight,
  false,
  "ON",
  offColor,
  onColor
);

void setup() {
  pixels.begin();
  M5.begin();
  M5.IMU.Init();
  M5.Axp.SetSpkEnable(true); 
  button.addHandler(handleButtonRelease, E_RELEASE);
  M5.Buttons.draw();
  playMp3();
}

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

void loop() {
  M5.update();
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  pixels.clear();

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
      if (!isPlay) {
        playMp3();
      }

      for (int i = 0; i < 16; i++) {
        pixels.setPixelColor((ledIndex + i * 2) % NUMPIXELS, pixels.Color(100, 100, 100));
      }

      pixels.show();
    } else {
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

void handleButtonRelease(Event & e) {
  isOn = !isOn;
  button.setLabel(isOn ? "OFF" : "ON");
  M5.Buttons.draw();
}