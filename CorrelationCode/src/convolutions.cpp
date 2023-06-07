#include "convolutions.h"

/* 
int samples: number of samples to take
int * sample_array: array to store 1k array in
*/
void make_samples(double * ref_array, int samples, int elapsed_time) {
    double T = (double) samples / (double) elapsed_time;
    for (int i = 0; i < 2 * samples; i++ ) {
        ref_array[i] = sin(i * 2 * PI / T);
    }
}

/*
sample_array: array of samples
signal_array: array of signal
samples: number of samples to take
int * cross_correlation: array to store cross correlation in
*/
void cross_correlation(double * corr_array, double * ref_array, double * raw_array, int samples) {
    for (int i = 0; i < samples; i++) {
        corr_array[i] = 0;
        for (int j = 0; j < samples; j++) {
            corr_array[i] += ref_array[i + j] * raw_array[j];
        }
    }
}

double max_of_array(double * array, int size) {
    int max = 0;
    for (int i = 0; i < size; i++) {
        if (abs(array[i]) > max) {
            max = array[i];
        }
    }
    return max;
}