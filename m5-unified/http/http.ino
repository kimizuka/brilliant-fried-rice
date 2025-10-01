#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char *api = "API_ENDPOINT";

void setup() {
  M5.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print('.');
  }
  M5.Display.println("\nWiFi connected.\n");

  M5.Display.print("Local IP: ");
  M5.Display.println(WiFi.localIP());

  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextSize(1);
}

void loop() {
  M5.update();

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();

    if (t.wasReleased()) {
      post();
    }
  }
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