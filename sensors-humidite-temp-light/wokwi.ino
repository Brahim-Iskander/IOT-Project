#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
/*
// ===== WIFI =====
const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "QFFVQ0KN51FUHFP7";
*/
// ===== PINS =====
#define PIR_PIN 5
#define LDR_PIN 12
#define LED_PIN 2
#define DHT_PIN 15
#define DHT_TYPE DHT22   // ✅ CHANGED HERE

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12);

  dht.begin();

  /*WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");*/
}

void loop() {

  // ===== PIR =====
  int motion = digitalRead(PIR_PIN);
  if (motion == HIGH)
    Serial.println("🚶 Motion detected");
  else
    Serial.println("No motion");

  // ===== LDR =====
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("💡 LDR: ");
  Serial.println(ldrValue);

  if (ldrValue < 1500)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);

  // ===== DHT11 =====
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ DHT11 read error");
    return;
  }

  Serial.print("🌡 Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | 💧 Humidity: ");
  Serial.println(humidity);
/*
  // ===== SEND TO THINGSPEAK =====
if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(ldrValue) +
                 "&field2=" + String(temperature) +
                 "&field3=" + String(humidity) +
                 "&field4=" + String(motion);   // 👈 motion ajouté ici

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    if (httpCode > 0)
      Serial.println("📤 Data sent to ThingSpeak");
    else
      Serial.println("❌ Failed to send data");
}
*/

  Serial.println("--------------------------------");

  // ⚠️ DHT11 needs slow reading
  delay(2000);
}