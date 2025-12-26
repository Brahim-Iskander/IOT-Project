#include <WiFi.h>
#include <HTTPClient.h>

#define LDR_PIN 12
#define LED_PIN 2

const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "QFFVQ0KN51FUHFP7";

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
  Serial.println("\nWiFi Connected");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR: ");
  Serial.println(ldrValue);

  if (ldrValue < 1500) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(ldrValue);
    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println(httpCode > 0 ? " Data sent to ThingSpeak" : "Failed to send");
  }

  delay(15000);
}
