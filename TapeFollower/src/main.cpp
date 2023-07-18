// #include <Wire.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define OLED_RESET 	-1 // This display does not have a reset pin accessible
// Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void setup() {
//   display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
//   // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
//   display_handler.display();
//   delay(2000);

//   // Displays "Hello world!" on the screen
//   display_handler.clearDisplay();
//   display_handler.setTextSize(1);
//   display_handler.setTextColor(SSD1306_WHITE);
//   display_handler.setCursor(0,0);
//   display_handler.println("Hello world!");
//   display_handler.display();
// }

// void loop() {};

#include "includes.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define SPEED 25000
#define MAX_SPEED 60000
#define DIFF_STEERING 0.4
#define MAX_ANGLE 60


Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// // // put function declarations here:

void print_constants(Adafruit_SSD1306 display_handler, int kp, int kd, int output, int state);

uint8_t light = HIGH;
void servo_write(int angle);
PD pd(0, 0);
int t = 0;
int tau = 0;
int sensor_values_tL[] = {0, 0}; //front row, left side (ordered outermost sensor to innermost) FOR 6 and 4 setup
int sensor_values_tR[] = {0, 0}; //front row, right side (ordered innermost sensor to outermost) FOR 6 and 4 setup
int sensor_values_centres[] = {0,0}; //centre line followers for six sensors (0 for left, 1 for right)

int sensor_values_bL[] = {0, 0}; //back row, left side (ordered outermost sensor to innermost) in addition to tL and tR, for eight>
int sensor_values_bR[] = {0, 0}; //back row, right side (ordered outermost sensor to innermost) ^^

int state = 0;
int prevState = 0;
int kd = 0;
int kp = 0;
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

double d(int tau){
  double der;
  der = kd*(state - prevState)/tau;  
  return der;
}   

double p(){
  double prop;
  prop = kp*state;

  return prop;
}    

double getOutput(int new_state, int tau){
  resetState(new_state);
  double output;
  output = p() + d(tau);

  return output;
}

int getKp(){
  return kp;
}

int getKd(){
  return kd;
}


int find_state_eight()
{
  int trans = 0;
  int rot = 0;
  int sLeft = 0;
  int sRight = 0;
  int sRotL = 0;
  int sRotR = 0;
  int u;



  for(int i = 0; i < 2; i++){
    sLeft = sLeft + sensor_values_tL[i] + sensor_values_bL[i];
    sRight = sRight + sensor_values_tR[i] + sensor_values_bR[i];
    sRotL = sRotL + (sensor_values_tL[i] - sensor_values_bL[i]);
    sRotR = sRotR + (sensor_values_tR[i] - sensor_values_bR[i]);
  }

  trans = sRight - sLeft;
  rot = sRotR - sRotL;
  u = rot + trans;

  if(u ==0){
    if(sensor_values_tL[0] == 1 && sensor_values_tL[1] == sensor_values_tL[2] == sensor_values_tL[3] ==0){
      //top left corner only one high - maybe dont turn?? mayve check sensors again sooner
    }else if(sensor_values_tR[3] == 1 && sensor_values_tR[0] == sensor_values_tR[2] == sensor_values_tR[1] ==0){
      //top right corner only one high - maybe dont turn?? maybe check sensors again sooner 
    }else if(sensor_values_bL[0] == 1 && sensor_values_bL[1] == sensor_values_bL[2] == sensor_values_bL[3] ==0){
      //bottom left corner only one high - hard left
    }else if(sensor_values_bR[3] == 1 && sensor_values_bR[1] == sensor_values_bR[2] == sensor_values_bR[0] ==0){
      //bottom right corner only one high - hard right
    }
  }
  
  return u;
}

int find_state_six(){
  int sLeft = 0;
  int sRight = 0;
  int sCentre = 0;
  int u;

  for(int i = 0; i < 2; i++){
    sLeft = sLeft + sensor_values_tL[i];
    sRight = sRight + sensor_values_tR[i];
    sCentre = sCentre + sensor_values_centres[i];
  }
  u = (sRight - sLeft)/(sLeft+sRight+sCentre);
  return u;
}

int find_state_four(){
  int sLeft = 0;
  int sRight = 0;
  int u;

  for(int i = 0; i < 2; i++){
    sLeft = sLeft + sensor_values_tL[i];
    sRight = sRight + sensor_values_tR[i];
  }
  u = sRight - sLeft;
  return u;
}

void setup() {
  pinMode(PA_1, OUTPUT);
  set_constants();

  //turning servo setup
  pinMode(SERVO, OUTPUT);

  //inturrupt button setup
  pinMode(CONSTANT_TRIGGER, INPUT_PULLUP);

  //sensor setup
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(s5, INPUT_PULLUP); //add for six
  pinMode(s6, INPUT_PULLUP); //add for six
  //pinMode(s7, INPUT_PULLUP); //add for eight
  //pinMode(s8, INPUT_PULLUP); //add for eight

  //motor setup
  pinMode(MOTOR_A_FORWARD, OUTPUT);
  pinMode(MOTOR_A_BACKWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);
  pinMode(MOTOR_B_BACKWARD, OUTPUT);

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.display();
  delay(2000);
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();


  attachInterrupt(digitalPinToInterrupt(CONSTANT_TRIGGER), set_constants, RISING);
  t = millis();
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);
  delay(1000);
}

void loop() {

  light = !light;
  tau = millis() - t;
  t = millis();
  read_sensors();
  state = find_state_four();

  output = getOutput(state, tau);
  servo_write(90 - output);
  // print_constants(display_handler, pd.getKp(), pd.getKd(), output, state);
  delay(30);
}

void set_constants() {
  kp = analogRead(KP_POT) / 20;
  kd = analogRead(KD_POT) / 20;
  pd.newKd(0); 
  pd.newKp(kd);
}

void read_sensors() {
  sensor_values_tL[0] = digitalRead(s1);
  sensor_values_tL[1] = digitalRead(s2);
  sensor_values_tR[0] = digitalRead(s3);
  sensor_values_tR[1] = digitalRead(s4);

  sensor_values_centres[0] = digitalRead(s5); //add for six sensors
  sensor_values_centres[1] = digitalRead(s6); // add for six sensors


  //sensor_values_bL[0] = digitalRead(s5); //add for eight
  //sensor_values_bL[1] = digitalRead(s6); //add for eight
  //sensor_values_bR[0] = digitalRead(s7); //add for eight sensors
  //sensor_values_bR[1] = digitalRead(s8); //add for eight sensors
}

void print_constants(Adafruit_SSD1306 display_handler, int kp, int kd, int output, int state) {
    display_handler.clearDisplay();
    display_handler.setCursor(0,0);
    display_handler.println("KP: " + String(kp));
    display_handler.println("KD: " + String(kd));
    display_handler.println("FRONT SENSORS: " + String(sensor_values_tL[0]) + " ,"+ String(sensor_values_tL[1]) + " ,"+  String(sensor_values_centres[0])  + " ,"+  String(sensor_values_centres[1]) + " ," + String(sensor_values_tR[0]) + " ,"+ String(sensor_values_tR[1]));
    //display_handler.println("BACK SENSORS: " + String(sensor_values_bL[0]) + " ,"+ String(sensor_values_bL[1]) + " ,"+  String(sensor_values_bL[2])  + " ,"+  String(sensor_values_bL[3]) + " ," + String(sensor_values_bR[0]) + " ,"+ String(sensor_values_bR[1]) + " ,"+  String(sensor_values_bR[2])  + " ,"+  String(sensor_values_bR[3]));
    display_handler.println("OUTPUT: " + String(output));
    display_handler.println("STATE: " + String(state));
    display_handler.display();
}

void servo_write(int angle) {
  int local_angle = angle;
  if (abs(local_angle) > MAX_ANGLE) {
    local_angle = MAX_ANGLE * ( (angle > 0) ? 1 : -1);
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
  pwm_start(MOTOR_A_FORWARD, 500, left_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  pwm_start(MOTOR_B_FORWARD, 500, right_speed, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
}


