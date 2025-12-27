#include <WiFi.h>
#include <HTTPClient.h>

#define VIB_SENSOR 4    // Connect vibration sensor DO to GPIO4

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";

unsigned long previousThingSpeakMillis = 0;
const unsigned long thingSpeakInterval = 15000; // 15s

int vibState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(VIB_SENSOR, INPUT_PULLUP);

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

  // ===== Read vibration sensor continuously =====
  vibState = !digitalRead(VIB_SENSOR); // 1 = vibration detected
  Serial.print("📳 Vibration detected: ");
  Serial.println(vibState ? "YES" : "NO");

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(vibState);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}

