#include "includes.h"

void motor_setup() {
  pinMode(MOTOR_A_FORWARD, OUTPUT);
  pinMode(MOTOR_A_BACKWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);
  pinMode(MOTOR_B_BACKWARD, OUTPUT);
}

void sensor_setup() {
  pinMode(LEFT_SENSOR, INPUT_PULLUP);
  pinMode(RIGHT_SENSOR, INPUT_PULLUP);
  pinMode(CENTER_SENSOR, INPUT_PULLUP);
  pinMode(CONSTANT_TRIGGER, INPUT_PULLUP);
}

void servo_setup() {
  pinMode(SERVO, OUTPUT);
}