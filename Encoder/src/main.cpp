#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define INTERRUPT_PIN PB11
#define MOTOR PA_0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // This display does not have a reset pin accessible

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile int count = 0;
int first_loops = 3;
void interrupt_handler();

void setup() {


  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);
  // pwm_start(MOTOR, 100, 20000, RESOLUTION_16B_COMPARE_FORMAT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interrupt_handler, FALLING);

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_handler.display();
  delay(2000);
  display_handler.clearDisplay();
  display_handler.display();
}
void loop() {
  if (first_loops > 0) {
    first_loops--;
  }

  delay(500);


  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println(count);
  display_handler.display();

  count = 0;
}

void interrupt_handler() {
  count++;
}