#include <WiFi.h>
#include <HTTPClient.h>

#define PIR_PIN 5
#define LED_PIN 2

// WiFi credentials
const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";

// ThingSpeak API key
String apiKey = "QFFVQ0KN51FUHFP7";

unsigned long previousThingSpeakMillis = 0;
const unsigned long thingSpeakInterval = 15000; // 15s

int motion = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

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

  // ===== Read PIR sensor continuously =====
  motion = digitalRead(PIR_PIN);
  Serial.println(motion ? "🚶 Motion detected" : "No motion");
  digitalWrite(LED_PIN, motion ? HIGH : LOW);

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(motion); // 1 = motion, 0 = no motion
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
