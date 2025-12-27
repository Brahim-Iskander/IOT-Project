#include <WiFi.h>
#include <HTTPClient.h>

#define LDR_PIN 12
#define LED_PIN 2

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "QFFVQ0KN51FUHFP7";

unsigned long previousThingSpeakMillis = 0;
const unsigned long thingSpeakInterval = 15000; // 15s

int ldrValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");
}

void loop() {
  unsigned long currentMillis = millis();

  // ===== Read LDR continuously =====
  ldrValue = analogRead(LDR_PIN);
  Serial.print("💡 LDR: ");
  Serial.println(ldrValue);

  digitalWrite(LED_PIN, ldrValue < 1500 ? HIGH : LOW);

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(ldrValue);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
