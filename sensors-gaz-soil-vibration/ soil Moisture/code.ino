#include <WiFi.h>
#include <HTTPClient.h>

#define SOIL_PIN 13     // ADC pin

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";

void setup() {
  Serial.begin(115200);
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
  int soilValue = analogRead(SOIL_PIN);
  Serial.print(" Soil moisture: ");
  Serial.println(soilValue);
  if (soilValue < 1500) Serial.println(" Soil is dry!");

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(soilValue);
    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println(httpCode > 0 ? "Data sent to ThingSpeak" : "Failed to send data");
  }

  delay(15000);
}
