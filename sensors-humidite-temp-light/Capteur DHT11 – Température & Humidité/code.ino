#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHT_PIN 15
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "QFFVQ0KN51FUHFP7";

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
  Serial.println("\n WiFi Connected");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(" DHT11 read error");
    delay(2000);
    return;
  }

  Serial.print(" Temp: ");
  Serial.print(temperature);
  Serial.print(" °C |  Humidity: ");
  Serial.println(humidity);

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity);
    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println(httpCode > 0 ? "📤Data sent to ThingSpeak" : "Failed to send");
  }

  delay(15000);
}
