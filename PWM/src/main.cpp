#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include <Adafruit_SSD1306.h>

#define MOTOR_A_FORWARD PA_1
#define MOTOR_A_BACKWARD PA_0
#define MOTOR_B_FORWARD PA_3
#define MOTOR_B_BACKWARD PA_2
#define SERVO PA_6

// put function declarations here:
#define RIGHT_ENCODER PB9
#define LEFT_ENCODER PB8

#define SCREEN_WIDTH 128 // OLED displa width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
#define SPEED 40000
#define MAX_SPEED 40000
#define DIFF_STEERING 0.0
#define MAX_ANGLE 180
#define SERVO_CENTRE 45
#define SERVO_BOTTOM 400
#define SERVO_TOP 1130
#define PICKUP_0 PA8
#define PICKUP_1 PA9

// #define SENSOR_0 PA0
// #define SENSOR_1 PA1
// #define SENSOR_2 PA2
// #define SENSOR_3 PA3

int sensors[4] = {0, 0, 0, 0};
int count_right = 0;
int count_left = 0;
int state = 0;
int prev_state_right = 0;
int prev_state_left = 0;
int t0 = 0;
int t1 = 0;
int speed = SPEED;
int target = 8;
int prev_error = 0;
int integral = 0;
int light = 0;
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void read_sensors();
void print_constants();
void servo_write(int angle);
void handle_interrupt();

void setup()
{
  pinMode(SERVO, OUTPUT);
  pinMode(MOTOR_A_FORWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);
  pinMode(MOTOR_A_BACKWARD, OUTPUT);
  pinMode(MOTOR_B_BACKWARD, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(PICKUP_0, OUTPUT);
  pinMode(PICKUP_1, OUTPUT);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);
  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  // pinMode(SONAR_INTR, INPUT_PULLUP);

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you
  // don't want this.
  display_handler.display();
  delay(2000);

  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();

  digitalWrite(PICKUP_0, HIGH);
  digitalWrite(PICKUP_1, HIGH);

  // attachInterrupt(digitalPinToInterrupt(PWM_PIN), handle_interrupt, RISING); 
  // attachInterrupt(digitalPinToInterrupt(SONAR_INTR), handle_interrupt, FALLING);
  delay(1000);
  // pinMode(SENSOR_0, INPUT_PULLUP);
  // pinMode(SENSOR_1, INPUT_PULLUP);
  // pinMode(SENSOR_2, INPUT_PULLUP);
  // pinMode(SENSOR_3, INPUT_PULLUP);
  // pinMode(PA8, OUTPUT);
  // digitalWrite(PA8, HIGH);
  servo_write(45);
  // delay(10000);
  prev_state_right = digitalRead(RIGHT_ENCODER);
  prev_state_left = digitalRead(LEFT_ENCODER);
  t0 = millis();
  prev_error = 0;
}

void loop()
{
  t1 = millis();
  int state_right = digitalRead(RIGHT_ENCODER);
  int state_left = digitalRead(LEFT_ENCODER);
  
  if (prev_state_right != state_right) {
    count_right++;
    prev_state_right = state_right;
  }
  if (prev_state_left != state_left) {
    count_left++;
    prev_state_left = state_left;
  }

  if ((t1 - t0) > 200) {
    if (count_right > 20) {
      count_right = -1;
    }
    if (count_left > 20) {
      count_left = -1;
    }
    light = !light;
    digitalWrite(PC13, light);
    t0 = t1;
    if ((count_left + count_right) / 2 > 9 ) {
      speed = 0;
      servo_write(45);
      delay(1000);
    } else {
      speed = SPEED;
    }
    count_right = 0;
    count_left = 0;


  }
  delay(10);
  while(1){
  servo_write(45);
  delay(1000);
  servo_write(75);
  delay(1000);
  servo_write(0);
  delay(1000);
  }
}

void servo_write(int angle)
{
  int local_angle = angle;
  int millisecs = map(local_angle, 0, 120, SERVO_BOTTOM, SERVO_TOP);
  int speed_adjust = (((double) angle - 60.0) / 120.0) * DIFF_STEERING * speed ;
  int left_speed = speed + speed_adjust;
  int right_speed = speed - speed_adjust;

  if (left_speed > MAX_SPEED) {
    left_speed = MAX_SPEED;
  }
  if (right_speed > MAX_SPEED) {
    right_speed = MAX_SPEED;
  }
  if (right_speed < 0) {
    right_speed = 0;
  }
  if (left_speed < 0) {
    left_speed = 0;
  }
  pwm_start(SERVO, 560, millisecs, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(20);
  // pwm_start(MOTOR_A_FORWARD, 100, right_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  // // delay(20);
  // pwm_start(MOTOR_B_FORWARD, 100, left_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
}

// void read_sensors()
// {
//   sensors[0] = digitalRead(SENSOR_0);
//   sensors[1] = digitalRead(SENSOR_1);
//   sensors[2] = digitalRead(SENSOR_2);
//   sensors[3] = digitalRead(SENSOR_3);
// }

void print_constants()
{
  display_handler.clearDisplay();
  display_handler.setCursor(0, 0);
  display_handler.println("SENSORS: " + String(sensors[0]) + " ," + String(sensors[1]) + " ," + String(sensors[2]) + ", " + String(sensors[3]));
  display_handler.display();
  delay(50);
}