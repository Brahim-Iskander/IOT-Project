#include <WiFi.h>
#include <HTTPClient.h>

#define FLAME_PIN 6
#define LED_PIN 7
#define TRIG_PIN 9
#define ECHO_PIN 10
#define SOUND_AO 4
const int SOUND_THRESHOLD = 2000;

// WiFi / ThingSpeak
/*
const char* ssid = "TON_SSID_2.4GHz";
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";
*/

unsigned long previousMillis = 0;
const unsigned long interval = 2000; // lecture toutes les 2s

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  analogReadResolution(12);

  /*
  // WiFi
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

    int flame = digitalRead(FLAME_PIN);
    Serial.print("Flame: ");
    Serial.println(flame == LOW ? "Detected" : "None");

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    float distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance > 0 && distance < 10) digitalWrite(LED_PIN, HIGH);
    else digitalWrite(LED_PIN, LOW);

    int soundLevel = analogRead(SOUND_AO);
    Serial.print("Sound level: ");
    Serial.println(soundLevel);
    Serial.println(soundLevel > SOUND_THRESHOLD ? "Sound detected!" : "Silence");

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
      Serial.println(httpCode > 0 ? "Data sent to ThingSpeak" : "Failed to send");
    }
    */

    Serial.println("-------------------------");
  }
}
