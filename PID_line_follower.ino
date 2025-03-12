//Sensor config
#define SENSOR_COUNT  5
int sensor_weights[SENSOR_COUNT] = {-2, -1, 0, 1, 2};
int sensor_pins[SENSOR_COUNT] = {A0,A1,A2,A3,A4};
//Motor direction Control
#define EN_A  12  // Right Motor
#define EN_B  11  // Left Motor
#define IN_1  10
#define IN_2  6
#define IN_3  9
#define IN_4  5

//PID config
#define Kp 56.8
#define Ki 1.7
#define Kd 0.0
float error, last_error = 0, integral = 0;
int last_position = 0;
int threshold = 500;  // Sesnor Reading Compare Value


void setup(){
  pinMode(IN_4, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_1, OUTPUT);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);  

  //motor direction control
  digitalWrite(IN_1,HIGH);
  digitalWrite(IN_2,LOW);
  digitalWrite(IN_3,HIGH);
  digitalWrite(IN_4,LOW);

  Serial.begin(9600); // for debugging || monitoring sensor values
}

int read_sensors(void){
    int position = 0;
    int count = 0;// how many sensors detected the line
    
    //loop over sensors
    for (int i = 0; i < SENSOR_COUNT; i++) {
        int value = analogRead(sensor_pins[i]) > threshold ? 1 : 0;  // returns one if True, zero if false
        position += sensor_weights[i] * value;
        count += value;
    }
    
    //update position based on the combined reading of all sensrs
    if (count == 0) return last_position;  // if line detected (sensors combined reading is zero), keep last position
    last_position = position / count;
    return last_position;
}


float compute_PID(int position) {
    error = position;  // Deviation from center, 0 if exactly centered
    integral += error;
    float derivative = error - last_error;
    last_error = error;

    return (Kp * error) + (Ki * integral) + (Kd * derivative);
}

void adjust_motors(float correction) {
    int base_speed = 50;  // Base motor speed
    int left_speed = base_speed + correction;   // +ve correction = turn right
    int right_speed = base_speed - correction;  // -ve correction = turn left

    left_speed = constrain(left_speed, 0, 255);
    right_speed = constrain(right_speed, 0, 255);
    
    analogWrite(EN_A, right_speed);
    analogWrite(EN_B, left_speed);
     
}

void loop() {
  
    int position = read_sensors();
    float correction = compute_PID(position);
    adjust_motors(correction);

    Serial.print("Position: "); Serial.print(position);Serial.print(" | Correction: "); Serial.println(correction); //debugging
    delay(25);
}
