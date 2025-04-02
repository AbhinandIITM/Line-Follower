#define ENA 9  // PWM for speed control
#define IN1 8  // Motor direction
#define IN2 7  // Motor direction

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  Serial.begin(9600);  // Start serial communication
  Serial.println("L298N Motor Control Started");
}

void loop() {
  // Move Forward at Full Speed
  Serial.println("Motor Moving Forward at Full Speed");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);  // Max speed (0-255)
  delay(4000);

  // Stop Motor
  Serial.println("Motor Stopped");
  analogWrite(ENA, 0);
  delay(1000);

  // Move Backward at Half Speed
  Serial.println("Motor Moving Backward at Full Speed");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);  // Try full speed
  delay(4000);

  // Stop Motor
  Serial.println("Motor Stopped");
  analogWrite(ENA, 0);
  delay(1000);
}
