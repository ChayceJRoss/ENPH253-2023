#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_SSD1306.h>
#include <utility/imumaths.h>
  


#define PWM_0 PA_0
#define PWM_1 PA_1
#define PWM_2 PA_2
#define PWM_3 PA_3
#define PWM_4 PA_6
#define PWM_5 PB_9
#define SONAR_INTR PB8


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // This display does not have a reset pin accessible

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool hb = HIGH;
volatile bool flag = false;
bool first_loop = true;
int loops = 3;

void collision_interrupt();

void setup() {

  pinMode(PWM_0, OUTPUT);
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);
  pinMode(PWM_3, OUTPUT);
  pinMode(PWM_4, OUTPUT);
  pinMode(PWM_5, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(SONAR_INTR, INPUT_PULLUP);

  digitalWrite(PC13, HIGH);



  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.display();
  delay(2000);
  display_handler.clearDisplay();
  display_handler.display();

  if(!bno.begin()) {
    while(1) {
      digitalWrite(PC13, hb);
      delay(1000);
      hb = !hb;
    }
  }

  

  bno.setExtCrystalUse(true);

  pwm_start(PWM_5, 250, 90, RESOLUTION_12B_COMPARE_FORMAT);
  // put your setup code here, to run once:

  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  flag = false;
  first_loop = true;
  attachInterrupt(digitalPinToInterrupt(SONAR_INTR), collision_interrupt, RISING);
}

void loop() {
  if (loops) {
    loops -= 1;
    flag = false;
    delay(500);
    return;
  }

  pwm_start(PWM_1, 100, 2000, RESOLUTION_16B_COMPARE_FORMAT);
  pwm_start(PWM_2, 100, 2000, RESOLUTION_16B_COMPARE_FORMAT);
  pwm_start(PWM_3, 100, 2000, RESOLUTION_16B_COMPARE_FORMAT);
  pwm_start(PWM_4, 560, 765, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

  // hb = !hb;

    /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);

  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println(event.orientation.z);
  display_handler.display();

  if (flag) {
    display_handler.clearDisplay();
    display_handler.setTextSize(1);
    display_handler.setTextColor(SSD1306_WHITE);
    display_handler.setCursor(0,0);
    display_handler.println("Collision!");
    display_handler.display();
    flag = false;
    delay(1000);
    display_handler.clearDisplay();
    display_handler.display();
    // return;
  }


}

void collision_interrupt(){
  flag = true;
  digitalWrite(PC13, LOW);
  delay(500);
  digitalWrite(PC13, HIGH);
  delay(500);
}