/*
current: current reading from sensors
target: desired reading from sensors
previous: the last reading from the sensors
elapsed_time: time between last pid execution and current excecution
kp: proportional coefficient
ki: integral coefficient
kd: derivatice coefficient
imax: limit for anti-windup
*/
int pid(double current, double target, double previous, double integral, int elapsed_time, double kp, double ki, double kd, double imax) {
    double error = target - current;
    double p = kp * error;
    double i = ki * (error + integral);
    if (i > imax) {
        i = imax;
    } else if (i < (-1 * imax)) {
        i = -1 * imax;
    }
    double d = kd * (error - previous) / (double) elapsed_time;
    return p + i + d;
}

