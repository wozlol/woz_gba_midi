constexpr uint8_t PIN_TEST_2 = 2;
constexpr uint8_t PIN_TEST_3 = 3;
constexpr uint8_t PIN_TEST_4 = 4;

constexpr uint32_t BLINK_ON_MS = 150;
constexpr uint32_t BLINK_OFF_MS = 500;
constexpr uint32_t GROUP_PAUSE_MS = 500;

void blinkPin(uint8_t pin, uint8_t count) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(pin, HIGH);
    delay(BLINK_ON_MS);
    digitalWrite(pin, LOW);
    delay(BLINK_OFF_MS);
  }

  pinMode(pin, INPUT);
  delay(GROUP_PAUSE_MS);
}

void setup() {
  pinMode(PIN_TEST_2, INPUT);
  pinMode(PIN_TEST_3, INPUT);
  pinMode(PIN_TEST_4, INPUT);
}

void loop() {
  blinkPin(PIN_TEST_2, 2);
  blinkPin(PIN_TEST_3, 3);
  blinkPin(PIN_TEST_4, 4);
}
