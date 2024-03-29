#include "includes.h"
#include <Adafruit_I2CDevice.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define SPEED 50000
#define MAX_SPEED 50000
#define DIFF_STEERING 2.5
#define DOWNHILL_SPEED 18000
#define MAX_ANGLE 40
#define P_CONST 6.0  
#define D_CONST  0.0
#define I_CONST 0.00
#define SERVO_TOP 1130
#define SERVO_BOTTOM 400
#define MAX_INTEGRAL 10
#define D_SAMPLE 10
#define SERVO_CENTER 80
#define BINARY_COUNTS 0
#define SENSOR_3_VALUE 3.0
#define SENSOR_2_VALUE 2.0
#define SENSOR_1_VALUE 1.0
#define OFF_TAPE MAX_ANGLE / P_CONST
#define SPEED_SAMPLE_TIME 100 
#define SLOWDOWN_TIME 4000
#define BETWEEN_SWITCH_DELAY 1000
#define HALL_SENSOR_TIME 1000
#define COLLISION_STOP_TIME 500
#define COLLISION_SPEED 0
#define NO_CLICK_TIME 10000
#define HILL_RIGHT_TURN_TIME 100
#define FAST_SPEED 50000
#define COLLECTION_LAPS 2
// // // put function declarations here:

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int collection_setting = HIGH;
int lap_counter = 0;
int running_speed = SPEED;
int binary_counts = BINARY_COUNTS;
bool flag = false;
int left_sensor = 0;
int right_sensor = 0;
int limit_switch = 0;
int n;
int speed = SPEED;
int speed_target = 5;
int hall_sensor_left = 0;
int hall_sensor_right = 0;
int collision_stop_timer = 0;
int collision_sensor = 0;
int hall_sensor_timer_right = 0;
int hall_sensor_timer_left = 0;
int no_click_timer = 0;
int no_click_timer_prev = 0;
uint8_t light = HIGH;
void servo_write(int angle);
int t0 = 0;
int t1 = 0;
int tau = 0;
int starter_loops = 3;
double integral;
int sensor_values_tL[] = {0, 0}; //front row, left side (ordered outermost sensor to innermost) FOR 6 and 4 setup
int sensor_values_tR[] = {0, 0}; //front row, right side (ordered innermost sensor to outermost) FOR 6 and 4 setup
int sensor_values_centres[] = {0,0}; //centre line followers for six sensors (0 for left, 1 for right)
int speed_mode = 0;
int sensor_values_bL[] = {0, 0}; //back row, left side (ordered outermost sensor to innermost) in addition to tL and tR, for eight>
int sensor_values_bR[] = {0, 0}; //back row, right side (ordered outermost sensor to innermost) ^^

double state = 0;
double prevState = 0;
double kd = D_CONST;
double kp = P_CONST;
double output = 0;
int d_timer = 0;
int loop_count = 0;
int gyro_transmission[] = {0,0,0,0,0,0,0,0,0,0};
int Addr = 0x28;
int speed_sample_timer = 0;
int slowdown_timer = 0;
int slowdown_timer_prev = 0;
int limit_switch_counter = 0;
int between_switch_delay_timer = 0;
int between_switch_delay_timer_prev = 0;
int signal_timer = 0;

void collision();
void signal_interrupt();
double getP(int state);
void resetState(int newState);
int getPrev();
double d();
double p();
double i();
int getOutput();
double find_state_six();
void read_sensors();
void set_constants();
void getGyro();

String convertToString(int* a, int size)
{
    int i;
    String s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


//PD "class"
    

void setup() {
  //turning servo setup
  pinMode(SERVO, OUTPUT);

  //sensor setup
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(s5, INPUT_PULLUP); //add for six
  pinMode(s6, INPUT_PULLUP); //add for six

  //motor setup
  pinMode(MOTOR_A_FORWARD, OUTPUT);
  pinMode(MOTOR_A_BACKWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);
  pinMode(MOTOR_B_BACKWARD, OUTPUT);
  pinMode(PC13, OUTPUT);

  // limit switch
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(SIGNAL_LED, OUTPUT);

  pinMode(PICKUP_LEFT_FORWARD, OUTPUT);
  pinMode(PICKUP_RIGHT_FORWARD, OUTPUT);
  pinMode(PICKUP_LEFT_BACK, OUTPUT);
  pinMode(PICKUP_RIGHT_BACK, OUTPUT);

  pinMode(HALL_SENSOR_LEFT, INPUT_PULLUP);
  pinMode(HALL_SENSOR_RIGHT, INPUT_PULLUP);

  pinMode(SONAR_PWM, OUTPUT);
  pinMode(SONAR_INPUT, INPUT_PULLUP);

  // OLED setup
  // display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display_handler.display();
  // delay(2000);
  // display_handler.clearDisplay();
  // display_handler.setTextSize(1);
  // display_handler.setTextColor(SSD1306_WHITE);
  // display_handler.setCursor(0,0);
  // display_handler.println("Hello, world!");
  // display_handler.display();

  pwm_start(MOTOR_A_BACKWARD, 100, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  pwm_start(MOTOR_B_BACKWARD, 100, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);

  // for (int i = 0; i < 10; i++) {
  // Wire.begin();
  // Wire.setClock(10000);
  // Wire.beginTransmission(Addr);
  // gyro_transmission[i] = Wire.endTransmission();
  // display_handler.setCursor(0,0);
  // display_handler.clearDisplay();
  // for (size_t i = 0; i < 10; i++)
  // {
  //   display_handler.println(gyro_transmission[i]);
  // }
  // display_handler.display();
  // delay(2000);
  // }
  // gyro setup
  // if(!bno.begin()) {
  //   while(1) {
  //       digitalWrite(PC13, HIGH);
  //       delay(1000);
  //       digitalWrite(PC13, LOW);
  //       delay(1000);
  //   }
  // }

  // bno.setExtCrystalUse(false);
  // Wire.setClock(10000);
 
  // heartbeat
  digitalWrite(PC13, HIGH);

  // sonar
  pwm_start(SONAR_PWM, 800, 20, RESOLUTION_12B_COMPARE_FORMAT);

  // center servo
  servo_write(SERVO_CENTER);
  delay(100); 

  // initialisation constants
  t0 = millis();
  read_sensors();
  prevState = find_state_six();

  // digitalWrite(PICKUP_0, HIGH);
  // digitalWrite(PICKUP_1, HIGH);

  // collision interupt
  // attachInterrupt(digitalPinToInterrupt(LEFT_TAPE_INTR), signal_interrupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(RIGHT_TAPE_INTR), signal_interrupt, FALLING);
}

void loop() {
  // initialisation loops
  if (starter_loops) {
    starter_loops--;
    flag = false;
    delay(500);
    return;
  }

  // collision
  // if (flag) {
  //   pwm_start(MOTOR_A_FORWARD, 100, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  //   delay(20);
  //   pwm_start(MOTOR_B_FORWARD, 100, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);

  //   delay(3000);
  //   flag = false;
  //   return;
  // }

  // find state
  read_sensors();
  state = find_state_six();
  t1 = millis();
  tau = t1 - t0;
  d_timer += tau;
  // D sampling
  if (d_timer >= D_SAMPLE) {
    speed_sample_timer += d_timer;
    prevState = state;
    d_timer = 0;
    if (speed_sample_timer >= SPEED_SAMPLE_TIME) {
      hall_sensor_left = digitalRead(HALL_SENSOR_LEFT);
      hall_sensor_right = digitalRead(HALL_SENSOR_RIGHT);
      collision_sensor = digitalRead(SONAR_INPUT);
      limit_switch = digitalRead(LIMIT_SWITCH);
      no_click_timer += SPEED_SAMPLE_TIME;
      hall_sensor_timer_right += SPEED_SAMPLE_TIME;
      hall_sensor_timer_left += SPEED_SAMPLE_TIME;
      between_switch_delay_timer += SPEED_SAMPLE_TIME;
      collision_stop_timer += SPEED_SAMPLE_TIME;
      signal_timer += SPEED_SAMPLE_TIME;
      slowdown_timer += SPEED_SAMPLE_TIME;
      if (collision_sensor == HIGH) {
        no_click_timer_prev = no_click_timer;
        between_switch_delay_timer_prev = between_switch_delay_timer;
        slowdown_timer_prev = slowdown_timer;
        collision_stop_timer = 0;
        speed = COLLISION_SPEED;
      }
      if (collision_stop_timer <= COLLISION_STOP_TIME) {
        no_click_timer = no_click_timer_prev;
        between_switch_delay_timer = between_switch_delay_timer_prev;
        slowdown_timer = slowdown_timer_prev;
      }
      if (hall_sensor_left == LOW) {
        hall_sensor_timer_left = 0;
        digitalWrite(PICKUP_LEFT_FORWARD, LOW);
        pwm_start(PICKUP_LEFT_BACK, 50, 50000, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
      }
      if (hall_sensor_right == LOW) {
        hall_sensor_timer_right = 0;
        digitalWrite(PICKUP_RIGHT_FORWARD, LOW);
        pwm_start(PICKUP_RIGHT_BACK, 50, 50000, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
      }
      if (hall_sensor_timer_left >= HALL_SENSOR_TIME) {
        pwm_start(PICKUP_LEFT_BACK, 50, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        digitalWrite(PICKUP_LEFT_FORWARD, collection_setting);
      }

      if (hall_sensor_timer_right >= HALL_SENSOR_TIME) {
        pwm_start(PICKUP_RIGHT_BACK, 50, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        digitalWrite(PICKUP_RIGHT_FORWARD, collection_setting);
      }
      if (signal_timer > 1000) {
        digitalWrite(SIGNAL_LED, LOW);
      }
      if (slowdown_timer >= SLOWDOWN_TIME && collision_stop_timer >= COLLISION_STOP_TIME)
      {
        speed = running_speed;
      }
      if (limit_switch == LOW && between_switch_delay_timer >= BETWEEN_SWITCH_DELAY && no_click_timer >= NO_CLICK_TIME) {
        signal_timer = 0;
        digitalWrite(SIGNAL_LED, HIGH);
        between_switch_delay_timer = 0;
        limit_switch_counter++;
        if (limit_switch_counter > 1) {
          slowdown_timer = 0;
          speed = DOWNHILL_SPEED;
        } 
        if (limit_switch_counter > 2) {
          lap_counter++;
          if (lap_counter >= COLLECTION_LAPS)
          {
            running_speed = FAST_SPEED;
            collection_setting = LOW;
          }
          
          no_click_timer = 0;
          limit_switch_counter = 0;
        } 
      }
      speed_sample_timer = 0;
    }
  }
  t0 = t1;
  // PD
  output = getOutput();

  // check if outside of angle range
  if (abs(output) > MAX_ANGLE)
  {
    output = MAX_ANGLE * ((output > 0) ? 1 : -1);
  }
  if (limit_switch_counter == 3) {
    servo_write(SERVO_CENTER - output);
  }
   else if (limit_switch_counter == 0 && slowdown_timer < HILL_RIGHT_TURN_TIME) {
    signal_timer = 0;
    digitalWrite(SIGNAL_LED, HIGH);
    servo_write(35);
  } 
  else {
  // write angle to servo
  servo_write(SERVO_CENTER - output);
  }
  //speed control

  loop_count++;
}

void read_sensors() {
  sensor_values_tL[0] = digitalRead(s1); 
  sensor_values_tL[1] = digitalRead(s2);
  sensor_values_centres[0] = digitalRead(s3);
  sensor_values_centres[1] = digitalRead(s4);
  sensor_values_tR[0] = digitalRead(s5);
  sensor_values_tR[1] = digitalRead(s6);
}

void servo_write(int angle)
{
  int local_speed = speed;
  // convert angle to millisecs
  int millisecs = map(angle, 120, 0, SERVO_BOTTOM, SERVO_TOP);

  // adjust speed based on angle 
  // local_speed = SPEED / (1 + abs(60 - angle) / 20.0 );

  // adjust speed based on number of sensors

  if (n == 0 && state != 0) {
    local_speed = local_speed * 0.35;
  }
  // find diff speed adjustment
  int speed_adjust = (((double) angle - (double) SERVO_CENTER) / (2 * MAX_ANGLE)) * DIFF_STEERING * local_speed ;

  // differential steering
  int left_speed = local_speed + speed_adjust;
  int right_speed = local_speed - speed_adjust;

  // check if speed is within bounds
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
  // write to servo and motors
    pwm_start(SERVO, 560, millisecs, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(20);
    pwm_start(MOTOR_A_FORWARD, 100, left_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  // // delay(20);
    pwm_start(MOTOR_B_FORWARD, 100, right_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
}


 // collision interrupt
void collision() {
  digitalWrite(PC13, LOW);
  flag = true;
}


double getP(int state){
  double prop = kp * state;

  return prop;
}
void resetState(int newState){
  prevState = state;
  state = newState;
}    

int getPrev(){
  return prevState;
}   

double d(){
  double der;
  der = kd*(state - prevState); 
  return der;
}   

double p(){
  double prop;
  prop = kp*state;

  return prop;
}    

double i() {
  integral = integral + I_CONST * state;
  if (abs(integral) > MAX_INTEGRAL) {
    integral = MAX_INTEGRAL * (integral > 0 ? 1 : -1);
  }
  if (n != 0){
    integral = 0;
  }
  return integral;
} 

int getOutput(){
  double output;
  output = p() + d() + i();

  return (int) output;
}


double find_state_six(){
  double u;

  // find weighted sum of sensors
  u = 0 - SENSOR_3_VALUE * sensor_values_tL[0] - SENSOR_2_VALUE*sensor_values_tL[1]  - SENSOR_1_VALUE * sensor_values_centres[0] + SENSOR_1_VALUE *sensor_values_centres[1] + SENSOR_2_VALUE*sensor_values_tR[0] + SENSOR_3_VALUE * sensor_values_tR[1];

  // find number of sensors
  n = sensor_values_tL[0] + sensor_values_tL[1] + sensor_values_centres[0] + sensor_values_centres[1] + sensor_values_tR[0] + sensor_values_tR[1];
  
  // set state if no sensors
  if (n == 0){
    if (state == 0) {
      return 0;
    }
    return (state > 0) ? OFF_TAPE : -1 * OFF_TAPE ;
  }

  return (double) u / n;
}

void signal_interrupt() {
  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
}
