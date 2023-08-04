#define SPEED_KP 1000
#define SPEED_KD 0
#define SPEED_KI 0
#define SPEED_WINDUP 1000

int speedChange(int error, int delta_time, int prev_error, int integral);
int getSpeedIntegral(int prev_integral, int error);
int getSpeedError(int target, int current);
