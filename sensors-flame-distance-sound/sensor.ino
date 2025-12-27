#include <WiFi.h>
#include <HTTPClient.h>

// ===== WIFI =====
const char* ssid = "iPhone";
const char* password = "12345678901";
String apiKey = "ZY59RYO41TC9A23L";

// ===== FLAME SENSOR =====
#define FLAME_PIN 6
#define LED_PIN 7

// ===== ULTRASONIC 3 PIN =====
#define ULTRA_PIN 10  // Pin SIG unique

// ===== ANALOG SOUND SENSOR =====
#define SOUND_AO 4
const int SOUND_THRESHOLD = 2000;

// Variables partagées
volatile int flame = 0;
volatile float distance = 0;
volatile int soundLevel = 0;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ULTRA_PIN, INPUT);

  analogReadResolution(12);

  // ===== WIFI =====
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // ===== CREATE TASKS =====
  xTaskCreate(flameTask, "Flame Task", 2048, NULL, 1, NULL);
  xTaskCreate(ultraTask, "Ultra Task", 4096, NULL, 1, NULL);
  xTaskCreate(soundTask, "Sound Task", 2048, NULL, 1, NULL);
  xTaskCreate(thingSpeakTask, "ThingSpeak Task", 4096, NULL, 1, NULL);
}

void loop() {
  // Loop vide, tout est géré par les tâches
  delay(1000);
}

// ===== TASK FLAME =====
void flameTask(void *pvParameters) {
  for (;;) {
    flame = digitalRead(FLAME_PIN);
    Serial.print("Flame: ");
    Serial.println((flame == LOW) ? "Detected" : "None");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1s
  }
}

// ===== TASK ULTRASONIC =====
void ultraTask(void *pvParameters) {
  for (;;) {
    pinMode(ULTRA_PIN, OUTPUT);
    digitalWrite(ULTRA_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRA_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRA_PIN, LOW);

    pinMode(ULTRA_PIN, INPUT);
    long duration = pulseIn(ULTRA_PIN, HIGH, 30000);
    distance = duration * 0.034 / 2;

    Serial.print("📏 Distance: ");
    Serial.println(distance);

    if (distance > 0 && distance < 10) {
      Serial.println(" Object very close!");
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS); // 0.5s
  }
}

// ===== TASK SOUND =====
void soundTask(void *pvParameters) {
  for (;;) {
    soundLevel = analogRead(SOUND_AO);
    Serial.print("🎵 Sound level: ");
    Serial.println(soundLevel);
    bool soundDetected = soundLevel > SOUND_THRESHOLD;
    Serial.println(soundDetected ? " Sound detected!" : "Silence");
    vTaskDelay(500 / portTICK_PERIOD_MS); // 0.5s
  }
}

// ===== TASK THINGSPEAK =====
void thingSpeakTask(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(flame == LOW ? 1 : 0) +
                   "&field2=" + String(distance) +
                   "&field3=" + String(soundLevel);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      if (httpCode > 0) Serial.println("Data sent to ThingSpeak");
      else Serial.println("Failed to send data");
    }
    Serial.println("-------------------------");
    vTaskDelay(15000 / portTICK_PERIOD_MS); // 15s
  }
}
