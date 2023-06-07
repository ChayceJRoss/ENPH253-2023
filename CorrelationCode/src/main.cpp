#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <convolutions.h> 

#define INPUTPIN PA0
#define SAMPLES 200
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int * take_samples();

void setup() {
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(INPUTPIN, INPUT);
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display_handler.display();
  delay(2000);

  // Displays "Hello world!" on the screen
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();
}

void loop() 
{
  int t0 = millis();
  int * signal_array = take_samples();
  int t1 = millis();
  int elapsed_time = t1 - t0;
  int * sample_array = make_samples(SAMPLES, elapsed_time);
  int * output_array = cross_correlation(sample_array, signal_array, SAMPLES);
  int max = max_of_array(output_array, SAMPLES);
  display_handler.clearDisplay();
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();
}

int * take_samples() {
  int sample_array[SAMPLES];
  for (int i = 0; i < SAMPLES; i++ ) {
    sample_array[i] = analogRead(INPUTPIN);
  }
  return sample_array;
}