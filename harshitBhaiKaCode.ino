#define ir1  A3
#define ir2  A2
#define ir3  7
#define ir4  6
#define ir5  A1
#define ir6  4
#define ir7  A0
#define ir8  2

#define ML1 3
#define ML2 5
#define MR1 9
#define MR2 10

int irPins[8] = {ir1, ir2, ir3, ir4, ir5, ir6, ir7, ir8};
int ir[8], irr[8];

int v = 60;
int u = 0;

int baseSpeed = 120;
float Kp = 20, Ki = 0.6, Kd = 5;
int lastError = 0;
float integral = 0;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(irPins[i], INPUT);
  }
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  Serial.begin(9600);
}

int calculateError() {
  int weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
  int sum = 0, activeSensors = 0;
  for (int i = 0; i < 8; i++) {
    ir[i] = digitalRead(irPins[i]);
    sum += ir[i] * weights[i];
    activeSensors += ir[i];
  }
  return (activeSensors == 0) ? lastError : sum / activeSensors;
}

void pidControl() {
  int error = calculateError();
  int derivative = error - lastError;
  integral += error;
  int correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;

  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  analogWrite(ML1, leftSpeed);
  analogWrite(ML2, 0);
  analogWrite(MR1, rightSpeed);
  analogWrite(MR2, 0);
}

void turnLeft() {
  analogWrite(ML1, u);
  analogWrite(ML2, v);
  analogWrite(MR1, v);
  analogWrite(MR2, u);
}

void turnRight() {
  analogWrite(ML1, v);
  analogWrite(ML2, u);
  analogWrite(MR1, u);
  analogWrite(MR2, v);
}

void turnBackwardForRight() {
  analogWrite(ML1, v);
  analogWrite(ML2, u);
  analogWrite(MR1, u);
  analogWrite(MR2, v);
}

void turnBackwardForLeft() {
  analogWrite(ML1, u);
  analogWrite(ML2, v);
  analogWrite(MR1, v);
  analogWrite(MR2, u);
}

void turnAround() {
  analogWrite(ML1, v);
  analogWrite(ML2, u);
  analogWrite(MR1, u);
  analogWrite(MR2, v);
  delay(50); // Allow enough time for a 180-degree turn
}

void updatePastReadings() {
  for (int i = 0; i < 8; i++) {
    irr[i] = ir[i];
  }
}

bool wasLeftPathBefore() {
  return irr[5] || irr[6] || irr[7];
}

bool wasRightPathBefore() {
  return irr[0] || irr[1] || irr[2];
}

bool isDeadEnd() {
  for (int i = 0; i < 8; i++) {
    if (ir[i] == 1) return false;
  }
  return true;
}

void loop() {
  for (int i = 0; i < 8; i++) {
    ir[i] = digitalRead(irPins[i]);
  }

  if (isDeadEnd()) {
    Serial.println("Dead End - Turning Around");
    turnAround();
  }
  else if (ir[7] || ir[6] || ir[5]) {
    turnLeft();
    Serial.println("Left");
  }
  else if (ir[3] || ir[4]) {
    pidControl();
    Serial.println("Forward");
  }
  else if (ir[0] || ir[1] || ir[2]) {
    turnRight();
    Serial.println("Right");
  }
  else if (wasLeftPathBefore()) {
    turnBackwardForLeft();
    Serial.println("Recover Left");
  }
  else if (wasRightPathBefore()) {
    turnBackwardForRight();
    Serial.println("Recover Right");
  }
  
  updatePastReadings();
}
