#include <WiFi.h>
#include <HTTPClient.h>
/*
// ===== WIFI =====
const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "ZY59RYO41TC9A23L";
*/
// ===== FLAME SENSOR =====
#define FLAME_PIN 6
#define LED_PIN 7

// ===== ULTRASONIC SENSOR (4 PINS) =====
#define TRIG_PIN 9
#define ECHO_PIN 10

// ===== SOUND SENSOR =====
#define SOUND_AO 4
const int SOUND_THRESHOLD = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  analogReadResolution(12);
/*
  // ===== WIFI =====
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");*/
}

void loop() {

  // ===== FLAME SENSOR =====
  int flame = digitalRead(FLAME_PIN);
  Serial.print("🔥 Flame: ");
  Serial.println(flame == LOW ? "Detected" : "None");

  // ===== ULTRASONIC SENSOR =====
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  Serial.print("📏 Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance < 10) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("🚨 Object very close!");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // ===== SOUND SENSOR =====
  int soundLevel = analogRead(SOUND_AO);
  Serial.print("🎵 Sound level: ");
  Serial.println(soundLevel);

  bool soundDetected = soundLevel > SOUND_THRESHOLD;
  Serial.println(soundDetected ? "🔊 Sound detected!" : "🔇 Silence");
/*
  // ===== SEND TO THINGSPEAK =====
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(flame == LOW ? 1 : 0) +
                 "&field2=" + String(distance) +
                 "&field3=" + String(soundLevel);

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    if (httpCode > 0)
      Serial.println("📤 Data sent to ThingSpeak");
    else
      Serial.println("❌ Failed to send data");
  }
*/
  Serial.println("-------------------------");
  delay(2000);  // obligatoire pour ThingSpeak
}
