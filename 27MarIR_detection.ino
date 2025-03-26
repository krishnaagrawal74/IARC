
#define ir1  A0
#define ir2  A1
#define ir3  A2
#define ir4  A3
#define ir5  A4
#define ir6  A5
#define ir7  7
#define ir8  2

#define ML1 5
#define ML2 10
#define MR1 3
#define MR2 9

const int irSensorPin = 13; // Analog pin connected to IR sensor output

int irPins[8] = {ir1, ir2, ir3, ir4, ir5, ir6, ir7, ir8};

int v = 140;
int u = 0;
int sp = 150;
int baseSpeed = sp;
float Kp = 25, Ki = 0, Kd = 17; // Adjusted PID values
int ir[8];
int lastError = 0;
float integral = 0;

void setup() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
  pinMode(ir6, INPUT);
  pinMode(ir7, INPUT);
  pinMode(ir8, INPUT);

  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);

  Serial.begin(9600);
}

int calculateError() {
  int weights[8] = { -4, -3, -2, -1, 1, 2, 3, 4};
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

void turnBackward() {
  analogWrite(ML1, v);
  analogWrite(ML2, u);
  analogWrite(MR1, u);
  analogWrite(MR2, v);
}


void brake() {
  analogWrite(ML1, 0);
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
  delay(100);  // Small delay for stability
}

void loop() {
  ir[0] = digitalRead(ir1);
  ir[1] = digitalRead(ir2);
  ir[2] = digitalRead(ir3);
  ir[3] = digitalRead(ir4);
  ir[4] = digitalRead(ir5);
  ir[5] = digitalRead(ir6);
  ir[6] = digitalRead(ir7);
  ir[7] = digitalRead(ir8);

  int irvalue = digitalRead(irSensorPin);

  if (irvalue == 0) {
     turnRight();
     delay(450);
     Serial.println("  || Backward due to ULTRASONIC");
  }

  // Prioritize LEFT movement first
  if (ir[7] && ir[6] && ir[5] && ir[4]) {
    int t = millis();
    while (millis() - t < 30) {
      analogWrite(ML1, 80);
      analogWrite(ML2, 0);
      analogWrite(MR1, 80);
      analogWrite(MR2, 0);
    }

    brake();
    baseSpeed = 50;
    turnLeft();
    Serial.println("turning Left");
    delay(200);
    baseSpeed = sp;
  } else if (ir[7] || ir[6] || ir[5]) {
    turnLeft();
    Serial.println("Left");
  }
  // If no left signal, check for forward movement
  else if (ir[3] || ir[4]) {
    pidControl();
    Serial.println("Forward");
  }
  // If neither left nor forward, check for right
  else if (ir[0] || ir[1] || ir[2]) {
    turnRight();
    Serial.println("Right");
  }
  // If no sensor detects anything, check again before moving backward
  else {
    turnBackward();
    Serial.println("Backward");

  }
}
