#include "includes.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define SPEED 35000
#define MAX_SPEED 60000
#define DIFF_STEERING 1.0
#define MAX_ANGLE 50
#define P_CONST 10.0
#define D_CONST 250.0
// // // put function declarations here:

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_constants();

void servo_write(int angle);
int t = 0;
int tau = 0;
int sensor_values_tL[] = {0, 0}; //front row, left side (ordered outermost sensor to innermost) FOR 6 and 4 setup
int sensor_values_tR[] = {0, 0}; //front row, right side (ordered innermost sensor to outermost) FOR 6 and 4 setup
int sensor_values_centres[] = {0,0}; //centre line followers for six sensors (0 for left, 1 for right)

int sensor_values_bL[] = {0, 0}; //back row, left side (ordered outermost sensor to innermost) in addition to tL and tR, for eight>
int sensor_values_bR[] = {0, 0}; //back row, right side (ordered outermost sensor to innermost) ^^

double state = 0;
int prevState = 0;
int kd = D_CONST;
int kp = P_CONST;
double output = 0;
void read_sensors();
void set_constants();

//PD "class"

void newKp(double new_Kp){
  kp = new_Kp;
}    
    
void newKd(double new_Kd){
  kd = new_Kd;
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
  if (tau == 0) return 0;
  der = kd*(state - prevState)/(double) tau;  
  return der;
}   

double p(){
  double prop;
  prop = kp*state;

  return prop;
}    

int getOutput(){
  double output;
  output = p() + d();

  return (int) output;
}

int getKp(){
  return kp;
}

int getKd(){
  return kd;
}

double find_state_six(){
  int u;
  int n;

  u = -3*sensor_values_tL[0] -2*sensor_values_tL[1]  - sensor_values_centres[0] + sensor_values_centres[1] + 2*sensor_values_tR[0] + 3*sensor_values_tR[1];
  n = sensor_values_tL[0] + sensor_values_tL[1] + sensor_values_centres[0] + sensor_values_centres[1] + sensor_values_tR[0] + sensor_values_tR[1];
  if (n == 0){
    return (state > 0) ? 4 : -4;
  }
  return (double) u / n;
}

void setup() {

  

  // //turning servo setup

  // //inturrupt button setup
  pinMode(CONSTANT_TRIGGER, INPUT_PULLUP);

  // //sensor setup
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(s5, INPUT_PULLUP); //add for six
  pinMode(s6, INPUT_PULLUP); //add for six
  //pinMode(s7, INPUT_PULLUP); //add for eight
  //pinMode(s8, INPUT_PULLUP); //add for eight

  // //motor setup

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you
  // don't want this.
  display_handler.display();
  delay(2000);

  // Displays "Hello world!" on the screen
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();

  // set_constants();
  // attachInterrupt(digitalPinToInterrupt(CONSTANT_TRIGGER), set_constants, RISING);
  t = micros();
  delay(1000);
}

void loop() {
  tau = micros() - t;
  t = micros();
  read_sensors();
  prevState = state;
  state = find_state_six();
  output = getOutput();
  // servo_write(90 - output);
  delay(50);
  print_constants();
}

void set_constants() {
  kp = analogRead(KP_POT) / 20;
  kd = analogRead(KD_POT) / 20;
}

void read_sensors() {
  sensor_values_tL[0] = digitalRead(s1);
  sensor_values_tL[1] = digitalRead(s2);
  sensor_values_centres[0] = digitalRead(s3); //add for six sensors
  sensor_values_centres[1] = digitalRead(s4); // add for six sensors
  sensor_values_tR[0] = digitalRead(s5);
  sensor_values_tR[1] = digitalRead(s6);
 

}

void print_constants() {
    display_handler.clearDisplay();
    display_handler.setCursor(0,0);
    display_handler.println("KP: " + String(kp));
    display_handler.println("KD: " + String(kd));
    display_handler.println("SENSORS: " + String(sensor_values_tL[0]) + " "+ String(sensor_values_tL[1]) + " "+  String(sensor_values_centres[0])  + " "+  String(sensor_values_centres[1]) + " " + String(sensor_values_tR[0]) + " "+ String(sensor_values_tR[1]));
    display_handler.println("STATE: " + String(state));
    display_handler.println("OUTPUT: " + String(output));
    display_handler.display();
}

void servo_write(int angle)
{
  int local_angle = angle - 90;
  if (abs(local_angle) > MAX_ANGLE)
  {
    local_angle = 90 + (MAX_ANGLE * ((local_angle > 0) ? 1 : -1));
  }
  else
  {
    local_angle = angle;
  }
  int millisecs = map(local_angle, 0, 180, 500, 2500);
  int speed_adjust = (((double) angle - 90.0) / 60.0) * DIFF_STEERING * SPEED ;
  int left_speed = SPEED + speed_adjust;
  int right_speed = SPEED - speed_adjust;

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
  pwm_start(SERVO, 50, millisecs, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(20);
  pwm_start(MOTOR_A_FORWARD, 500, SPEED, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  // // delay(20);
  pwm_start(MOTOR_B_FORWARD, 500, SPEED, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
}