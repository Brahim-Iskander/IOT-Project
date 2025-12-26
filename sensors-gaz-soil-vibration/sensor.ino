#include <WiFi.h>
#include <HTTPClient.h>

// ===== WIFI =====
const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "N382YP4YE0TKD7JR";

// ===== GAS SENSOR =====
#define GAS_DIGITAL 2   // Connect gas sensor DO to GPIO2

// ===== SOIL MOISTURE =====
#define SOIL_PIN 13     // ADC pin

// ===== VIBRATION SENSOR (DFR0027) =====
#define VIB_SENSOR 4    // Connect vibration sensor DO to GPIO4

void setup() {
  Serial.begin(115200);

  // Configure pins
  pinMode(GAS_DIGITAL, INPUT);            // Gas sensor digital input
  pinMode(VIB_SENSOR, INPUT_PULLUP);      // Vibration sensor with internal pull-up
  analogReadResolution(12);               // For soil moisture (0–4095)

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");
}

void loop() {
  // ----- GAS SENSOR (digital) -----
  int gasState = digitalRead(GAS_DIGITAL);
  Serial.print("🧪 Gas detected: ");
  Serial.println(gasState ? "YES" : "NO");

  // ----- SOIL MOISTURE (analog) -----
  int soilValue = analogRead(SOIL_PIN);
  Serial.print("🌱 Soil moisture: ");
  Serial.println(soilValue);
  if (soilValue < 1500) {   // Adjust threshold
    Serial.println("⚠️ Soil is dry!");
  }

  // ----- VIBRATION SENSOR (digital) -----
  int vibState = !digitalRead(VIB_SENSOR); // Invert logic: 1 = vibration detected
  Serial.print("📳 Vibration detected: ");
  Serial.println(vibState ? "YES" : "NO");

  // ===== SEND DATA TO THINGSPEAK =====
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(gasState) +
                 "&field2=" + String(soilValue) +
                 "&field3=" + String(vibState);

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    if (httpCode > 0) Serial.println("📤 Data sent to ThingSpeak");
    else Serial.println("❌ Failed to send data");
  }

  Serial.println("-------------------------");
  delay(15000); // ThingSpeak requires at least 15s between updates
}