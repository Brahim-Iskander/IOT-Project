#include <WiFi.h>
#include <HTTPClient.h>

#define ULTRA_PIN 10
#define LED_PIN 7

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "ZY59RYO41TC9A23L";

unsigned long previousSensorMillis = 0;
unsigned long previousThingSpeakMillis = 0;
const unsigned long sensorInterval = 500;      // 0.5 s
const unsigned long thingSpeakInterval = 15000; // 15 s

float distance = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ULTRA_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

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

  // ===== Measure sensor every 0.5s =====
  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;

    // Trigger ultrason
    pinMode(ULTRA_PIN, OUTPUT);
    digitalWrite(ULTRA_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRA_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRA_PIN, LOW);

    // Echo
    pinMode(ULTRA_PIN, INPUT);
    long duration = pulseIn(ULTRA_PIN, HIGH, 30000);
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);

    // LED control
    digitalWrite(LED_PIN, (distance > 0 && distance < 10) ? HIGH : LOW);
  }

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(distance);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
