//motor pins

#define MOTOR_A_FORWARD PB_8//forward pwm motor A (back left)
#define MOTOR_A_BACKWARD PB_9//backward pwm motor A
#define MOTOR_B_FORWARD PA_9////forward pwm motor B (back right)
#define MOTOR_B_BACKWARD PA_10//backward pwm motor B


//reflective sensor pins
#define s1 PB3 //top leftmost sensor pin 
#define s2 PB11 //
#define s3 PB12 //
#define s4 PB6 //top rightmost sensor pin
#define s5 PB7 //bottom leftmost sensor pin
#define s6 PB8 
#define s7 PB9
#define s8 PB10 //bottom rightmost sensor pin
#define KP_POT PA4 //potentiometer for kp
#define KD_POT PA5 //potentiometer for ki
#define CONSTANT_TRIGGER PA15 //button to trigger constant speed


//ir sensor pins
#define SENSOR_IR PA0//IR sensor pin

//servo pin
#define SERVO PA_6//servo pwm pin number

void motor_setup();
void sensor_setup();
void servo_setup();