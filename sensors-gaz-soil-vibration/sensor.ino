#include <WiFi.h>
#include <HTTPClient.h>

// ===== WIFI =====
const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";

// ===== GAS SENSOR =====
#define GAS_DIGITAL 2

// ===== SOIL MOISTURE =====
#define SOIL_PIN 13

// ===== VIBRATION SENSOR =====
#define VIB_SENSOR 4

// Variables partagées
volatile int gasState = 0;
volatile int soilValue = 0;
volatile int vibState = 0;

void setup() {
  Serial.begin(115200);

  pinMode(GAS_DIGITAL, INPUT);
  pinMode(VIB_SENSOR, INPUT_PULLUP);
  analogReadResolution(12);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Create tasks
  xTaskCreate(gasTask, "Gas Task", 2048, NULL, 1, NULL);
  xTaskCreate(soilTask, "Soil Task", 2048, NULL, 1, NULL);
  xTaskCreate(vibrationTask, "Vibration Task", 2048, NULL, 1, NULL);
  xTaskCreate(thingSpeakTask, "ThingSpeak Task", 4096, NULL, 1, NULL);
}

void loop() {
  // loop vide, tout est géré par les tâches
  delay(1000);
}

// ===== GAS SENSOR TASK =====
void gasTask(void *pvParameters) {
  for (;;) {
    gasState = digitalRead(GAS_DIGITAL);
    Serial.print("🧪 Gas detected: ");
    Serial.println(gasState ? "YES" : "NO");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ===== SOIL SENSOR TASK =====
void soilTask(void *pvParameters) {
  for (;;) {
    soilValue = analogRead(SOIL_PIN);
    Serial.print("🌱 Soil moisture: ");
    Serial.println(soilValue);
    if (soilValue < 1500) Serial.println(" Soil is dry!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ===== VIBRATION SENSOR TASK =====
void vibrationTask(void *pvParameters) {
  for (;;) {
    vibState = !digitalRead(VIB_SENSOR); // Invert logic
    Serial.print("📳 Vibration detected: ");
    Serial.println(vibState ? "YES" : "NO");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ===== THINGSPEAK TASK =====
void thingSpeakTask(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(gasState) +
                   "&field2=" + String(soilValue) +
                   "&field3=" + String(vibState);
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
