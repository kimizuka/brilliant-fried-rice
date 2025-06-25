#include <M5Core2.h>
#include <Adafruit_NeoPixel.h>

#define LEDPIN 32
#define PIXELS_LENGTH 72
#define DELAY 100

Adafruit_NeoPixel pixels(PIXELS_LENGTH, LEDPIN, NEO_GRB + NEO_KHZ800);

int shiftIndex = 0;

void setup() {
  pixels.begin();
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(16);
}

void loop() {
  pixels.clear();
  M5.update();

  for (int i = 0; i < PIXELS_LENGTH / 2; i++) { 
    int index = i * 2 + shiftIndex;

    pixels.setPixelColor(index, pixels.Color(255, 255, 255));
    pixels.show();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("%d", index);
  }

  if (shiftIndex == 0) {
    shiftIndex = 1;
  } else {
    shiftIndex = 0;
  }

  delay(DELAY);
}