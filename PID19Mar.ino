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

int ir[8];
int position = 0, lastError = 0, error = 0;
float Kp = 15.0, Ki = 0.5, Kd = 5.0;  // Tune these values
float P, I, D, PIDvalue;
int baseSpeed = 150;  // Base motor speed (0-255)

// Sensor weights for position calculation
int weights[8] = { -4, -3, -2, -1, 1, 2, 3, 4};

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

void loop() {
  int sum = 0, count = 0;

  for (int i = 0; i < 8; i++) {
    ir[0] = digitalRead(ir1);
    ir[1] = digitalRead(ir2);
    ir[2] = digitalRead(ir3);
    ir[3] = digitalRead(ir4);
    ir[4] = digitalRead(ir5);
    ir[5] = digitalRead(ir6);
    ir[6] = digitalRead(ir7);
    ir[7] = digitalRead(ir8);

    if (ir[i] == 0) {  // Line detected
      sum += weights[i];
      count++;
    }
  }

  // Calculate Position
  if (count > 0) {
    position = sum / count;
  } else {
    position = lastError > 0 ? 4 : -4;  // If line lost, continue last movement
  }

  // Compute PID error
  error = position;
  P = error;
  I += error;
  D = error - lastError;
  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  lastError = error;

  // Adjust motor speeds using PID output
  int leftSpeed = constrain(baseSpeed - PIDvalue, 0, 255);
  int rightSpeed = constrain(baseSpeed + PIDvalue, 0, 255);

  // Move bot
  analogWrite(ML1, leftSpeed);
  analogWrite(ML2, 0);
  analogWrite(MR1, rightSpeed);
  analogWrite(MR2, 0);

  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" PID: ");
  Serial.println(PIDvalue);
  delay(10);
}
