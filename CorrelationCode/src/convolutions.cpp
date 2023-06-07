#include <convolutions.h>

/* 
int samples: number of samples to take
int * sample_array: array to store 1k array in
*/
int * make_samples(int samples, int elapsed_time) {
    int T = samples / (double) elapsed_time;
    int sample_array[2 * samples];
    for (int i = 0; i < 2 * samples; i++ ) {
        sample_array[i] = sin(i * 2 * PI / T);
    }
    return sample_array;
}

/*
sample_array: array of samples
signal_array: array of signal
samples: number of samples to take
int * cross_correlation: array to store cross correlation in
*/
int * cross_correlation(int * sample_array, int * signal_array, int samples) {
    int * output_array = new int[samples];
    for (int i = 0; i < samples; i++) {
        output_array[i] = 0;
        for (int j = 0; j < samples; j++) {
            output_array[i] += sample_array[i + j] * signal_array[j];
        }
    }
    return output_array;
}

int max_of_array(int * array, int size) {
    int max = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}