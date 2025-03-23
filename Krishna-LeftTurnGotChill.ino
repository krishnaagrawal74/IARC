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

int v = 150;
int u = 0;

int baseSpeed = 100;  // Increased speed
float Kp = 20, Ki = 0, Kd = 15;  // Increased Kd for faster correction
int ir[8];
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
  for (int i = 0; i < 8; i++) {
    ir[i] = digitalRead(irPins[i]);
  }

  // Check if a junction exists (multiple sensors detecting black)
  int activeSensors = 0;
  for (int i = 0; i < 8; i++) {
    activeSensors += ir[i];
  }

  // If a left-only junction is detected (left sensors active but not right)
  if ((ir[7] && ir[6] && ir[5])) {
    brake();  // Stop briefly to avoid overshoot
    baseSpeed = 50; // Reduce speed for the turn
    turnLeft();
    Serial.println("Turning Left at Junction");
    delay(250);  // Give time to turn
    baseSpeed = 100; // Restore speed after turn
  }else if(ir[7] || ir[6] || ir[5]){
    turnLeft();
    Serial.println("Left");
  }
  else if (ir[3] || ir[4]) { // Move forward if center sensors detect black
    pidControl();
    Serial.println("Forward");
  }
  else if (ir[0] || ir[1] || ir[2]) { // Turn right if right sensors detect black
    turnRight();
    Serial.println("Right");
  }
  else { 
    delay(50);
    turnBackward();
    Serial.println("Backward");
  }
}
