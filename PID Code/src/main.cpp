#include <Arduino.h>
#include <pid.h>


#define KP 0.5
#define KI 0.5
#define KD 0.5
#define IMAX 1

#define INPUT_PIN_LEFT PA0
#define INPUT_PIN_RIGHT PA1

int time0;
int time1;
int left;
int right;
int dt;
int pid_output;
int input_reading;
int previous;

void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT_PIN_LEFT, INPUT);
  pinMode(INPUT_PIN_RIGHT, INPUT);
  Serial.begin(9600);
  time0 = millis();
  time1 = millis();
  left = digitalRead(INPUT_PIN_LEFT);
  right = digitalRead(INPUT_PIN_RIGHT);
  previous = left - right;
}

void loop() {
  // put your main code here, to run repeatedly:
  time0 = millis();
  dt = time1 - time0;
  time1 = time0;
  left = digitalRead(INPUT_PIN_LEFT);
  right = digitalRead(INPUT_PIN_RIGHT);
  input_reading = left - right;
  pid_output = pid(input_reading, 0, previous, 0, dt, KP, KI, KD, IMAX);
  activate_motor(pid_output);
  previous = input_reading;
}