const int NUM_SENSORS = 5;
const int IR_LED_PIN = 2; // IR LEDs control pin
const int IR_RECEIVER_PINS[NUM_SENSORS] = {A0, A1, A2, A3, A4}; // Phototransistor connected to analog pins A0, A1, A2, A3, A4
 
void setup() {
  Serial.begin(9600);
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, HIGH); // Turn on the IR LEDs
}
 
void loop() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    int sensorValue = analogRead(IR_RECEIVER_PINS[i]);
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(" Value: ");
    Serial.println(sensorValue);
  }
  Serial.println();
  delay(500); // Wait 500ms between readings
}