#include <WiFi.h>
#define LIR 32
#define RIR 34

// #define EN12 7
// #define EN34 6
#define EN12 12
#define EN34 13

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

const char* ssid = "realme GT 2";
const char* password = "12345678";

WiFiServer server(80);

void setup() {
    pinMode(LIR, INPUT);
    pinMode(RIR, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    ledcAttach(EN12, 5000, 8); // Pin, Frequency, Resolution

    ledcAttach(EN34, 5000, 8);


    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}


void set_motor_speed(int motor, int speed) {
  if (motor == 0) { // Left motor
    if (speed > 0) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(0, speed);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      ledcWrite(0, 0);
    }
  } else if (motor == 1) { // Right motor
    if (speed > 0) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      ledcWrite(1, speed);
    } else {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      ledcWrite(1, 0);
    }
  }
}

void loop() {
    WiFiClient client = server.available();
    if (!client) return;

    Serial.println("Client connected");

    while (!client.available()) {
        delay(1);
    }

    String req = client.readStringUntil('\r');
    client.readStringUntil('\n'); // discard the rest
    Serial.print("Request: ");
    Serial.println(req);
    int left = analogRead(LIR);   // 0 - 1023
    int right = analogRead(RIR);

    error = (float)(right - left); // Positive if drifted left
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
    
    if (req.indexOf("/sensor") >= 0) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.print("Error: ");
        client.print(error);
        client.print(" Correction: ");
        client.println(correction);
    } else {
        // Serve HTML with JavaScript
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE html><html><head><title>ESP32 Sensor</title></head><body>");
        client.println("<h1>Line Follower</h1>");
        client.println("<div id='sensor'>Loading...</div>");
        client.println("<script>");
        client.println("setInterval(() => {");
        client.println("fetch('/sensor').then(r => r.text()).then(data => {");
        client.println("document.getElementById('sensor').innerText = data;");
        client.println("}); }, 500);");
        client.println("</script>");
        client.println("</body></html>");
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected");
}
