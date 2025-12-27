#include <WiFi.h>
#include <HTTPClient.h>

#define FLAME_PIN 6
#define LED_PIN 7

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";

unsigned long previousMillis = 0;
const unsigned long thingSpeakInterval = 15000; // 15s

int flameState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  unsigned long currentMillis = millis();

  // ===== Read flame sensor =====
  flameState = digitalRead(FLAME_PIN);
  Serial.print("Flame: ");
  Serial.println(flameState == LOW ? "Detected" : "None");

  digitalWrite(LED_PIN, flameState == LOW ? HIGH : LOW);

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousMillis >= thingSpeakInterval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(flameState == LOW ? 1 : 0);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
