#include "includes.h"
#include <Adafruit_I2CDevice.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define SPEED 15000
#define MAX_SPEED 30000
#define DIFF_STEERING 1.0
#define MAX_ANGLE 34
#define P_CONST 8.3
#define D_CONST  20.0
#define I_CONST 0.01
#define SERVO_TOP 1130
#define SERVO_BOTTOM 400
#define MAX_INTEGRAL 10
#define D_SAMPLE 100
#define SERVO_CENTER 60
#define BINARY_COUNTS 3
#define SENSOR_3_VALUE 1.0
#define SENSOR_2_VALUE 0.6
#define SENSOR_1_VALUE 0.3

// // // put function declarations here:

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int binary_counts = BINARY_COUNTS;
bool flag = false;
int n;
int speed = SPEED;
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

void collision();
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

  // sonar setup
  pinMode(ECHOPIN, INPUT_PULLUP);
  pinMode(TRIGPIN, OUTPUT);

  pinMode(PICKUP_0, OUTPUT);
  pinMode(PICKUP_1, OUTPUT);

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
  digitalWrite(PC13, LOW);

  // sonar
  // pwm_start(SONAR_PWM, 250, 90, RESOLUTION_12B_COMPARE_FORMAT);

  // center servo
  servo_write(SERVO_CENTER);
  delay(100); 

  // initialisation constants
  t0 = millis();
  read_sensors();
  prevState = find_state_six();

  digitalWrite(PICKUP_0, HIGH);
  digitalWrite(PICKUP_1, HIGH);

  // collision interupt
  // attachInterrupt(digitalPinToInterrupt(SONAR_INTR), collision, RISING);
}

void loop() {


  // initialisation loops
  if (starter_loops) {
    starter_loops--;
    flag = false;
    delay(500);
    return;
  }


  if (loop_count > 1000) {
    // check i2c connection
    // Wire.beginTransmission(Addr);
    // Wire.write(0x08);
    // if (Wire.endTransmission() != 0) {
    //   // if i2c failed reboot gyro
    //   while(!bno.begin()) {
    //     digitalWrite(PC13, LOW);
    //   }
    //   Wire.setClock(10000);
    //   bno.setExtCrystalUse(false);

    // }
    light = !light;
    digitalWrite(PC13, light);
    loop_count = 0;

    // get gyro data
    // sensors_event_t event; 
    // bno.getEvent(&event);

    // display_handler.clearDisplay();
    // display_handler.setTextSize(1);
    // display_handler.setTextColor(SSD1306_WHITE);
    // display_handler.setCursor(0,0);
    // display_handler.println(event.orientation.z);
    // display_handler.display();

    // speed adjust based on tilt
    // if (event.orientation.z + 6 > 5) {
    //   speed = SPEED * 0.5;
    // } else if (event.orientation.z + 6 < -5) {
    //   speed = SPEED * 1.5;
    // } else {
    //   speed = SPEED;
    // }
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
  // if ((sensor_values_tL[0] == 1) && (sensor_values_tR[1] == 1)) {
  //   binary_counts--;
  //   delay(100);
  // }

  // if (binary_counts == 2) {
  //   speed = SPEED * 1.5;
  // } else {
  //   speed = SPEED;
  // }
  t1 = millis();
  tau = t1 - t0;
  d_timer += tau;

  // D sampling
  if (d_timer > D_SAMPLE) {

    prevState = state;
    d_timer = 0;
  }
  t0 = t1;

  // PD
  output = getOutput();

  // check if outside of angle range
  if (abs(output) > MAX_ANGLE)
  {
    output = MAX_ANGLE * ((output > 0) ? 1 : -1);
  }
  // write angle to servo
  servo_write(SERVO_CENTER - output);

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
  if (n == 0) {
    local_speed = local_speed * 0.4;
  }

  // find diff speed adjustment
  int speed_adjust = (((double) angle - 60.0) / (2 * MAX_ANGLE)) * DIFF_STEERING * local_speed ;

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
  u = 0 - SENSOR_3_VALUE*sensor_values_tL[0] - SENSOR_2_VALUE*sensor_values_tL[1]  - SENSOR_1_VALUE * sensor_values_centres[0] + SENSOR_1_VALUE *sensor_values_centres[1] + SENSOR_2_VALUE*sensor_values_tR[0] + SENSOR_3_VALUE*sensor_values_tR[1];

  // find number of sensors
  n = sensor_values_tL[0] + sensor_values_tL[1] + sensor_values_centres[0] + sensor_values_centres[1] + sensor_values_tR[0] + sensor_values_tR[1];
  
  // set state if no sensors
  if (n == 0){
    return (state > 0) ? 3 : -3 ;
  }
  return (double) u / n;
}
