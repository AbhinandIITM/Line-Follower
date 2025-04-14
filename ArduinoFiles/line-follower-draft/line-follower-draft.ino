#define LIR A0
#define RIR A1

#define EN12 7
#define EN34 6

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// PID Variables
float kp = 0.5;
float kd = 0.01;
float ki = 0.0;
int base_speed = 200;
float error = 0;
float integral = 0;
float derivative = 0;
float last_error = 0;
float output = 0;

void setup() {
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(EN12, base_speed);  // Use analogWrite only once per update
  analogWrite(EN34, base_speed);

  Serial.begin(9600);
}

void loop() {
  int left = analogRead(LIR);   // 0 - 1023
  int right = analogRead(RIR);

  float error = (float)(right - left); // Positive if drifted left
  integral += error;
  derivative = error - last_error;
  float correction = kp * error + kd * derivative + ki * integral;

  int left_speed = base_speed + correction;
  int right_speed = base_speed - correction;

  // Clamp values between 0 and 255
  left_speed = constrain(left_speed, 0, 255);
  right_speed = constrain(right_speed, 0, 255);

  set_motor_speed(0, left_speed);
  set_motor_speed(1, right_speed);

  /*Serial.print("L: "); Serial.print(left);
  Serial.print(" R: "); Serial.print(right);*/
   // --------- Serial Plotter Output ---------
  //Serial.print("Correction: ");
  Serial.println(correction);
  delay(10);
  /*Serial.print("\tError: ");
  Serial.println(error);
  delay(2);*/

  last_error = error;
  delay(20);  // adjust for smoother response
}

void set_motor_speed(int motor, int speed) {
  if (motor == 0) { // Left motor
    if (speed > 0) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(EN12, speed);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(EN12, 0);
    }
  } else if (motor == 1) { // Right motor
    if (speed > 0) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN34, speed);
    } else {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(EN34, 0);
    }
  }
}
