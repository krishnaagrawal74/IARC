#define ir1  A3
#define ir2  A2
#define ir3  7
#define ir4  6
#define ir5  A1
#define ir6  4
#define ir7  A0
#define ir8  2

int irPins[8] = {ir1, ir2, ir3, ir4, ir5, ir6, ir7, ir8};

#define ML1 3
#define ML2 5
#define MR1 9
#define MR2 10

int baseSpeed = 60;
float Kp = 25, Ki = 0, Kd =7; // Adjusted PID values

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

void loop() {
  pidControl();
}
