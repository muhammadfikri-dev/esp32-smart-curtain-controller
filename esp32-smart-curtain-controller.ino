#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define PIN_STEP 18
#define PIN_DIR 19
#define PIN_LDR 34

AsyncWebServer server(80);
int currentPosition = 0; // 0: Closed, 100: Open

void moveCurtain(int target) {
  int steps = (target - currentPosition) * 100;
  digitalWrite(PIN_DIR, steps > 0 ? HIGH : LOW);
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(800);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(800);
  }
  currentPosition = target;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  WiFi.begin("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/open", HTTP_GET, [](AsyncWebServerRequest *req){ moveCurtain(100); req->send(200, "text/plain", "Curtain Opened"); });
  server.on("/close", HTTP_GET, [](AsyncWebServerRequest *req){ moveCurtain(0); req->send(200, "text/plain", "Curtain Closed"); });
  server.begin();
}

void loop() {
  int lightVal = analogRead(PIN_LDR);
  // Auto open at sunrise, close at sunset
  delay(2000);
}