//Sensor config
#define SENSOR_COUNT  5
int sensor_weights[SENSOR_COUNT] = {-2, -1, 0, 1, 2};
int sensor_pins[SENSOR_COUNT] = {A0, A1, A2, A3, A4};

//Motor config
#define FRONT_LEFT_MOTOR  5
#define REAR_LEFT_MOTOR   6
#define FRONT_RIGHT_MOTOR 9
#define REAR_RIGHT_MOTOR  10

//PID config
float Kp = 1.5, Ki = 0.01, Kd = 0.5;  // PID variables
float error, last_error = 0, integral = 0;
int last_position = 0;
int threshold = 500;  // Sesnor Reading Compare Value


void setup(){
  pinMode(FRONT_LEFT_MOTOR, OUTPUT);
  pinMode(REAR_LEFT_MOTOR, OUTPUT);
  pinMode(FRONT_RIGHT_MOTOR, OUTPUT);
  pinMode(REAR_RIGHT_MOTOR, OUTPUT);
  Serial.begin(9600); // for debugging || monitoring sensor values
}

int read_sensors(void){
    int position = 0;
    int count = 0;// how many sensors detected the line
    
    //loop over sensors
    for (char i = 0; i < SENSOR_COUNT; i++) {
        char value = analogRead(sensor_pins[i]) > threshold ? 1 : 0;  // returns one if True, zero if false
        position += sensor_weights[i] * value;
        count += value;
    }
    
    //update position based on the combined reading of all sensrs
    if (count == 0) return last_position;  // if line detected (sensors read zero), keep last position
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
    int base_speed = 150;  // Base motor speed
    int left_speed = base_speed + correction;   // +ve correction = turn right
    int right_speed = base_speed - correction;

    left_speed = constrain(left_speed, 0, 255);
    right_speed = constrain(right_speed, 0, 255);

    //adjust motor speeds
    analogWrite(FRONT_LEFT_MOTOR, left_speed);
    analogWrite(REAR_LEFT_MOTOR, left_speed);

    analogWrite(FRONT_RIGHT_MOTOR, right_speed);
    analogWrite(REAR_RIGHT_MOTOR, right_speed);

}

void loop() {
  
    int position = read_sensors();
    float correction = compute_PID(position);
    adjust_motors(correction);

    Serial.print("Position: "); Serial.print(position);Serial.print(" | Correction: "); Serial.println(correction); //debugging
}
