#include <WiFi.h>
#include <HTTPClient.h>
/*
// ===== WIFI =====
const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";
*/
// ===== GAS SENSOR =====
#define GAS_DIGITAL 2   // Connect the sensor's DO pin to GPIO2

// ===== SOIL MOISTURE =====
#define SOIL_PIN 13     // ADC pin

// ===== VIBRATION SENSOR =====
#define VIB_SENSOR 4    // Connect vibration sensor DO to GPIO4

void setup() {
  Serial.begin(115200);

  pinMode(GAS_DIGITAL, INPUT); // Digital input for gas sensor
  pinMode(VIB_SENSOR, INPUT_PULLUP);  // Digital input for vibration sensor
  analogReadResolution(12);    // For soil moisture
/*
  // ===== WIFI CONNECT =====
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");*/
}

void loop() {
  // ----- GAS SENSOR (digital) -----
  int gasState = digitalRead(GAS_DIGITAL);
  Serial.print("Gas detected: ");
  Serial.println(gasState ? "YES" : "NO");

  // ----- SOIL MOISTURE -----
  int soilValue = analogRead(SOIL_PIN);
  Serial.print("Soil moisture: ");
  Serial.println(soilValue);

  if (soilValue < 1500) {   // adjust threshold
    Serial.println("Sol sec !");
  }

  // ----- VIBRATION SENSOR (digital) -----
  int vibState = !digitalRead(VIB_SENSOR);
  Serial.print("📳 Vibration detected: ");
  Serial.println(vibState ? "YES" : "NO");
/*
  // ===== SEND TO THINGSPEAK =====
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(gasState) +
                 "&field2=" + String(soilValue) +
                 "&field3=" + String(vibState);  // Added vibration

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    if (httpCode > 0) Serial.println(" Data sent to ThingSpeak");
    else Serial.println("Failed to send data");
  }
*/
  Serial.println("-------------------------");
  delay(2000); // ThingSpeak requires minimum 15s
}
