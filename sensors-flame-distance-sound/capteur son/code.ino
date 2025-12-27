#include <WiFi.h>
#include <HTTPClient.h>

#define SOUND_AO 4
const int SOUND_THRESHOLD = 2000;

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "TON_WRITE_API_KEY";

unsigned long previousSensorMillis = 0;
unsigned long previousThingSpeakMillis = 0;
const unsigned long sensorInterval = 500;       // 0.5s
const unsigned long thingSpeakInterval = 15000; // 15s

int soundLevel = 0;

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
  unsigned long currentMillis = millis();

  // ===== Read sound sensor every 0.5s =====
  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;

    soundLevel = analogRead(SOUND_AO);
    Serial.print("Sound level: ");
    Serial.println(soundLevel);

    bool detected = soundLevel > SOUND_THRESHOLD;
    Serial.println(detected ? " Sound detected!" : "Silence");
  }

  // ===== Send to ThingSpeak every 15s =====
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                   "&field1=" + String(soundLevel);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      Serial.println(httpCode > 0 ? "📤 Data sent to ThingSpeak" : "❌ Failed to send");
    }
  }
}
