#include <M5Unified.h>

int count = 0;

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(4);
}

void loop() {
  M5.update();
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("%d", count);

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();

    if (t.wasReleased()) {
      handleButtonRelease();
    }
  }
}

void handleButtonRelease() {
  count += 1;
}