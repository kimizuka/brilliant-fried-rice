#include <M5Core2.h>

float accelerationX = 0.0F;
float accelerationY = 0.0F;
float accelerationZ = 0.0F;

int DELAY = 100;

void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(4);
}

void loop() {
  M5.update();
  M5.IMU.getAccelData(&accelerationX, &accelerationY, &accelerationZ);

  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("X: %.2f", accelerationX);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.printf("Y: %.2f", accelerationY);
  M5.Lcd.setCursor(10, 90);
  M5.Lcd.printf("Z: %.2f", accelerationZ);

  delay(DELAY);
}