#define TRIG_PIN 9
#define ECHO_PIN 10

#define RED_LED 13
#define YELLOW_LED 12
#define GREEN_LED 11

#define BUTTON_PIN 2

const unsigned long ECHO_TIMEOUT = 30000;
const unsigned long DEBOUNCE_TIME = 50;

bool buttonState = HIGH;
bool lastButtonReading = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(TRIG_PIN, LOW);
  turnOffLEDs();

  Serial.println("Arduino HC-SR04 system started");
}

void loop() {
  checkButton();

  if (buttonState == LOW) {
    measureDistance();
  } else {
    turnOffLEDs();
  }
}

void checkButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= DEBOUNCE_TIME) {
    buttonState = reading;
  }

  lastButtonReading = reading;
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(
    ECHO_PIN, HIGH, ECHO_TIMEOUT
  );

  if (duration == 0) {
    turnOffLEDs();
    Serial.println("ERROR: No Echo signal!");
    delay(100);
    return;
  }

  float distance = duration * 0.0343 / 2.0;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  turnOffLEDs();

  if (distance < 20) {
    digitalWrite(RED_LED, HIGH);
  }
  else if (distance <= 50) {
    digitalWrite(YELLOW_LED, HIGH);
  }
  else {
    digitalWrite(GREEN_LED, HIGH);
  }

  delay(100);
}

void turnOffLEDs() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}