#define LDR_PIN 12
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12); // ESP32 ADC 12-bit
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("💡 LDR: ");
  Serial.println(ldrValue);

  if (ldrValue < 1500) {
    digitalWrite(LED_PIN, HIGH); // Allumer LED si sombre
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(500);
}
