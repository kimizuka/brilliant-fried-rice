#include <M5Unified.h>

void setup() {
  M5.begin();
}

void loop() {
  int batteryLevel = M5.Power.getBatteryLevel();
  
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Battery Level: ");
  M5.Lcd.print(batteryLevel);
  M5.Lcd.println("%");
  delay(1000);
}