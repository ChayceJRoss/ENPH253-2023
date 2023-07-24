//motor pins

#define MOTOR_A_FORWARD PA_1
#define MOTOR_A_BACKWARD PA_0
#define MOTOR_B_FORWARD PA_2
#define MOTOR_B_BACKWARD PA_3


//reflective sensor pins
#define s1 PA11 //front leftmost sensor pin 
#define s2 PA12 // 
#define s3 PA15 //
#define s4 PB3 //front rightmost sensor pin
#define s5 PB4 //bottom leftmost sensor pin
#define s6 PB5 
#define s7 PB8
#define s8 PB9 //bottom rightmost sensor pin
#define KP_POT PA4 //potentiometer for kp
#define KD_POT PA5 //potentiometer for ki
#define CONSTANT_TRIGGER PA10 //button to trigger constant speed


//ir sensor pins
#define SENSOR_IR PA0//IR sensor pin

//servo pin
#define SERVO PA_6//servo pwm pin number

void motor_setup();
void sensor_setup();
void servo_setup();