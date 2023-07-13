//motor pins

#define MOTOR_A_FORWARD PB_8//forward pwm motor A (back left)
#define MOTOR_A_BACKWARD PB_9//backward pwm motor A
#define MOTOR_B_FORWARD PA_9////forward pwm motor B (back right)
#define MOTOR_B_BACKWARD PA_10//backward pwm motor B


//reflective sensor pins
#define LEFT_SENSOR PB3 //leftmost sensor pin (a)
#define RIGHT_SENSOR PB4 //central sensor pin (b)
#define CENTER_SENSOR PB5 //rightmost sensor pin (c)
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