#pragma once
#include <cmath>
#define PI 3.14159265

void make_samples(double * ref_array, int samples, int elapsed_time);
void cross_correlation(double * corr_array, double * ref_array, double * raw_array, int samples);
double max_of_array(double * array, int size);


