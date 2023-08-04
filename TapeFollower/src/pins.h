//motor pins

#define MOTOR_A_FORWARD PA_1
#define MOTOR_A_BACKWARD PA_0
#define MOTOR_B_FORWARD PA_3
#define MOTOR_B_BACKWARD PA_2
#define SERVO PA_6//servo pwm pin number

//reflective sensor pins
#define s1 PA11 //front leftmost sensor pin 
#define s2 PA12 // 
#define s3 PA15 //
#define s4 PB3 //front rightmost sensor pin
#define s5 PB4 //bottom leftmost sensor pin
#define s6 PB5 
#define LEFT_ENCODER PB8
#define RIGHT_ENCODER PB9 //bottom rightmost sensor pin
#define KP_POT PA4 //potentiometer for kp
#define KD_POT PA5 //potentiometer for ki
#define CONSTANT_TRIGGER PA10 //button to trigger constant speed
#define PICKUP_0 PA8
#define PICKUP_1 PA9


//servo pin


void motor_setup();
void sensor_setup();
void servo_setup();
