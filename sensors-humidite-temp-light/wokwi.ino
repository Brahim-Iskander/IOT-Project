#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define PIR_PIN 5
#define LDR_PIN 12
#define LED_PIN 2
#define DHT_PIN 15
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long previousMillis = 0;
const unsigned long interval = 2000; // lecture toutes les 2s

/*
const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "QFFVQ0KN51FUHFP7";
*/

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12);
  dht.begin();

  /*
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  */
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // PIR
    int motion = digitalRead(PIR_PIN);
    Serial.println(motion ? "🚶 Motion detected" : "No motion");

    // LDR
    int ldrValue = analogRead(LDR_PIN);
    Serial.print("💡 LDR: ");
    Serial.println(ldrValue);
    digitalWrite(LED_PIN, ldrValue < 1500 ? HIGH : LOW);

    // DHT
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("❌ DHT read error");
    } else {
      Serial.print("🌡 Temp: ");
      Serial.print(temperature);
      Serial.print(" °C | 💧 Humidity: ");
      Serial.println(humidity);
    }

    /*
    // ===== SEND TO THINGSPEAK =====
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(ldrValue) +
                   "&field2=" + String(temperature) +
                   "&field3=" + String(humidity) +
                   "&field4=" + String(motion);
      http.begin(url);
      int httpCode = http.GET();
      http.end();
      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
    */

    Serial.println("--------------------------------");
  }
}
