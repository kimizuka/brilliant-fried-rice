#include <M5Core2.h>

ButtonColors offColor = {BLACK, WHITE, WHITE};
ButtonColors onColor  = {BLACK, RED, RED};

int count = 0;
char buffer[10];

Button button(80, 65, 160, 90, false, "", offColor, onColor);

void setup() {
  M5.begin();
  button.addHandler(handleButtonTouch, E_TOUCH);
  button.addHandler(handleButtonRelease, E_RELEASE);
  sprintf(buffer, "%d", count);
  button.setLabel(buffer);
  M5.Buttons.draw();
}

void loop() {
  M5.update();
}

void handleButtonTouch(Event & e) {
  count += 1;
}

void handleButtonRelease(Event & e) {
  sprintf(buffer, "%d", count);
  button.setLabel(buffer);
  M5.Buttons.draw();
}