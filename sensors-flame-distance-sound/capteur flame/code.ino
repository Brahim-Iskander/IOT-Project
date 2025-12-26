#include <WiFi.h>
#include <HTTPClient.h>

#define FLAME_PIN 6
#define LED_PIN 7

const char* ssid = "TON_SSID_2.4GHz";  
const char* password = "TON_MOT_DE_PASSE";
String apiKey = "ZY59RYO41TC9A23L";

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_PIN, INPUT_PULLUP);
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
  int flame = digitalRead(FLAME_PIN);
  Serial.print("Flame: ");
  Serial.println((flame == LOW) ? "Detected" : "None");

  if (flame == LOW) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(flame == LOW ? 1 : 0);
    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println(httpCode > 0 ? " Data sent to ThingSpeak" : "Failed to send");
  }

  delay(15000);
}
