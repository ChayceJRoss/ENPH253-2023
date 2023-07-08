#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <controls.h>

using namespace std;

// global variables
const double period = 0.1;
const double sensor_disp = 0.5; // sensors are 1 inch appart (2 sensors)
const double Kd = 1;
const double Kp = 1;
const double tau = 0.3;

void setup(){
  // initialize pd
  PD(Kd, Kp, tau);

    // pinmodes - sensors analog or digital? input or output

  pinMode(SENSOR_A, INPUT);
  pinMode(SENSOR_C, INPUT);


    // motor initialization
    //none?


  // sensor calibration/initialization?

};

void loop()
{
  pwm_start(MOTOR_A_P1, tau, PD);

  // read sensors - save to sensorStates

  // determine state -- update state

  // calculate error from state - calculate_error

  // calculate output?

  double output;
};
