// ==========================================
// SMART REVERSE PARKING ASSIST SYSTEM
// ==========================================

// ---------- Ultrasonic Sensor ----------
#define TRIG_PIN 9
#define ECHO_PIN 10

// ---------- LEDs ----------
#define GREEN_LED 2
#define YELLOW_LED 3
#define RED_LED 4

// ---------- Buzzer ----------
#define BUZZER 5

// ---------- 7 Segment Display ----------
int segmentPins[7] = {6, 7, 8, 11, 12, 13, A0};

// Segment patterns for COMMON CATHODE
// a b c d e f g

byte numbers[10][7] = {

  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

long duration;
int distance;
int level;

void setup() {

  // Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LED Pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // 7 Segment Pins
  for(int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {

  // Clear Trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send Pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read Echo Time
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate Distance
  distance = duration * 0.034 / 2;

  // Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ======================================
  // LEVEL 5 : SAFE
  // ======================================

  if(distance > 40) {

    level = 5;

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);

    noTone(BUZZER);
  }

  // ======================================
  // LEVEL 4 : SAFE CAUTION
  // ======================================

  else if(distance > 30) {

    level = 4;

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);

    tone(BUZZER, 1000);
    delay(500);

    noTone(BUZZER);
    delay(500);
  }

  // ======================================
  // LEVEL 3 : CAUTION
  // ======================================

  else if(distance > 20) {

    level = 3;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    tone(BUZZER, 1200);
    delay(300);

    noTone(BUZZER);
    delay(300);
  }

  // ======================================
  // LEVEL 2 : DANGER
  // ======================================

  else if(distance > 10) {

    level = 2;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 1500);
    delay(150);

    noTone(BUZZER);
    delay(150);
  }

  // ======================================
  // LEVEL 1 : VERY DANGEROUS
  // ======================================

  else {

    level = 1;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 2000);
  }

  // Display Level
  displayDigit(level);

  delay(50);
}

// ==========================================
// FUNCTION TO DISPLAY DIGITS
// ==========================================

void displayDigit(int num) {

  for(int i = 0; i < 7; i++) {

    digitalWrite(segmentPins[i], numbers[num][i]);
  }
}