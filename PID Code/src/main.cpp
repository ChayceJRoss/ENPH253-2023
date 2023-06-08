#include <Arduino.h>
#include <pid.h>


#define KP 0.5
#define KI 0.5
#define KD 0.5
#define IMAX 1

#define INPUT_PIN_LEFT PA0
#define INPUT_PIN_RIGHT PA1
#define INPUT_P_PIN PA2
#define INPUT_D_PIN PA3
#define INPUT_I_PIN PA4

int time0;
int time1;
int left;
int right;
int dt;
int pid_output;
int input_reading;
int previous;
int kp;
int ki;
int kd;

void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT_PIN_LEFT, INPUT);
  pinMode(INPUT_PIN_RIGHT, INPUT);
  pinMode(INPUT_P_PIN, INPUT);
  pinMode(INPUT_D_PIN, INPUT);
  pinMode(INPUT_I_PIN, INPUT);
  Serial.begin(9600);
  time0 = millis();
  time1 = millis();
  left = digitalRead(INPUT_PIN_LEFT);
  right = digitalRead(INPUT_PIN_RIGHT);
  previous = left - right;
}

void loop() {

  time0 = millis();
  dt = time1 - time0;
  time1 = time0;
  kp = analogRead(INPUT_P_PIN);
  ki = analogRead(INPUT_I_PIN);
  kd = analogRead(INPUT_D_PIN);
  left = digitalRead(INPUT_PIN_LEFT);
  right = digitalRead(INPUT_PIN_RIGHT);
  input_reading = left - right;
  pid_output = pid(input_reading, 0, previous, 0, dt, kp, ki, kd, IMAX);
  activate_motor(pid_output);
  previous = input_reading;
}