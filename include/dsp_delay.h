#ifndef DSP_DELAY_H
#define DSP_DELAY_H

#include <stdint.h>
#define SAMPLE_RATE 48000

typedef struct {
    float* buffer;
    uint32_t size;
    uint32_t counter;
} delay_line;

static inline void delay_line_init(delay_line* target_delay_line, float* buffer, uint32_t size){
    target_delay_line->buffer = buffer;
    target_delay_line->size = size;
    target_delay_line->counter = 0;
    for (uint32_t counter = 0; counter < size; counter++){
        buffer[counter] = 0.0f;
    }
}

#define DELAY_LINE(name, max_length_ms) \
    static float name##_data[((SAMPLE_RATE)*(max_length_ms) / 1000) + 1]; \
    static delay_line name

#define DELAY_LINE_INIT(name) \
    delay_line_init(&name, name##_data, sizeof(name##_data)/sizeof(float))

//applies a delay, time is in miliseconds, feedback takes the values from 0 to 1. needs a initialised delay line
void delay(const float* audio_in, float* audio_out, uint32_t size, uint32_t time_ms, float feedback, delay_line* target_delay_line);

#endif