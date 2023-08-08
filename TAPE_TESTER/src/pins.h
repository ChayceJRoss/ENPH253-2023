//motor pins

#define MOTOR_A_FORWARD PB_8//forward pwm motor A (back left)
#define MOTOR_A_BACKWARD PB_9//backward pwm motor A
#define MOTOR_B_FORWARD PA_9////forward pwm motor B (back right)
#define MOTOR_B_BACKWARD PA_10//backward pwm motor B


//reflective sensor pins
#define s1 PA12 
#define s2 PB3
#define s3 PA11
#define s4 PB5 //top rightmost sensor pin
#define s5 PB4//bottom leftmost sensor pin
#define s6 PA15 
#define KP_POT PA4 //potentiomet er for kp
#define KD_POT PA5 //potentiometer for ki
#define CONSTANT_TRIGGER PA10 //button to trigger constant speed
#define TRIGPIN PB9
#define ECHOPIN PB8


//ir sensor pins
#define SENSOR_IR PA0//IR sensor pin

//servo pin
#define SERVO PA_6//servo pwm pin number

void motor_setup();
void sensor_setup();
void servo_setup();
