#include <M5Unified.h>
#include <ArduinoOSCWiFi.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char *api = "API_ENDPOINT";
const char *ip = "IP_ADDRESS";
const int port = 10000;

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
      OscWiFi.send(ip, port, "/send", 1);
    }
  }
}

void post() {

}