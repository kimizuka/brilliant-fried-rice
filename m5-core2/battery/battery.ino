# include <M5Core2.h>
# include <Wire.h>
# include <AXP192.h>

AXP192 power;

void setup(){
  M5.begin();
}

void loop(){
  M5.Lcd.printf("%3.4f V", power.GetBatVoltage());
}