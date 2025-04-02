#include <Servo.h>  // Include the Servo library

Servo myServo;  // Create a Servo object

void setup() {
  myServo.attach(10);  // Attach servo to pin 9
}

void loop() {
  // myServo.write(90);  // Move to 90 degrees
  // delay(2000);        // Wait 2 seconds

  // myServo.write(0);   // Move to 0 degrees
  // delay(2000);        // Wait 2 seconds

  myServo.write(0); // Move to 180 degrees
  delay(2000);        // Wait 2 seconds
}
