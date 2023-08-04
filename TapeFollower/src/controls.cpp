#include "includes.h"

int getSpeedError(int target, int current) {
  int error = target - current;
  return error;
}

int getSpeedIntegral(int prev_integral, int error) {
  int integral = prev_integral + error;
  if (integral > SPEED_WINDUP) {
    integral = SPEED_WINDUP;
  }
  return integral;
}

int speedChange(int error, int delta_time, int prev_error, int integral) {
  int speed = error * SPEED_KP + (error - prev_error) * SPEED_KD / delta_time + integral * SPEED_KI;
  return speed;
}

