#define PIR_PIN 5
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("🚶 Motion detected");
    digitalWrite(LED_PIN, HIGH); // Allumer LED quand mouvement
  } else {
    Serial.println("No motion");
    digitalWrite(LED_PIN, LOW);
  }

  delay(500);
}
