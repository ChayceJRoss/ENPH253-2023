//motor pins

#define MOTOR_A_FORWARD PA_1
#define MOTOR_A_BACKWARD PA_0
#define MOTOR_B_FORWARD PA_3
#define MOTOR_B_BACKWARD PA_2
#define SERVO PA_6//servo pwm pin number

//reflective sensor pins
#define s1 PA12 
#define s2 PB3
#define s3 PA11
#define s4 PB5 //top rightmost sensor pin
#define s5 PB4//bottom leftmost sensor pin
#define s6 PA15 
#define LIMIT_SWITCH PB9
#define SIGNAL_LED PB8 //bottom rightmost sensor pin
#define CONSTANT_TRIGGER PA10 //button to trigger constant speed
#define PICKUP_LEFT_FORWARD PA4
#define PICKUP_RIGHT_FORWARD PA5
#define PICKUP_LEFT_BACK PB_0
#define PICKUP_RIGHT_BACK PB_1
#define HALL_SENSOR_RIGHT PA9
#define HALL_SENSOR_LEFT PA8
//servo pin


void motor_setup();
void sensor_setup();
void servo_setup();
