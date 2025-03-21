#define ir1  A3
#define ir2  A2
#define ir3  7
#define ir4  6
#define ir5  A1
#define ir6  4
#define ir7  A0
#define ir8  2

int v = 60;
int u = 0;

int ir[8];
#define ML1 3
#define ML2 5
#define MR1 9
#define MR2 10

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

void moveForward() {
  analogWrite(ML1, v);
  analogWrite(ML2, u);
  analogWrite(MR1, v);
  analogWrite(MR2, u);
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

//void stopBot() {
//  digitalWrite(ML1, u);
//  digitalWrite(ML2, u);
//  digitalWrite(MR1, u);
//  digitalWrite(MR2, u);
//}

void loop() {
  ir[0] = digitalRead(ir1);
  ir[1] = digitalRead(ir2);
  ir[2] = digitalRead(ir3);
  ir[3] = digitalRead(ir4);
  ir[4] = digitalRead(ir5);
  ir[5] = digitalRead(ir6);
  ir[6] = digitalRead(ir7);
  ir[7] = digitalRead(ir8);

  // Print IR values for debugging
  Serial.print("IR: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(ir[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Prioritize LEFT movement first
  if (ir[7] || ir[6] || ir[5]) {  
    turnLeft();
    Serial.println("Left");
  } 
  // If no left signal, check for forward movement
  else if (ir[3] || ir[4]) {  
    moveForward();
    Serial.println("Forward");
  } 
  // If neither left nor forward, check for right
  else if (ir[0] || ir[1] || ir[2]) {  
    turnRight();
    Serial.println("Right");
  } 
  // If no sensor detects anything, check again before moving backward
  else {  
    delay(50);  
    bool allZero = true;
    for (int i = 0; i < 8; i++) {
      if (ir[i] == 1) {
        allZero = false;
        break;
      }
    }
    if (allZero) {
      turnBackward();
      Serial.println("Backward");
    }
  }
  delay(10); // Small delay for stability
}
