#ifndef DSP_DELAY_H
#define DSP_DELAY_H

#include <stdint.h>
#define SAMPLE_RATE 48000

//applies a delay, time is in seconds, feedback takes the values from 0 to 1
void delay(const float* audio_in, float* audio_out, uint32_t size, float time, float feedback, float* buffer, uint32_t buffer_size, uint32_t* buffer_counter);

#endif