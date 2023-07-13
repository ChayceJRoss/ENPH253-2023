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

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// // // put function declarations here:

void print_constants(Adafruit_SSD1306 display_handler, int kp, int kd, int output, int state);

uint8_t light = HIGH;
void servo_write(int angle);
PD pd(0, 0);
int t = 0;
int tau = 0;
int sensor_values[] = {0, 0, 0};
int state = 0;
int kd = 0;
int kp = 0;
int output = 0;
void read_sensors();
void set_constants();

void setup() {
  pinMode(PA_1, OUTPUT);
  servo_setup();
  sensor_setup();
  set_constants();
  motor_setup();
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
  state = find_state(pd, sensor_values);

  output = pd.output(state, tau);
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
  sensor_values[0] = digitalRead(LEFT_SENSOR);
  sensor_values[1] = digitalRead(CENTER_SENSOR);
  sensor_values[2] = digitalRead(RIGHT_SENSOR);
}

void print_constants(Adafruit_SSD1306 display_handler, int kp, int kd, int output, int state) {
    display_handler.clearDisplay();
    display_handler.setCursor(0,0);
    display_handler.println("KP: " + String(kp));
    display_handler.println("KD: " + String(kd));
    display_handler.println("SENSORS: " + String(sensor_values[0]) + " ,"+ String(sensor_values[1]) + " ,"+  String(sensor_values[2]));
    display_handler.println("OUTPUT: " + String(output));
    display_handler.println("STATE: " + String(state));
    display_handler.display();
}

void servo_write(int angle) {
  int millisecs = map(angle, 0, 180, 500, 2500);
  int speed_adjust = (angle - 90) * 200; 
  pwm_start(SERVO, 50, millisecs, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // pwm_start(MOTOR_A_FORWARD, 500, 25000 + speed_adjust, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
  // pwm_start(MOTOR_B_FORWARD, 500, 25000 - speed_adjust, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
}