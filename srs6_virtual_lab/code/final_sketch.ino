
#define TRIG_PIN 9
#define ECHO_PIN 10

#define RED_LED 13
#define YELLOW_LED 12
#define GREEN_LED 11

#define BUTTON_PIN 2

const unsigned long ECHO_TIMEOUT = 30000;
const unsigned long DEBOUNCE_TIME = 50;
const unsigned long MEASURE_INTERVAL = 100;

bool buttonState = HIGH;
bool lastButtonReading = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long lastMeasureTime = 0;

bool validMeasurement = false;
float distance = 0;

float measurements[3];
byte measurementIndex = 0;

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

  Serial.println("Improved HC-SR04 system started");
}

void loop() {
  checkButton();

  if (buttonState == HIGH) {
    validMeasurement = false;
    measurementIndex = 0;
    turnOffLEDs();
    return;
  }

  if (millis() - lastMeasureTime >= MEASURE_INTERVAL) {
    lastMeasureTime = millis();
    measureDistance();
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
    validMeasurement = false;
    measurementIndex = 0;
    turnOffLEDs();
    Serial.println("ERROR: No valid Echo!");
    return;
  }

  float currentDistance = duration * 0.0343 / 2.0;

  measurements[measurementIndex] = currentDistance;
  measurementIndex++;

  if (measurementIndex >= 3) {
    distance = getMedian(
      measurements[0],
      measurements[1],
      measurements[2]
    );

    measurementIndex = 0;
    validMeasurement = true;

    updateLEDs();

    Serial.print("Filtered distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

float getMedian(float a, float b, float c) {
  if (a > b) {
    float temp = a;
    a = b;
    b = temp;
  }

  if (b > c) {
    float temp = b;
    b = c;
    c = temp;
  }

  if (a > b) {
    float temp = a;
    a = b;
    b = temp;
  }

  return b;
}

void updateLEDs() {
  turnOffLEDs();

  if (!validMeasurement) {
    return;
  }

  if (distance < 20) {
    digitalWrite(RED_LED, HIGH);
  }
  else if (distance <= 50) {
    digitalWrite(YELLOW_LED, HIGH);
  }
  else {
    digitalWrite(GREEN_LED, HIGH);
  }
}

void turnOffLEDs() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}
