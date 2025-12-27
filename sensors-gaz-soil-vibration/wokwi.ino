#include <WiFi.h>
#include <HTTPClient.h>

#define GAS_DIGITAL 2
#define SOIL_PIN 13
#define VIB_SENSOR 4

/*
const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";
*/

unsigned long previousMillis = 0;
const unsigned long interval = 2000; // lecture toutes les 2s

void setup() {
  Serial.begin(115200);
  pinMode(GAS_DIGITAL, INPUT);
  pinMode(VIB_SENSOR, INPUT_PULLUP);
  analogReadResolution(12);
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

    int gasState = digitalRead(GAS_DIGITAL);
    Serial.print("Gas detected: ");
    Serial.println(gasState ? "YES" : "NO");

    int soilValue = analogRead(SOIL_PIN);
    Serial.print("Soil moisture: ");
    Serial.println(soilValue);
    if (soilValue < 1500) Serial.println("Sol sec !");

    int vibState = !digitalRead(VIB_SENSOR);
    Serial.print("📳 Vibration detected: ");
    Serial.println(vibState ? "YES" : "NO");

    /*
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(gasState) +
                   "&field2=" + String(soilValue) +
                   "&field3=" + String(vibState);
      http.begin(url);
      int httpCode = http.GET();
      http.end();
      Serial.println(httpCode > 0 ? "Data sent to ThingSpeak" : "Failed to send");
    }
    */

    Serial.println("-------------------------");
  }
}
