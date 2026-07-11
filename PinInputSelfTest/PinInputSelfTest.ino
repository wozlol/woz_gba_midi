constexpr uint8_t PIN_STATUS_LED = 16;
constexpr uint8_t TEST_PINS[] = {2, 3, 4};

constexpr uint32_t SETTLE_MS = 20;
constexpr uint32_t BLINK_ON_MS = 120;
constexpr uint32_t BLINK_OFF_MS = 500;
constexpr uint32_t GROUP_PAUSE_MS = 500;

void blinkOutputPin(uint8_t pin, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(pin, HIGH);
    delay(BLINK_ON_MS);
    digitalWrite(pin, LOW);
    delay(BLINK_OFF_MS);
  }
}

bool inputSelfTest(uint8_t pin) {
  pinMode(pin, INPUT_PULLDOWN);
  delay(SETTLE_MS);
  const bool readsLow = digitalRead(pin) == LOW;

  pinMode(pin, INPUT_PULLUP);
  delay(SETTLE_MS);
  const bool readsHigh = digitalRead(pin) == HIGH;

  pinMode(pin, INPUT);
  return readsLow && readsHigh;
}

bool allInputsPass() {
  for (uint8_t pin : TEST_PINS) {
    if (!inputSelfTest(pin)) {
      return false;
    }
  }
  return true;
}

void outputBlinkPattern() {
  for (uint8_t pin : TEST_PINS) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    blinkOutputPin(pin, pin);

    pinMode(pin, INPUT);
    delay(GROUP_PAUSE_MS);
  }
}

void setup() {
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  for (uint8_t pin : TEST_PINS) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  const bool inputsPass = allInputsPass();
  digitalWrite(PIN_STATUS_LED, inputsPass ? HIGH : LOW);

  if (!inputsPass) {
    digitalWrite(PIN_STATUS_LED, HIGH);
    delay(BLINK_ON_MS);
    digitalWrite(PIN_STATUS_LED, LOW);
    delay(BLINK_OFF_MS);
  }

  outputBlinkPattern();
}
