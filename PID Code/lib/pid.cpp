int pid(double current, double target, double previous, double integral, int elapsed_time, double kp, double ki, double kd, double imax) {
    double error = target - current;
    double p = kp * error;
    double i = ki * (error + integral);
    if (i > imax) {
        i = imax;
    } else if (i < (-1 * imax)) {
        i = -1 * imax;
    }
    double d = kd * (error - previous) / elapsed_time;
    return p + i + d;
}

