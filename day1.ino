// Define sensor pins
#define NUM_SENSORS 8
int ir_pins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, 6, 7}; // IR sensor pins
int sensor_values[NUM_SENSORS];  // Declare array to store sensor readings

void setup() {
    Serial.begin(9600);
    
    // Set IR sensor pins as input
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(ir_pins[i], INPUT);
    }
}

void loop() {
    // Read sensor values
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensor_values[i] = digitalRead(ir_pins[i]);
        
//        Serial.print("IR");
//        Serial.print(i);
//        Serial.print("-");
        Serial.print(sensor_values[i], " ");
       
    }
    Serial.println();
    
    delay(100); // Small delay for readability in Serial Monitor
}
