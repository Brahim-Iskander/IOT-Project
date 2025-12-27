#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// ===== WIFI =====
const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "QFFVQ0KN51FUHFP7";

// ===== PINS =====
#define PIR_PIN 5
#define LDR_PIN 12
#define LED_PIN 2
#define DHT_PIN 15
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// Variables partagées
volatile int motion = 0;
volatile int ldrValue = 0;
volatile float temperature = 0;
volatile float humidity = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12);

  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  // Create tasks
  xTaskCreate(pirTask, "PIR Task", 2048, NULL, 1, NULL);
  xTaskCreate(ldrTask, "LDR Task", 2048, NULL, 1, NULL);
  xTaskCreate(dhtTask, "DHT Task", 4096, NULL, 1, NULL);
  xTaskCreate(thingSpeakTask, "ThingSpeak Task", 4096, NULL, 1, NULL);
}

void loop() {
  // Loop vide, tout est géré par les tâches
  delay(1000);
}

// ===== PIR TASK =====
void pirTask(void *pvParameters) {
  for (;;) {
    motion = digitalRead(PIR_PIN);
    Serial.println(motion ? "🚶 Motion detected" : "No motion");
    vTaskDelay(500 / portTICK_PERIOD_MS); // Lire toutes les 0,5s
  }
}

// ===== LDR TASK =====
void ldrTask(void *pvParameters) {
  for (;;) {
    ldrValue = analogRead(LDR_PIN);
    Serial.print("💡 LDR: ");
    Serial.println(ldrValue);
    digitalWrite(LED_PIN, ldrValue < 1500 ? HIGH : LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ===== DHT TASK =====
void dhtTask(void *pvParameters) {
  for (;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;
      Serial.print("🌡 Temp: "); Serial.print(temperature);
      Serial.print(" °C | 💧 Humidity: "); Serial.println(humidity);
    } else {
      Serial.println("❌ DHT11 read error");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // DHT11 peut être lu toutes les 2s
  }
}

// ===== THINGSPEAK TASK =====
void thingSpeakTask(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(ldrValue) +
                   "&field2=" + String(temperature) +
                   "&field3=" + String(humidity);
      http.begin(url);
      int httpCode = http.GET();
      http.end();
      if (httpCode > 0) Serial.println("📤 Data sent to ThingSpeak");
      else Serial.println("❌ Failed to send data");
    }
    Serial.println("--------------------------------");
    vTaskDelay(15000 / portTICK_PERIOD_MS); // 15s
  }
}
