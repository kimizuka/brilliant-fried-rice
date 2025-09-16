#include <M5Unified.h>
#include <ArduinoOSCWiFi.h>

const char* ssid = "ExPLOT_Studio";
const char* password = "0456632812";
const char *ip = "192.168.0.198";

void setup() {
  M5.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print('.');
  }

  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextSize(2);
}

void loop() {
  M5.update();
  M5.Display.setCursor(0, 0);
  M5.Display.print("IP: ");
  M5.Display.println(WiFi.localIP());

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();

    if (t.wasReleased()) {
      OscWiFi.send(ip, 9000, "/send", 1);
      M5.Display.println("hello");
    }
  }
}

void post() {

}