#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHT_PIN 15
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "QFFVQ0KN51FUHFP7";

unsigned long previousThingSpeakMillis = 0;
const unsigned long thingSpeakInterval = 15000; // 15s

float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(1000);

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

  // ===== Read DHT11 sensor continuously =====
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    temperature = t;
    humidity = h;

    Serial.print("🌡 Temp: ");
    Serial.print(temperature);
    Serial.print(" °C | 💧 Humidity: ");
    Serial.println(humidity);
  } else {
    Serial.println("❌ DHT11 read error");
  }

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(temperature) +
                   "&field2=" + String(humidity);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
